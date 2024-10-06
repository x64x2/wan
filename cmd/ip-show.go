package cmd

import (
	"fmt"
	"net/netip"
	"os"
	"time"

	"github.com/spf13/cobra"
)

// showCmd represents the show command
var ipshowCmd = &cobra.Command{
	Use:     "show",
	Short:   "Show IP and associated name",
	Long:    `Show IP and associated name`,
	Aliases: []string{"s"},
	Args:    cobra.ExactArgs(1),
	Example: "ipam ip show 192.168.0.1",
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

		addr, addrexists := subnet.GetIP(ip)
		if !addrexists {
			fmt.Printf("[ERROR] Couldn't find IP %v\n", ip.String())
			os.Exit(1)
		}

		fmt.Printf("IP:          %v\n", ip.String())
		fmt.Printf("FQDN:        %v\n", addr.FQDN)
		fmt.Printf("Subnet:      %v (%v, vlan %v)\n", subnet.Subnet.String(), subnet.Name, subnet.Vlan)
		fmt.Printf("Modified at: %v\n", subnet.ChangedAt.Format(time.RFC1123))
		fmt.Printf("Modified by: %v\n", subnet.ChangedBy)
	},
}

func init() {
	ipCmd.AddCommand(ipshowCmd)
}