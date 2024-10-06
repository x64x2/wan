package cmd

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"net/http"
	"net/netip"
	"strings"

	"github.com/spf13/viper"
)

type DNSZone struct {
	ID         string            `json:"id"`
	Name       string            `json:"name"`
	Kind       string            `json:"kind"`
	RRsets     []DNSRecordSet    `json:"rrsets"`
	Metadata   map[string]string `json:"metadata"`
	DNSSEC     bool              `json:"dnssec"`
	NSEC3Param string            `json:"nsec3param,omitempty"`
	Account    string            `json:"account,omitempty"`
	Serial     int               `json:"serial"`
}

type Patch struct {
	Rrsets []DNSRecordSet `json:"rrsets"`
}

// GetRecord checks if a given Record already exists in the DNSRecordSet list.
func (z DNSZone) GetRecord(fqdn string, rtype string, rcontent string) (DNSRecordSet, bool) {
	if !strings.HasSuffix(fqdn, ".") {
		fqdn = fqdn + "."
	}
	if (rtype == "PTR") && !strings.HasSuffix(rcontent, ".") {
		rcontent = rcontent + "."
	}
	for _, recordset := range z.RRsets {
		if recordset.Name == fqdn && recordset.Type == rtype {
			for _, record := range recordset.Records {
				if record.Content == rcontent {
					return recordset, true
				}
			}
		}
	}
	return DNSRecordSet{}, false
}

// SendPATCH sends a PATCH API request for DNSZone z. Returns error or nil
func (z DNSZone) SendPATCH(record string, value string, recordtype string, changetype string) error {
	pdnsendpoint := viper.GetString("powerdnsendpoint")
	pdnsapikey := viper.GetString("powerdnsapikey")
	debug, _ := rootCmd.Flags().GetBool("debug")

	if !viper.GetBool("powerdnsenabled") {
		return nil
	}

	url := pdnsendpoint + "/api/v1/servers/localhost/zones/" + z.Name
	if debug {
		fmt.Println("[DEBUG] PowerDNS URL: " + url)
	}

	rset := DNSRecordSet{}
	rset.Changetype = changetype
	rset.Name = strings.Join([]string{record, z.Name}, ".")
	rset.TTL = 3600
	rset.Type = recordtype
	rec := DNSRecord{}
	if recordtype == "PTR" {
		rec.Content = value + "."
	} else {
		rec.Content = value
	}
	rset.Records = append(rset.Records, rec)

	patch := Patch{}
	patch.Rrsets = append(patch.Rrsets, rset)

	payload, marsherr := json.Marshal(patch)
	if marsherr != nil {
		return marsherr
	}

	req, reqerr := http.NewRequest("PATCH", url, bytes.NewBuffer(payload))
	if reqerr != nil {
		return reqerr
	}

	req.Header.Add("X-API-Key", pdnsapikey)
	req.Header.Add("Content-Type", "application/json")

	client := &http.Client{}
	resp, _ := client.Do(req)
	if resp.StatusCode != 204 {
		defer resp.Body.Close()

		body, readerr := io.ReadAll(resp.Body)
		if readerr != nil {
			fmt.Println(readerr)
		}
		return fmt.Errorf("[HTTP ERROR] %v: %q", resp.Status, string(body))
	}
	return nil
}

type DNSRecordSet struct {
	Name       string      `json:"name"`
	Type       string      `json:"type"`
	TTL        int         `json:"ttl"`
	Records    []DNSRecord `json:"records"`
	Changetype string      `json:"changetype,omitempty"`
}

type DNSRecord struct {
	Content  string `json:"content"`
	Disabled bool   `json:"disabled,omitempty"`
	SetPTR   bool   `json:"set-ptr,omitempty"`
}

