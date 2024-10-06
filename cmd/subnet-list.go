package cmd

import (
	"fmt"
	"math"
	"net/netip"
	"os"
	"sort"

	"github.com/spf13/cobra"
)

// listCmd represents the list command
var subnetlistCmd = &cobra.Command{
	Use:     "list",
	Short:   "List all subnets",
	Long:    `List all subnets`,
	Aliases: []string{"l"},
	Args:    cobra.ExactArgs(0),
	Example: "cmdb subnet list",
	Run: func(cmd *cobra.Command, args []string) {
		//verbose, _ := cmd.Flags().GetBool("verbose")
		subnetlist := ListSubnets()
		var subnets []Subnet

		for _, subnet := range subnetlist {
			prefix, _ := netip.ParsePrefix(subnet)
			net, err := GetSubnet(prefix)
			if err != nil {
				fmt.Println("[ERROR]", err)
				os.Exit(1)
			}
			subnets = append(subnets, net)
		}
		sort.Slice(subnets, func(i, j int) bool {
			return subnets[i].Subnet.Addr().Less(subnets[j].Subnet.Addr())
		})

		fmt.Printf("%-18s  VLAN  %-25s  Free IPs\n", "Prefix", "Name")

		for _, subnet := range subnets {
			var numip, freeip int

			if subnet.Subnet.Addr().Is4() {
				hostbits := float64(32 - subnet.Subnet.Bits())
				if subnet.Subnet.Bits() == 31 {
					numip = 2
				} else {
					numip = int(math.Pow(2, hostbits)) - 2
				}

				freeip = numip - len(subnet.Addresses)

				if freeip > 1000 {

					fmt.Printf("%-18s  %-4s  %-25s  >1000\n", subnet.Subnet, subnet.Vlan, subnet.Name)
				} else {
					//fmt.Printf("| %-20s | %-20s |\n", "vegetables", "fruits")
					fmt.Printf("%-18s  %-4s  %-25s  %v\n", subnet.Subnet, subnet.Vlan, subnet.Name, freeip)
				}
			} else {
				fmt.Printf("%v:\t%v\t(vl: %v)\n", subnet.Subnet, subnet.Name, subnet.Vlan)
				
			}

		}
	},
}

func init() {
	subnetCmd.AddCommand(subnetlistCmd)
}