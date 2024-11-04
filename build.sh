#!/bin/sh

# Build project executable
mkdir -p build
pushd build
cmake .. -G Ninja
ninja
popd