// Returns the DNSZone and nil if successful, empty DNSZone and an
// error otherwise
func GetDNSZone(zone string) (DNSZone, error) {
	if !strings.HasSuffix(zone, ".") {
		zone = zone + "."
	}
	pdnsendpoint := viper.GetString("powerdnsendpoint")
	pdnsapikey := viper.GetString("powerdnsapikey")

	url := pdnsendpoint + "/api/v1/servers/localhost/zones/" + zone

	req, reqerr := http.NewRequest("GET", url, nil)
	if reqerr != nil {
		fmt.Println(reqerr)
		return DNSZone{}, reqerr
	}

	req.Header.Add("X-API-Key", pdnsapikey)
	req.Header.Add("Accept", "application/json")

	client := &http.Client{}
	resp, resperr := client.Do(req)
	if resperr != nil {
		fmt.Println(resperr)
		return DNSZone{}, resperr
	}
	defer resp.Body.Close()

	body, readerr := io.ReadAll(resp.Body)
	if readerr != nil {
		fmt.Println(readerr)
		return DNSZone{}, readerr
	}

	var zoneobj DNSZone
	marsherr := json.Unmarshal(body, &zoneobj)
	if marsherr != nil {
		fmt.Println(marsherr)
		return DNSZone{}, marsherr
	}

	return zoneobj, nil
}

// GetBestDNSZone requests a list of all zones from PowerDNS
// and determines the best zone, if possible.
func GetBestDNSZone(fqdn string) (DNSZone, error) {
	pdnsendpoint := viper.GetString("powerdnsendpoint")
	pdnsapikey := viper.GetString("powerdnsapikey")
	fqdn = fqdn + "."

	if !viper.GetBool("powerdnsenabled") {
		return DNSZone{}, errors.New("PowerDNS integration not enabled")
	}

	url := pdnsendpoint + "/api/v1/servers/localhost/zones"

	req, reqerr := http.NewRequest("GET", url, nil)
	if reqerr != nil {
		fmt.Println(reqerr)
		return DNSZone{}, reqerr
	}

	req.Header.Add("X-API-Key", pdnsapikey)

	client := &http.Client{}
	resp, resperr := client.Do(req)
	if resperr != nil {
		fmt.Println(resperr)
		return DNSZone{}, resperr
	}
	defer resp.Body.Close()

	body, readerr := io.ReadAll(resp.Body)
	if readerr != nil {
		fmt.Println(readerr)
		return DNSZone{}, readerr
	}

	var zones []DNSZone
	marsherr := json.Unmarshal(body, &zones)
	if marsherr != nil {
		fmt.Println(marsherr)
		return DNSZone{}, marsherr
	}

	var bestmatch DNSZone
	var matchfound = false

	for _, zone := range zones {
		if strings.HasSuffix(fqdn, "."+zone.Name) {
			if !matchfound {
				matchfound = true
				bestmatch = zone
			}
			if matchfound && len(zone.Name) > len(bestmatch.Name) {
				bestmatch = zone
			}
		}
	}

	if !matchfound {
		return DNSZone{}, errors.New("No suitable zone found for " + fqdn)
	}

	zone, geterr := GetDNSZone(bestmatch.ID)
	if geterr != nil {
		return DNSZone{}, geterr
	}

	return zone, nil
}

