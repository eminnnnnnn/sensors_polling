#!/bin/bash

echo "=== Preparing stage ==="
./prepare.sh

echo "=== Build stage ==="
./build.sh

echo "=== Install stage ==="
./install.sh