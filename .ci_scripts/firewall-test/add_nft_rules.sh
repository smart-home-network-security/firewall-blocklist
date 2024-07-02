EXITCODE=0

for nft_script in $GITHUB_WORKSPACE/test/translator/devices/*/firewall.nft
do
    # Flush the ruleset before next device
    sudo nft flush ruleset
    # Try adding the ruleset
    sudo nft -f "$nft_script"
    # If the exit code is not 0, set EXITCODE to 1
    if [[ $? -ne 0 ]]
    then
        EXITCODE=1
    fi
done

exit $EXITCODE
