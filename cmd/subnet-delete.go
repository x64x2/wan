package cmd

import (
	"fmt"
	"net/netip"
	"os"

	"github.com/spf13/cobra"
)

// deleteCmd represents the delete command
var subnetdeleteCmd = &cobra.Command{
	Use:     "delete",
	Short:   "delete subnet",
	Long:    `Delete a subnet from the ipam.`,
	Args:    cobra.ExactArgs(1),
	Aliases: []string{"d"},
	Example: "ipam subnet delete 192.168.0.0/24",
	Run: func(cmd *cobra.Command, args []string) {
		subnet, parseerr := netip.ParsePrefix(args[0])
		if parseerr != nil {
			fmt.Println("[ERROR]", parseerr)
			os.Exit(1)
		}

		if !SubnetExists(subnet) {
			fmt.Printf("[ERROR] Couldn't find subnet %v\n", subnet.String())
			os.Exit(1)
		}

		subnetobj, suberr := GetSubnet(subnet)
		if suberr != nil {
			fmt.Println("[ERROR]", suberr)
			os.Exit(1)
		}

		var confirmation string
		skipinteractive, _ := cmd.Flags().GetBool("yes")
		if skipinteractive {
			confirmation = "y"
		} else {
			fmt.Printf("[WARNING] Do you really want to delete subnet %v?\n", subnet.String())
			fmt.Printf("[WARNING] This will also delete all DNS records if PowerDNS integration is enabled!\n")
			fmt.Printf("[WARNING] Continue? [y/N] ")
			fmt.Scan(&confirmation)
		}

		if (confirmation == "y") || (confirmation == "Y") {
			for _, address := range subnetobj.Addresses {
				if address.FQDN != "" {
					deleteerr := DeleteDNSFqdn(address.FQDN, address.IP)
					if deleteerr != nil {
						fmt.Println("[ERROR]", deleteerr)
					}
				}
			}
			deleteerr := DeleteSubnet(subnet)
			if deleteerr != nil {
				fmt.Println("[ERROR]", deleteerr)
				os.Exit(1)
			} else {
				fmt.Printf("deleted subnet %v\n", subnet.String())
			}

		}
	},
}

func init() {
	subnetCmd.AddCommand(subnetdeleteCmd)
	subnetdeleteCmd.Flags().BoolP("yes", "y", false, "suppress interactive prompts and answer yes.")
}
