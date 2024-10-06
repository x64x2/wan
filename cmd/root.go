package cmd

import (
	"fmt"
	"os"

	"github.com/spf13/cobra"
	"github.com/spf13/viper"
)

var rootCmd = &cobra.Command{
	Use:   "ipam",
	Short: "A cli based ipam",
	Long: `A cli based ipam. 
You can manage subnets and single IP addresses within those subnets.
ipam can also manage the corresponding DNS records in your PowerDNS Instance.`,
	Version: ipamVersion,
}

// Execute adds all child commands to the root command and sets flags appropriately.
func Execute() {
	err := rootCmd.Execute()
	if err != nil {
		os.Exit(1)
	}
}

func init() {
	cobra.OnInitialize(initConfig)

	rootCmd.Flags().BoolP("debug", "d", false, "Enable debug mode. (may print sensitive Information, so please watch out!)")
}

func initConfig() {
	// Find home directory.
	homedir, err := os.UserHomeDir()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	var ipamdir = homedir + "/.ipam/"
	// Search config in home directory with name ".cobra" (without extension).
	viper.AddConfigPath(ipamdir)
	viper.SetConfigName("ipam")
	viper.SetConfigType("yaml")

	viper.SetDefault("DataPath", ipamdir+"data/")
	viper.SetDefault("PowerDNSEnabled", false)
	viper.SetDefault("PowerDNSEndpoint", "")
	viper.SetDefault("PowerDNSApiKey", "")

	if err := viper.ReadInConfig(); err != nil {
		_, patherr := os.Stat(ipamdir)
		if patherr != nil {
			mkerr := os.MkdirAll(ipamdir, 0755)
			if mkerr != nil {
				println("[ERROR] Can't create ipam config directory!", mkerr)
			}
		}

		// I have no idea what's happening here...
		if _, ok := err.(viper.ConfigFileNotFoundError); ok {
			writeerr := viper.SafeWriteConfig()
			if writeerr != nil {
				println("[ERROR] Can't write config file!", writeerr)
			}
		} else {
			println("[ERROR] Can't read config file!", err)
		}
	}
}
