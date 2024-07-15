#!/bin/bash

## CONSTANTS
DEVICES_DIR="$GITHUB_WORKSPACE/test/translator/devices"
TRANSLATOR_PATH="$GITHUB_WORKSPACE/src/translator/translator.py"

# Ensure globbing expands to an empty list if no matches are found
shopt -s nullglob

# Loop over devices
NFQ_BASE_ID=0
for DEVICE in "$DEVICES_DIR"/*/; do
    # Call translator over device profile
    # Arguments $1 & $2 represent the verdict mode
    python3 $TRANSLATOR_PATH "$DEVICE"profile.yaml $NFQ_BASE_ID $1 $2
    ((NFQ_BASE_ID=NFQ_BASE_ID+100))
done
