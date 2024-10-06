package cmd

import (
	"github.com/spf13/cobra"
)

// ipCmd represents the ip command
var ipCmd = &cobra.Command{
	Use:     "ip",
	Short:   "manage IP addresses",
	Long:    `Add, delete and show IP addresses`,
	Aliases: []string{"i"},
}

func init() {
	rootCmd.AddCommand(ipCmd)
}
