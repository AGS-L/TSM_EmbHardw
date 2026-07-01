#!/bin/bash

# Start a subshell
(
# Exit immediately on error
set -Eeuo pipefail

# Make sure the current working directory is where the script is located
SCRIPT_DIR=$(cd -- "$( dirname -- "$BASH_SOURCE[0]" )" &> /dev/null && pwd)
cd "$SCRIPT_DIR"

# Clean-up previous run
rm or1300SingleCore.*

# Synthesize
yosys -D GECKO5Education -s yosysOr1300.script

# Implement (place & route)
nextpnr-ecp5 --threads $(nproc) --timing-allow-fail --85k --package CABGA381 --json or1300SingleCore.json --lpf gecko5.lpf --textcfg or1300SingleCore.config

# Bitstream generation
ecppack --compress --freq 62.0 --input or1300SingleCore.config --bit ../../bit/or1300SingleCore.bit

# Bitstream download
openFPGALoader ../../bit/or1300SingleCore.bit
)
