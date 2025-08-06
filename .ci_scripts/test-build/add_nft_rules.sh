# CONSTANTS
EXITCODE=0

# Flush the ruleset before next device
sudo nft flush ruleset
# Try adding the ruleset
sudo nft -f test/device/firewall.nft
# If the exit code is not 0, set EXITCODE to 1
if [[ $? -ne 0 ]]
then
    EXITCODE=1
fi

sudo nft flush ruleset

exit $EXITCODE