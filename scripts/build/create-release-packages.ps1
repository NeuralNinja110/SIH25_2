# Script to create distribution packages for GitHub Release

Write-Host "`n════════════════════════════════════════════════════════════════════════" -ForegroundColor Cyan
Write-Host "        Creating Distribution Packages for GitHub Release" -ForegroundColor Cyan
Write-Host "════════════════════════════════════════════════════════════════════════`n" -ForegroundColor Cyan

$DIST_DIR = "dist"
$RELEASE_DIR = "release"

# Create release directory
New-Item -ItemType Directory -Force -Path $RELEASE_DIR | Out-Null

# Function to create Windows package
function New-WindowsPackage {
    param([string]$Arch)
    
    $pkgName = "phantron-obfuscator-windows-$Arch-v2.0.5"
    $pkgDir = "$RELEASE_DIR\$pkgName"
    
    Write-Host "[INFO] Creating Windows $Arch package..." -ForegroundColor Blue
    
    # Create package directory
    New-Item -ItemType Directory -Force -Path $pkgDir | Out-Null
    
    # Copy files
    if ($Arch -eq "x64") {
        Copy-Item "$DIST_DIR\phantron-win-x64.exe" "$pkgDir\phantron.exe"
    } else {
        Copy-Item "$DIST_DIR\phantron-win-arm64.exe" "$pkgDir\phantron.exe"
    }
    
    Copy-Item "$DIST_DIR\phantron-llvm-obfuscator" "$pkgDir\phantron-llvm-obfuscator" -ErrorAction SilentlyContinue
    Copy-Item "$DIST_DIR\install.bat" "$pkgDir\"
    Copy-Item "$DIST_DIR\README.txt" "$pkgDir\"
    Copy-Item -Recurse "$DIST_DIR\config" "$pkgDir\"
    Copy-Item "LICENSE" "$pkgDir\" -ErrorAction SilentlyContinue
    
    # Create ZIP
    $zipPath = "$RELEASE_DIR\$pkgName.zip"
    Compress-Archive -Path $pkgDir -DestinationPath $zipPath -Force
    
    $size = [math]::Round((Get-Item $zipPath).Length / 1MB, 2)
    Write-Host "[SUCCESS] Created: $pkgName.zip ($size MB)" -ForegroundColor Green
    
    # Cleanup temp directory
    Remove-Item -Recurse -Force $pkgDir
}

# Function to create Linux package
function New-LinuxPackage {
    param([string]$Arch)
    
    $pkgName = "phantron-obfuscator-linux-$Arch-v2.0.5"
    $pkgDir = "$RELEASE_DIR\$pkgName"
    
    Write-Host "[INFO] Creating Linux $Arch package..." -ForegroundColor Blue
    
    # Create package directory
    New-Item -ItemType Directory -Force -Path $pkgDir | Out-Null
    
    # Copy files
    if ($Arch -eq "x64") {
        Copy-Item "$DIST_DIR\phantron-linux-x64" "$pkgDir\phantron"
    } else {
        Copy-Item "$DIST_DIR\phantron-linux-arm64" "$pkgDir\phantron"
    }
    
    Copy-Item "$DIST_DIR\phantron-llvm-obfuscator" "$pkgDir\"
    Copy-Item "$DIST_DIR\install.sh" "$pkgDir\"
    Copy-Item "$DIST_DIR\README.txt" "$pkgDir\"
    Copy-Item -Recurse "$DIST_DIR\config" "$pkgDir\"
    Copy-Item "LICENSE" "$pkgDir\" -ErrorAction SilentlyContinue
    
    # Create tar.gz using WSL
    Write-Host "[INFO] Creating tar.gz archive..." -ForegroundColor Blue
    wsl -d Ubuntu-24.04 -- bash -c "cd /mnt/c/Users/abcra/OneDrive/Desktop/Phantron/SIH25_2/release && tar -czf $pkgName.tar.gz $pkgName && rm -rf $pkgName"
    
    if (Test-Path "$RELEASE_DIR\$pkgName.tar.gz") {
        $size = [math]::Round((Get-Item "$RELEASE_DIR\$pkgName.tar.gz").Length / 1MB, 2)
        Write-Host "[SUCCESS] Created: $pkgName.tar.gz ($size MB)" -ForegroundColor Green
    }
}

# Create packages
New-WindowsPackage "x64"
New-WindowsPackage "arm64"
New-LinuxPackage "x64"
New-LinuxPackage "arm64"

# Generate SHA256 checksums
Write-Host "`n[INFO] Generating checksums..." -ForegroundColor Blue
$checksums = @()
Get-ChildItem "$RELEASE_DIR\*" -Include *.zip,*.tar.gz | ForEach-Object {
    $hash = Get-FileHash $_.FullName -Algorithm SHA256
    $checksums += "$($hash.Hash)  $($_.Name)"
}

if ($checksums.Count -gt 0) {
    $checksums | Out-File "$RELEASE_DIR\SHA256SUMS.txt" -Encoding UTF8
    Write-Host "[SUCCESS] Checksums generated: SHA256SUMS.txt" -ForegroundColor Green
}

# Summary
Write-Host "`n════════════════════════════════════════════════════════════════════════" -ForegroundColor Green
Write-Host "                    PACKAGES READY FOR RELEASE!" -ForegroundColor Green
Write-Host "════════════════════════════════════════════════════════════════════════`n" -ForegroundColor Green

Write-Host "Release packages created in: release/`n" -ForegroundColor Cyan
Get-ChildItem "$RELEASE_DIR\*" -Include *.zip,*.tar.gz,*.txt | ForEach-Object {
    $size = [math]::Round($_.Length / 1MB, 2)
    Write-Host "  [OK] $($_.Name) - $size MB" -ForegroundColor White
}

Write-Host "`nNext steps:" -ForegroundColor Yellow
Write-Host "  1. Review the packages in the release/ directory" -ForegroundColor White
Write-Host "  2. Create a GitHub release tag v2.0.5" -ForegroundColor White
Write-Host "  3. Upload all files from release/ directory" -ForegroundColor White
Write-Host "  4. Or use GitHub CLI to create release" -ForegroundColor White
Write-Host "`n" -ForegroundColor Green
