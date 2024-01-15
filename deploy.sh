#!/bin/bash

echo "=== Preparing stage ==="
./prepare.sh
echo "Succeeded"

echo "=== Build stage ==="
./build.sh
echo "Succeeded"

echo "=== Install stage ==="
./install.sh
echo "Succeeded"