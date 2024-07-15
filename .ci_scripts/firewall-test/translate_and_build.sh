#!/bin/bash

## CONSTANTS
CI_SCRIPTS_DIR="$GITHUB_WORKSPACE/.ci_scripts/firewall-test"


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

# 2. Build project with CMake
$GITHUB_WORKSPACE/build.sh -d $GITHUB_WORKSPACE

# 3. Run CUnit tests
$CI_SCRIPTS_DIR/run_tests.sh

# 4. Run Valgrind on CUnit tests
$CI_SCRIPTS_DIR/run_tests.sh valgrind

# 5. Run cppcheck on source files
$CI_SCRIPTS_DIR/run_cppcheck.sh

# 6. Add nftables rules
$CI_SCRIPTS_DIR/add_nft_rules.sh
