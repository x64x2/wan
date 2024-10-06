package cmd

import (
	"fmt"
	"net/netip"
	"os"
	"os/user"
	"time"

	"github.com/spf13/cobra"
)

// deleteCmd represents the delete command
var ipdeleteCmd = &cobra.Command{
	Use:     "delete ipaddress",
	Short:   "Delete an IP address",
	Long:    `Delete an IP address`,
	Aliases: []string{"d"},
	Args:    cobra.ExactArgs(1),
	Example: "ipam ip delete 192.168.0.1",
	Run: func(cmd *cobra.Command, args []string) {
		ip, parseerr := netip.ParseAddr(args[0])

		if parseerr != nil {
			fmt.Println("[ERROR]", parseerr)
			os.Exit(1)
		}

		subnet, subnetexists := FindBestSubnet(ip)
		if !subnetexists {
			fmt.Printf("[ERROR] Couldn't find IP %v\n", ip.String())
			os.Exit(1)
		}

		address, _ := subnet.GetIP(ip)

		subnet, removeerr := subnet.RemoveIP(ip)
		if removeerr != nil {
			fmt.Println("[ERROR]", removeerr)
			os.Exit(1)
		}

		currentuser, _ := user.Current()
		subnet.ChangedAt = time.Now()
		subnet.ChangedBy = currentuser.Username

		writeerr := subnet.WriteSubnet()
		if writeerr != nil {
			fmt.Println("[ERROR]", writeerr)
			os.Exit(1)
		}

		if address.FQDN == "" {
			fmt.Printf("deleted ip %v\n", address.IP.String())
		} else {
			fmt.Printf("deleted ip %v (%v)\n", address.IP.String(), address.FQDN)
			dnserr := DeleteDNSFqdn(address.FQDN, address.IP)
			if dnserr != nil {
				fmt.Println("[ERROR]", writeerr)
				os.Exit(1)
			}
		}
	},
}

func init() {
	ipCmd.AddCommand(ipdeleteCmd)
}
