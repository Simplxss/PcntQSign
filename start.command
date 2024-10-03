#!/bin/bash
SCRIPT_DIR=$(realpath $(dirname "${BASH_SOURCE[0]}"))
export ELECTRON_RUN_AS_NODE=1
DYLD_INSERT_LIBRARIES=./libSignerServer.dylib /Applications/QQ.app/Contents/MacOS/qq ${SCRIPT_DIR}/load.js $@
