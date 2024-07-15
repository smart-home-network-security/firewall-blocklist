#!/bin/bash

## CONSTANTS
CI_SCRIPTS_DIR="$GITHUB_WORKSPACE/.ci_scripts/test-firewall"


## COMMAND LINE ARGUMENTS
MODE=""
ARG=""
while getopts "r:p:" opt;
do
    case "${opt}" in
        r|p)
            # Rate limit / stochastic verdict
            MODE="${opt}"
            ARG="${OPTARG}"
            ;;
        *)
            # Default: binary verdict (ACCEPT or DROP)
            ;;
    esac
done
shift $((OPTIND-1))


########
# MAIN #
########

# 1. Translate profiles with given verdict mode
$CI_SCRIPTS_DIR/translate_profiles.sh $MODE $ARG

# 2. Cross-compile project with CMake
$GITHUB_WORKSPACE/build.sh -d $GITHUB_WORKSPACE -t $GITHUB_WORKSPACE/openwrt/tl-wdr4900.cmake