// AddDNSFqdn tries to create forward and reverse lookup records
func AddDNSFqdn(fqdn string, addr netip.Addr) error {
	debug, _ := rootCmd.Flags().GetBool("debug")
	if !viper.GetBool("powerdnsenabled") {
		if debug {
			fmt.Println("[INFO] PowerDNS integration disabled, skipping DNS operations.")
		}
		return nil
	}

	var recordtype string
	if addr.Is4() {
		recordtype = "A"
	} else if addr.Is6() {
		recordtype = "AAAA"
	} else {
		return errors.New(addr.String() + " is not a valid IP address")
	}

	fzone, fzoneerr := GetBestDNSZone(fqdn)
	if fzoneerr != nil {
		fmt.Printf("[DNS] No suitable zone found for %v, skipping DNS op\n", fqdn)
	} else {

		_, frecordexists := fzone.GetRecord(fqdn, recordtype, addr.String())

		if frecordexists {
			fmt.Printf("[DNS] DNS Record for %v already exists, no need to change DNS.\n", fqdn)
		} else {
			var dotfqdn, dotfzone = fqdn + ".", "." + fzone.Name
			record := strings.Replace(dotfqdn, dotfzone, "", 1)
			fpatcherr := fzone.SendPATCH(record, addr.String(), recordtype, "REPLACE")
			if fpatcherr != nil {
				return fpatcherr
			}
			fmt.Printf("[DNS] + %v IN %v %v\n", fqdn, recordtype, addr.String())
		}
	}

	baseip := addr.StringExpanded()
	var rfqdn string
	if addr.Is4() {
		a := strings.Split(baseip, ".")
		b := reverse(a)
		rfqdn = strings.Join(b, ".") + ".in-addr.arpa"
	} else if addr.Is6() {
		a := strings.Replace(baseip, ":", "", -1)
		b := strings.Split(a, "")
		c := reverse(b)
		rfqdn = strings.Join(c, ".") + ".ip6.arpa"
	}

	rzone, rzoneerr := GetBestDNSZone(rfqdn)
	if rzoneerr != nil {
		fmt.Printf("[DNS] No suitable zone found for %v, skipping DNS op\n", rfqdn)
	} else {

		_, rrecordexists := rzone.GetRecord(rfqdn, "PTR", fqdn)
		rhost := strings.Replace(rfqdn+".", "."+rzone.Name, "", 1)

		if rrecordexists {
			fmt.Printf("[DNS] Reverse DNS Record for %v already exists, no need to change DNS.\n", addr.String())
		} else {
			rpatcherr := rzone.SendPATCH(rhost, fqdn, "PTR", "REPLACE")
			if rpatcherr != nil {
				return rpatcherr
			}
			fmt.Printf("[DNS] + %v IN %v %v\n", rfqdn, "PTR", fqdn)
		}
	}

	return nil
}

// DeleteDNSFqdn tries to delete the corresponding record for
func DeleteDNSFqdn(fqdn string, addr netip.Addr) error {
	debug, _ := rootCmd.Flags().GetBool("debug")
	if !viper.GetBool("powerdnsenabled") {
		if debug {
			fmt.Println("[INFO] PowerDNS integration disabled, skipping DNS operations.")
		}
		return nil
	}

	var recordtype string
	if addr.Is4() {
		recordtype = "A"
	} else if addr.Is6() {
		recordtype = "AAAA"
	} else {
		return errors.New(addr.String() + " is not a valid IP address")
	}

	fzone, fzoneerr := GetBestDNSZone(fqdn)
	if fzoneerr != nil {
		fmt.Printf("[DNS] No suitable zone found for %v, skipping DNS delete op\n", fqdn)
	} else {
		_, frecordexists := fzone.GetRecord(fqdn, recordtype, addr.String())

		if !frecordexists {
			fmt.Printf("[DNS] DNS Record for %v doesn't exists, no need to change DNS.\n", fqdn)
		} else {
			fpatcherr := fzone.SendPATCH(strings.Replace(fqdn+".", "."+fzone.Name, "", 1), addr.String(), recordtype, "DELETE")
			if fpatcherr != nil {
				return fpatcherr
			}
			fmt.Printf("[DNS] - %v IN %v %v\n", fqdn, recordtype, addr.String())
		}
	}

	baseip := addr.StringExpanded()
	var rfqdn string
	if addr.Is4() {
		a := strings.Split(baseip, ".")
		b := reverse(a)
		rfqdn = strings.Join(b, ".") + ".in-addr.arpa"
	} else if addr.Is6() {
		a := strings.Replace(baseip, ":", "", -1)
		b := strings.Split(a, "")
		c := reverse(b)
		rfqdn = strings.Join(c, ".") + ".ip6.arpa"
	}

	rzone, rzoneerr := GetBestDNSZone(rfqdn)
	if rzoneerr != nil {
		fmt.Printf("[DNS] No suitable zone found for %v, skipping DNS delete op\n", rfqdn)
	} else {
		_, rrecordexists := rzone.GetRecord(rfqdn, "PTR", fqdn)
		rhost := strings.Replace(rfqdn+".", "."+rzone.Name, "", 1)

		if !rrecordexists {
			fmt.Printf("[DNS] Reverse DNS Record for %v doesn't exists, no need to change DNS.\n", addr.String())
		} else {
			rpatcherr := rzone.SendPATCH(rhost, fqdn, "PTR", "DELETE")
			if rpatcherr != nil {
				return rpatcherr
			}
			fmt.Printf("[DNS] - %v IN %v %v\n", rfqdn, "PTR", fqdn)
		}
	}

	return nil
}
