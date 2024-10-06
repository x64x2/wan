package cmd

import (
	"github.com/spf13/cobra"
)

// subnetCmd represents the subnet command
var subnetCmd = &cobra.Command{
	Use:     "subnet",
	Short:   "Manage IP subnets",
	Long:    `Add, delete and show IP subnets`,
	Aliases: []string{"s"},
}

func init() {
	rootCmd.AddCommand(subnetCmd)
}