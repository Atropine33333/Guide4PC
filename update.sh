#!/usr/bin/env bash
# update.sh — Guide4PC build wrapper
# Run this script to build the dist/ directory for deployment.
#
# Usage:
#   chmod +x update.sh
#   ./update.sh

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

echo "==> Running build script..."
node js/build.js

echo ""
echo "==> Done! Deploy the dist/ directory to GitHub Pages."
