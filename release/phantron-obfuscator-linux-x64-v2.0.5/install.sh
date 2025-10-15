#!/bin/bash
# Phantron Obfuscator Installation Script for Linux
# Run with: sudo ./install.sh

set -e

INSTALL_DIR="/usr/local/bin"
CONFIG_DIR="/usr/local/share/phantron"

echo "============================================================================"
echo "                    PHANTRON OBFUSCATOR INSTALLER"
echo "============================================================================"
echo ""

# Check for root privileges
if [ "$EUID" -ne 0 ]; then 
    echo "[ERROR] Please run as root: sudo ./install.sh"
    exit 1
fi

echo "[INFO] Installing to: $INSTALL_DIR"
echo ""

# Copy binaries
echo "[INFO] Copying binaries..."
cp phantron "$INSTALL_DIR/phantron"
if [ -f "phantron-llvm-obfuscator" ]; then
    cp phantron-llvm-obfuscator "$INSTALL_DIR/phantron-llvm-obfuscator"
    chmod +x "$INSTALL_DIR/phantron-llvm-obfuscator"
fi

# Make executable
chmod +x "$INSTALL_DIR/phantron"

# Copy config if exists
if [ -d "config" ]; then
    echo "[INFO] Copying configuration files..."
    mkdir -p "$CONFIG_DIR"
    cp -r config/* "$CONFIG_DIR/"
fi

echo ""
echo "============================================================================"
echo "[SUCCESS] Installation Complete!"
echo "============================================================================"
echo ""
echo "Installed to: $INSTALL_DIR"
echo ""
echo "Next steps:"
echo "  1. Run: phantron --version"
echo "  2. Run: phantron --help for usage information"
echo ""
echo "For support: https://github.com/NeuralNinja110/SIH25_2"
echo ""
