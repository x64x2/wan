package cmd

import (
	"fmt"

	"github.com/spf13/cobra"
)

var ipeditCmd = &cobra.Command{
	Use:     "edit",
	Short:   "Edit an IP address",
	Long:    `Edit an IP address`,
	Aliases: []string{"e"},
	//Args:    cobra.ExactArgs(1),
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("not implemented yet; please delete and readd")
	},
}

func init() {
	ipCmd.AddCommand(ipeditCmd)
}