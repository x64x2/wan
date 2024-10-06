package cmd

import (
	"fmt"
	"net/netip"
	"os"
	"os/user"
	"time"

	"github.com/spf13/cobra"
)

var ipaddCmd = &cobra.Command{
	Use:     "add ipaddress|subnet [hostname]",
	Short:   "Add new IP address",
	Long:    `Adds a new IP address OR the next free IP address from a subnet`,
	Aliases: []string{"a"},
	Args:    cobra.RangeArgs(1, 2),
	Run: func(cmd *cobra.Command, args []string) {
		var iparg, hostname string

		if len(args) == 1 {
			iparg = args[0]
			hostname = ""
		} else {
			iparg = args[0]
			hostname = args[1]
		}

		var bestsubnet Subnet
		var ip netip.Addr

		argip, ipparseerr := netip.ParseAddr(iparg)
		argsubnet, subparseerr := netip.ParsePrefix(iparg)

		if ipparseerr != nil && subparseerr != nil {
			fmt.Printf("[ERROR] Argument is neither a valid IP address nor a valid Subnet: %v", iparg)
			os.Exit(1)
		} else if ipparseerr == nil && subparseerr != nil {
			// argument was a single IP
			var subnetexists bool
			bestsubnet, subnetexists = FindBestSubnet(argip)
			if !subnetexists {
				fmt.Printf("[ERROR] Found no suitable subnet for IP %v\n", iparg)
				fmt.Printf("[ERROR] Maybe you need to add it first?\n")
				os.Exit(1)
			}
			if bestsubnet.HasIP(argip) {
				fmt.Printf("[ERROR] IP %v already exists in subnet %v\n", argip.String(), bestsubnet.Subnet.String())
				os.Exit(1)
			}
			ip = argip

		} else if subparseerr == nil && ipparseerr != nil {
			// argument was a subnet
			var subneterr error
			bestsubnet, subneterr = GetSubnet(argsubnet)
			if subneterr != nil {
				fmt.Println("[ERROR]", subneterr)
				os.Exit(1)
			}
			ip = bestsubnet.FindFirstFreeIP()

			if !ip.IsValid() {
				fmt.Printf("[ERROR] Found no free IP in Subnet %v\n", argsubnet.String())
				os.Exit(1)
			}

		}

		currentuser, _ := user.Current()
		timestamp := time.Now()

		bestsubnet.Addresses = append(bestsubnet.Addresses, Address{ip, hostname, timestamp, currentuser.Username})
		bestsubnet.ChangedBy = currentuser.Username
		bestsubnet.ChangedAt = timestamp

		writeerr := bestsubnet.WriteSubnet()
		if writeerr != nil {
			fmt.Println("[ERROR]", writeerr)
			os.Exit(1)
		}

		if hostname == "" {
			fmt.Printf("added ip:\nip: %v\n", ip.String())
		} else {
			fmt.Printf("added ip:\nip: %v\nhostname: %v\n", ip.String(), hostname)
			dnserr := AddDNSFqdn(hostname, ip)
			if dnserr != nil {
				fmt.Println("[ERROR]", dnserr)
				os.Exit(1)
			}
		}
	},
}

func init() {
	ipCmd.AddCommand(ipaddCmd)
}
