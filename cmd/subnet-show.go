package cmd

import (
	"fmt"
	"net/netip"
	"os"
	"time"

	"github.com/spf13/cobra"
)

// showCmd represents the show command
var subnetshowCmd = &cobra.Command{
	Use:   "show subnet",
	Short: "Displays a subnet.",
	Long: `Displays a subnets details like name and vlan tag, 
aswell as a list of containing IP addresses`,
	Args:    cobra.ExactArgs(1),
	Aliases: []string{"s"},
	Example: "ipam subnet show 192.168.0.0/24\nipam subnet show 2001:db8::/64",
	Run: func(cmd *cobra.Command, args []string) {
		net, parseerr := netip.ParsePrefix(args[0])

		if parseerr != nil {
			fmt.Println("[ERROR]", parseerr)
			os.Exit(1)
		}

		if !SubnetExists(net) {
			fmt.Printf("[ERROR] no subnet found for prefix: %v\n", args[0])
			os.Exit(1)
		}

		subnet, subneterr := GetSubnet(net)
		if subneterr != nil {
			fmt.Println("[ERROR]", subneterr)
			os.Exit(1)
		}
		fmt.Printf("\n")
		fmt.Printf("Name:         %v\n", subnet.Name)
		fmt.Printf("Vlan:         %v\n", subnet.Vlan)
		fmt.Printf("Prefix:       %v\n", subnet.Subnet)
		fmt.Printf("Modified at:  %v\n", subnet.ChangedAt.Format(time.RFC1123))
		fmt.Printf("Modified by:  %v\n\n", subnet.ChangedBy)

		//fmt.Printf("%v:\n", subnet.Subnet)

		addrlist := SortAddresses(subnet.Addresses)

		if subnet.Subnet.Addr().Is4() {

			fmt.Printf("%-15s  Hostname\n", "IP Address")
			for _, element := range addrlist {
				if element.FQDN == "" {
					fmt.Printf("%v\n", element.IP.String())
				} else {
					fmt.Printf("%-15s  %v\n", element.IP.String(), element.FQDN)
				}
			}
		} else {

			fmt.Printf("%-39s  Hostname\n", "IP Address")

			for _, element := range addrlist {
				if element.FQDN == "" {
					fmt.Printf("%v\n", element.IP.String())
				} else {
					fmt.Printf("%-39s  %v\n", element.IP.String(), element.FQDN)
				}
			}
		}

	},
}

func init() {
	subnetCmd.AddCommand(subnetshowCmd)
}