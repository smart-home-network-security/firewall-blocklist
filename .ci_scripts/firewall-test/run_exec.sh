#!/bin/bash

# Constants
TIMEOUT=5  # seconds
BIN_DIR="$GITHUB_WORKSPACE/bin"

# Ensure globbing expands to an empty list if no matches are found
shopt -s nullglob

# Execute all NFQueue executables
for EXEC in "$BIN_DIR"/*
do
    if [ -f "$EXEC" ]
    then
        sudo $EXEC & sleep $TIMEOUT
        sudo kill $!
    fi
done
