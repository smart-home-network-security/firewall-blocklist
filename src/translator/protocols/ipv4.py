from ip import ip

class ipv4(ip):

    # Class variables
    protocol_name = "ipv4"  # Protocol name
    nft_prefix = "ip"       # Prefix for nftables rules

    # Well-known addresses
    addrs = ip.addrs["ipv4"]
