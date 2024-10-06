package cmd

import (
	"errors"
	"math"
	"net/netip"
	"time"
)

type Subnet struct {
	Subnet    netip.Prefix `json:"subnet"`
	Name      string       `json:"name"`
	Vlan      string       `json:"vlan"`
	ChangedAt time.Time    `json:"changedat,omitempty"`
	ChangedBy string       `json:"changedby,omitempty"`
	Addresses []Address    `json:"addresses"`
}

// GetIPCount gets the IP count for the Subnet
// Returns the IP count or -1 if it's a IPv6 prefix
func (s Subnet) GetIPCount() int {
	if !s.Subnet.Addr().Is4() {
		return -1
	}
	hostbits := float64(32 - s.Subnet.Bits())
	if s.Subnet.Bits() == 31 {
		return 2
	} else {
		return int(math.Pow(2, hostbits)) - 2
	}
}

// HasIP checks if a Subnet already contains given netip.Addr.
// Returns true if the IP already is present, false otherwise.
func (s Subnet) HasIP(ip netip.Addr) bool {
	iscontained := false

	for _, element := range s.Addresses {
		if element.IP.Compare(ip) == 0 {
			iscontained = true
		}
	}

	return iscontained
}

// FindFirstFreeIP finds and returns the next free netip.Addr
// or an invalid netip.Addr if no free IP was found
func (s Subnet) FindFirstFreeIP() netip.Addr {
	var ip netip.Addr

	if s.Subnet.Addr().Is4() {
		subnetips := s.GetIPCount()

		// handling /31 prefixes
		if subnetips == 2 {
			ip = s.Subnet.Addr()
		} else {
			ip = s.Subnet.Addr().Next()
		}

		// start at first free IP
		for count := 0; count < subnetips; count++ {

			if s.HasIP(ip) {
				ip = ip.Next()
			} else {
				return ip
			}
		}
	} else {
		ip = s.Subnet.Addr().Next()
		for ; s.Subnet.Contains(ip); ip = ip.Next() {
			if !s.HasIP(ip) {
				return ip
			}
		}
	}

	return netip.Addr{}
}

// Returns the changed Subnet and nil if delete was
// successful, or an empty Subnet and an error if
// ip could not be deleted.
func (s Subnet) RemoveIP(ip netip.Addr) (Subnet, error) {
	var addrlist []Address

	if !s.HasIP(ip) {

		return Subnet{}, errors.New("IP " + ip.String() + " wasn't found in subnet " + s.Subnet.String())
	}

	for _, item := range s.Addresses {
		if item.IP.Compare(ip) != 0 {
			addrlist = append(addrlist, item)
		}
	}
	s.Addresses = addrlist
	return s, nil
}

// GetIP returns the Address object for the subnet 
// Returns the Address object and true if a corresponding
// object was found, an empty Address and false otherwise.
func (s Subnet) GetIP(ip netip.Addr) (Address, bool) {
	for _, item := range s.Addresses {
		if item.IP.Compare(ip) == 0 {
			return item, true
		}
	}

	return Address{}, false
}

type Address struct {
	IP        netip.Addr `json:"ip"`
	FQDN      string     `json:"fqdn"`
	ChangedAt time.Time  `json:"changedat,omitempty"`
	ChangedBy string     `json:"changedby,omitempty"`
}
