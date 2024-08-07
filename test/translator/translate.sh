#!/bin/bash

## CONSTANTS
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )  # This script's path
DEVICES_DIR="$SCRIPT_DIR/devices"
TRANSLATOR_PATH="$SCRIPT_DIR/../../src/translator/translator.py"

# Ensure globbing expands to an empty list if no matches are found
shopt -s nullglob

# Loop over devices
NFQ_BASE_ID=0
for DEVICE in "$DEVICES_DIR"/*/; do
    python3 "$TRANSLATOR_PATH" "$DEVICE"profile.yaml -q $NFQ_BASE_ID
    ((NFQ_BASE_ID=NFQ_BASE_ID+100))
done
