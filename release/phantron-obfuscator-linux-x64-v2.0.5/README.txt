═══════════════════════════════════════════════════════════════════════════
                    PHANTRON OBFUSCATOR v2.0.5
                Professional Code Protection & Obfuscation Tool
═══════════════════════════════════════════════════════════════════════════

PACKAGE CONTENTS
────────────────────────────────────────────────────────────────────────────

This package contains standalone executables for multiple platforms:

Windows:
  • phantron-win-x64.exe       - Windows x64 (Intel/AMD 64-bit)
  • phantron-win-arm64.exe     - Windows ARM64

Linux:
  • phantron-linux-x64         - Linux x64 (Intel/AMD 64-bit) 
  • phantron-linux-arm64       - Linux ARM64 (Apple Silicon, Raspberry Pi, etc.)

Installation Scripts:
  • install.bat                - Windows automated installer
  • install.sh                 - Linux automated installer

Configuration:
  • config/                    - Obfuscation profiles (balanced, security, size)

QUICK START
────────────────────────────────────────────────────────────────────────────

Windows:
  1. Choose the appropriate executable for your system (usually win-x64)
  2. Copy install.bat to the same folder
  3. Right-click install.bat and select "Run as Administrator"
  4. Restart your terminal
  5. Run: phantron --version

Linux:
  1. Choose the appropriate executable for your system (usually linux-x64)
  2. Make executable: chmod +x phantron-linux-x64 install.sh
  3. Run installer: sudo ./install.sh
  4. Run: phantron --version

MANUAL INSTALLATION
────────────────────────────────────────────────────────────────────────────

Windows (PowerShell as Administrator):
  $exe = "phantron-win-x64.exe"  # or phantron-win-arm64.exe
  Copy-Item $exe "$env:ProgramFiles\Phantron\phantron.exe"
  [Environment]::SetEnvironmentVariable("PATH", "$env:PATH;$env:ProgramFiles\Phantron", "Machine")

Linux:
  sudo cp phantron-linux-x64 /usr/local/bin/phantron  # or phantron-linux-arm64
  sudo chmod +x /usr/local/bin/phantron

USAGE EXAMPLES
────────────────────────────────────────────────────────────────────────────

Basic obfuscation:
  phantron obfuscate input.c -o output.c

With specific mode:
  phantron obfuscate input.c -o output.c --mode security

Interactive mode (guided setup):
  phantron interactive

Auto-tune (find optimal settings):
  phantron auto-tune input.c

Batch processing:
  phantron batch src/*.c -o obfuscated/

Custom config file:
  phantron obfuscate input.c -o output.c -c custom-config.yaml

Full help:
  phantron --help

FEATURES
────────────────────────────────────────────────────────────────────────────

✓ 14+ Advanced Obfuscation Techniques:
  • Control Flow Flattening
  • Bogus Control Flow
  • Opaque Predicates (Classical & Quantum-Inspired)
  • Instruction Substitution
  • Dead Code Injection
  • String & Constant Obfuscation
  • Mixed Boolean-Arithmetic (MBA)
  • Function Virtualization
  • Call Graph Obfuscation
  • Anti-Debugging Techniques
  • Hardware Cache Obfuscation
  • Grammar-Based Metamorphic Transformations

✓ Advanced Systems:
  • MAOS: Multi-Layered Adaptive Obfuscation System
  • QIRL: Quantum-Inspired Resistance Layer
  • ATIE: AI-Tuned Intelligent Engine
  • PCGE: Polymorphic Code Generation Engine

✓ Professional Features:
  • Auto-tuning for optimal protection
  • Comprehensive HTML reporting with metrics
  • Multi-file batch processing
  • Custom configuration support
  • Cross-platform compatibility

SYSTEM REQUIREMENTS
────────────────────────────────────────────────────────────────────────────

Minimum:
  • 2GB RAM
  • 500MB disk space
  • Windows 10/11 or Linux (kernel 4.x+)

Recommended:
  • 4GB+ RAM
  • 1GB disk space
  • SSD storage

For full obfuscation features, the Phantron LLVM Obfuscator binary is required.
This standalone CLI can work independently but with limited transformations.

CONFIGURATION FILES
────────────────────────────────────────────────────────────────────────────

Built-in profiles (in config/ directory):

balanced.yaml:
  - Balanced protection and performance
  - Good for most use cases
  - Moderate obfuscation

maximum_security.yaml:
  - Maximum protection
  - Highest obfuscation level
  - May impact performance

size_conservative.yaml:
  - Minimal size increase
  - Faster compilation
  - Basic protection

TROUBLESHOOTING
────────────────────────────────────────────────────────────────────────────

"Command not found" after installation:
  Windows: Restart terminal, verify PATH includes installation directory
  Linux: Run `which phantron` to verify installation

"Permission denied":
  Windows: Run as Administrator
  Linux: Run with sudo or chmod +x the executable

"Missing LLVM binary":
  The CLI will work in standalone mode with limited features.
  For full features, ensure phantron-llvm-obfuscator is in the same directory.

Antivirus warnings:
  Standalone executables may trigger false positives.
  Add to antivirus exceptions if needed.

ARCHITECTURE-SPECIFIC NOTES
────────────────────────────────────────────────────────────────────────────

x64 (AMD64/Intel 64-bit):
  - Most common architecture
  - Full feature support
  - Use win-x64 or linux-x64

ARM64 (Apple Silicon, Raspberry Pi):
  - Modern ARM-based systems
  - Use win-arm64 or linux-arm64
  - Note: Limited LLVM support on some ARM platforms

SUPPORT & DOCUMENTATION
────────────────────────────────────────────────────────────────────────────

GitHub Repository:
  https://github.com/NeuralNinja110/SIH25_2

NPM Package:
  https://www.npmjs.com/package/phantron-obfuscator
  Install via: npm install -g phantron-obfuscator

Report Issues:
  https://github.com/NeuralNinja110/SIH25_2/issues

Documentation:
  See INSTALLATION.md and README.md in the GitHub repository

TEAM
────────────────────────────────────────────────────────────────────────────

Developed by Team ELEVATE_1:
  • Rahul Ashok
  • Pritham Devaprasad
  • Anish R
  • Siddarth S
  • Pranav Harish
  • Yashaswi Karthik

LICENSE
────────────────────────────────────────────────────────────────────────────

MIT License
Copyright (c) 2025 Team ELEVATE_1

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

═══════════════════════════════════════════════════════════════════════════
          Built with ❤️ by Team ELEVATE_1 | Version 2.0.5
═══════════════════════════════════════════════════════════════════════════
