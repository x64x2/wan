package cmd

import (
	"encoding/json"
	"fmt"
	"os"
	"path"
	"time"

	"github.com/spf13/cobra"
)

var importCmd = &cobra.Command{
	Use:     "import",
	Short:   "Import ipam configuration",
	Long:    `Import subnets to ipam.`,
	Example: "ipam import --file import.json",
	Args:    cobra.NoArgs,
	Aliases: []string{"im"},
	Run: func(cmd *cobra.Command, args []string) {
		var importname string
		var subnets []Subnet

		flagpath, _ := cmd.Flags().GetString("file")
		if path.IsAbs(flagpath) {
			importname = flagpath
		} else {
			wd, _ := os.Getwd()
			importname = path.Join(wd, flagpath)
		}

		file, readerr := os.ReadFile(importname)
		if readerr != nil {
			fmt.Printf("[ERROR] Can't read file %v\n", importname)
			fmt.Println(readerr)
		}

		marsherr := json.Unmarshal(file, &subnets)
		if marsherr != nil {
			fmt.Printf("[ERROR] Invalid format for file %v\n", importname)
			fmt.Println(marsherr)
		}

		for _, subnet := range subnets {
			fmt.Printf("[INFO] Start import of %v\n", subnet.Subnet.String())
			subnet.ChangedBy = "ipam import"
			subnet.ChangedAt = time.Now()

			for _, addr := range subnet.Addresses {
				addr.ChangedBy = "ipam import"
				addr.ChangedAt = time.Now()
				if addr.FQDN != "" {
					err := AddDNSFqdn(addr.FQDN, addr.IP)
					if err != nil {
						fmt.Printf("[ERROR] %v\n", err)
					}
				}
			}

			suberr := subnet.WriteSubnet()
			if suberr != nil {
				fmt.Printf("[ERROR] Can't write subnet to file %v\n", subnet.Subnet.String())
				fmt.Println(suberr)
			}
			fmt.Printf("[INFO] Imported subnet %v successfully\n", subnet.Subnet.String())

		}
	},
}

func init() {
	rootCmd.AddCommand(importCmd)

	importCmd.Flags().StringP("file", "f", "import.json", "File to use for import operation")
}
