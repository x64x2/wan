# ipam
A cli based [IP Address Management](https://en.wikipedia.org/wiki/IP_address_management)
built with [Go](https://go.dev/) using [Cobra](https://cobra.dev/) and
[Viper](https://github.com/spf13/viper) with optional [PowerDNS](https://www.powerdns.com/auth.html)
API integration.

## Features
`ipam` is a cli based IPAM. It supports:
- Adding, listing, viewing and deleting subnets
- Adding, viewing and deleting single IP addresses with FQDNs
- automatic addition and deletion of forward and reverse records
- import and export configuration

## How do I use this?
`ipam` is built as a cli app. Once you installed `ipam` and added it to your `$PATH`, you can
- add prefixes with `ipam subnet add 192.168.0.0/24 fancy-subnet-name`
- add IPs to those prefixes with `ipam ip add 192.168.0.1 fqdn.example.com`
- add the next free IP in a prefix with `ipam ip add 192.168.0.0/24 fqdn2.example.com`
- much more (see `ipam (command) --help` for examples)


If PowerDNS integration is enabled in `$HOME/.ipam/ipam.yml`, forward and reverse DNS records
are automatically managed when you add or delete IP addresses with hostnames.

[![Asciicast displaying the usage of the ipam app](docs/static/ipam-asciicast.svg)](https://asciinema.org/a/tT3SbtjsN9bmEE4QgKuanhNcX)

## CLI
```
❯ ipam
A cli based ipam.
You can manage subnets and single IP addresses within those subnets.
ipam can also manage the corresponding DNS records in your PowerDNS Instance.

Usage:
ipam [command]

Available Commands:
completion  Generate the autocompletion script for the specified shell
export      Export ipam configuration
help        Help about any command
import      Import ipam configuration
ip          manage IP addresses
subnet      Manage IP subnets

Flags:
-d, --debug     Enable debug mode. (may print sensitive Information, so please watch out!)
-h, --help      help for ipam
-v, --version   version for ipam

Use "ipam [command] --help" for more information about a command.
```