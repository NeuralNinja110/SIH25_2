# ============================================================================
# MAOS - Windows Test Suite (PowerShell)
# ============================================================================
# Comprehensive test suite with progress tracking and detailed reports
# ============================================================================

[CmdletBinding()]
param(
    [Parameter(Mandatory=$false)]
    [switch]$Verbose,
    
    [Parameter(Mandatory=$false)]
    [switch]$SkipBaseline,
    
    [Parameter(Mandatory=$false)]
    [switch]$SkipSizeConservative,
    
    [Parameter(Mandatory=$false)]
    [switch]$SkipMaximumSecurity
)

$ErrorActionPreference = "Continue"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  MAOS Comprehensive Test Suite" -ForegroundColor Cyan
Write-Host "  PowerShell Edition" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Find obfuscator
$obfuscatorPaths = @(
    "build\Release\llvm-obfuscator.exe",
    "build\Debug\llvm-obfuscator.exe",
    "build\llvm-obfuscator.exe"
)

$obfuscator = $null
foreach ($path in $obfuscatorPaths) {
    if (Test-Path $path) {
        $obfuscator = $path
        break
    }
}

if (-not $obfuscator) {
    Write-Host "ERROR: Obfuscator executable not found!" -ForegroundColor Red
    Write-Host "Please run build_windows.ps1 first." -ForegroundColor Yellow
    exit 1
}

Write-Host "✓ Obfuscator: $obfuscator" -ForegroundColor Green

# Configuration
$testDir = "tests"
$resultsDir = "test_results_windows"
$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"

# Create results structure
$directories = @(
    $resultsDir,
    "$resultsDir\logs",
    "$resultsDir\binaries",
    "$resultsDir\reports"
)

foreach ($dir in $directories) {
    if (-not (Test-Path $dir)) {
        New-Item -ItemType Directory -Path $dir | Out-Null
    }
}

Write-Host "✓ Results directory: $resultsDir" -ForegroundColor Green
Write-Host ""

# Test cases
$testCases = @(
    "test_easy.c",
    "test_medium.c",
    "test_difficult.c",
    "test_variadic_functions.c",
    "test_function_pointers.c",
    "test_data_structures.c",
    "test_floating_point.c",
    "test_bit_manipulation.c"
)

# Counters
$script:totalTests = 0
$script:passedTests = 0
$script:failedTests = 0

# Results array
$script:results = @()

# Function to run test
function Test-Obfuscation {
    param(
        [string]$TestFile,
        [string]$Mode,
        [string]$ModeConfig
    )
    
    $script:totalTests++
    
    $inputPath = Join-Path $testDir $TestFile
    if (-not (Test-Path $inputPath)) {
        Write-Host "  [SKIP] File not found: $TestFile" -ForegroundColor Yellow
        return
    }
    
    $baseName = [System.IO.Path]::GetFileNameWithoutExtension($TestFile)
    $outputPath = Join-Path $resultsDir "binaries\${baseName}_${Mode}.exe"
    $logPath = Join-Path $resultsDir "logs\${baseName}_${Mode}.log"
    
    Write-Host "Testing: $TestFile [$Mode Mode]" -ForegroundColor Cyan
    
    # Get original size
    $originalSize = (Get-Item $inputPath).Length
    
    # Build command
    $args = @("-i", $inputPath, "-o", $outputPath)
    $args += $ModeConfig.Split(" ")
    
    if ($Verbose) {
        Write-Host "  Command: $obfuscator $($args -join ' ')" -ForegroundColor Gray
    }
    
    # Run obfuscation
    $startTime = Get-Date
    try {
        $output = & $obfuscator @args 2>&1 | Tee-Object -FilePath $logPath
        $exitCode = $LASTEXITCODE
    } catch {
        $exitCode = 1
        $output = $_.Exception.Message
        $output | Out-File -FilePath $logPath
    }
    $endTime = Get-Date
    $duration = ($endTime - $startTime).TotalMilliseconds
    
    # Check result
    if ($exitCode -eq 0 -and (Test-Path $outputPath)) {
        $obfuscatedSize = (Get-Item $outputPath).Length
        $sizeIncrease = [math]::Round((($obfuscatedSize - $originalSize) / $originalSize) * 100, 2)
        
        Write-Host "  [PASS] Binary created successfully" -ForegroundColor Green
        Write-Host "  Original: $originalSize bytes" -ForegroundColor White
        Write-Host "  Obfuscated: $obfuscatedSize bytes" -ForegroundColor White
        Write-Host "  Size increase: $sizeIncrease%" -ForegroundColor White
        Write-Host "  Duration: $([math]::Round($duration, 2)) ms" -ForegroundColor White
        
        $script:passedTests++
        
        # Store result
        $script:results += [PSCustomObject]@{
            TestCase = $TestFile
            Mode = $Mode
            Status = "PASS"
            OriginalSize = $originalSize
            ObfuscatedSize = $obfuscatedSize
            SizeIncrease = $sizeIncrease
            Duration = $duration
            OutputPath = $outputPath
        }
    } else {
        Write-Host "  [FAIL] Obfuscation failed" -ForegroundColor Red
        if ($Verbose) {
            Write-Host "  Exit code: $exitCode" -ForegroundColor Red
            Write-Host "  See log: $logPath" -ForegroundColor Yellow
        }
        
        $script:failedTests++
        
        $script:results += [PSCustomObject]@{
            TestCase = $TestFile
            Mode = $Mode
            Status = "FAIL"
            OriginalSize = $originalSize
            ObfuscatedSize = 0
            SizeIncrease = 0
            Duration = $duration
            OutputPath = ""
        }
    }
    Write-Host ""
}

# MODE 1: Normal Obfuscation
if (-not $SkipBaseline) {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "  MODE 1: Normal Obfuscation" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host ""
    
    foreach ($test in $testCases) {
        Test-Obfuscation -TestFile $test -Mode "normal" -ModeConfig "-l 2 -C 2"
    }
}

# MODE 2: Size-Conservative
if (-not $SkipSizeConservative) {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "  MODE 2: Size-Conservative" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host ""
    
    foreach ($test in $testCases) {
        Test-Obfuscation -TestFile $test -Mode "size_conservative" -ModeConfig "-c config\size_conservative.yaml"
    }
}

# MODE 3: Maximum-Security
if (-not $SkipMaximumSecurity) {
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "  MODE 3: Maximum-Security" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host ""
    
    foreach ($test in $testCases) {
        Test-Obfuscation -TestFile $test -Mode "maximum_security" -ModeConfig "-c config\maximum_security.yaml"
    }
}

# Generate summary
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  TEST SUMMARY" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Total Tests: $script:totalTests" -ForegroundColor White
Write-Host "Passed: $script:passedTests" -ForegroundColor Green
Write-Host "Failed: $script:failedTests" -ForegroundColor $(if ($script:failedTests -eq 0) { "Green" } else { "Red" })
Write-Host ""

if ($script:failedTests -eq 0) {
    Write-Host "[SUCCESS] All tests passed!" -ForegroundColor Green
} else {
    Write-Host "[WARNING] Some tests failed. Check logs in $resultsDir\logs" -ForegroundColor Yellow
}
Write-Host ""

# Export results to CSV
$csvPath = Join-Path $resultsDir "reports\test_results_$timestamp.csv"
$script:results | Export-Csv -Path $csvPath -NoTypeInformation
Write-Host "✓ Results exported to: $csvPath" -ForegroundColor Green

# Generate text report
$reportPath = Join-Path $resultsDir "reports\test_summary_$timestamp.txt"
$report = @"
MAOS Test Suite Summary
=======================

Date: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
Obfuscator: $obfuscator
Results Directory: $resultsDir

Test Statistics:
----------------
Total Tests: $script:totalTests
Passed: $script:passedTests
Failed: $script:failedTests
Success Rate: $([math]::Round(($script:passedTests / $script:totalTests) * 100, 2))%

Detailed Results:
-----------------

"@

foreach ($result in $script:results) {
    $report += @"
Test: $($result.TestCase) [$($result.Mode)]
  Status: $($result.Status)
  Original Size: $($result.OriginalSize) bytes
  Obfuscated Size: $($result.ObfuscatedSize) bytes
  Size Increase: $($result.SizeIncrease)%
  Duration: $([math]::Round($result.Duration, 2)) ms

"@
}

$report | Out-File -FilePath $reportPath -Encoding UTF8
Write-Host "✓ Summary report saved to: $reportPath" -ForegroundColor Green

# Generate HTML report
$htmlPath = Join-Path $resultsDir "reports\test_results_$timestamp.html"
$html = @"
<!DOCTYPE html>
<html>
<head>
    <title>MAOS Test Results</title>
    <style>
        body { font-family: 'Segoe UI', Arial, sans-serif; margin: 20px; background: #f5f5f5; }
        .header { background: #0078d4; color: white; padding: 20px; border-radius: 8px; }
        .summary { background: white; padding: 20px; margin: 20px 0; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }
        .stats { display: flex; gap: 20px; margin: 20px 0; }
        .stat { flex: 1; background: white; padding: 20px; border-radius: 8px; text-align: center; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }
        .stat-value { font-size: 36px; font-weight: bold; margin: 10px 0; }
        .pass { color: #107c10; }
        .fail { color: #d13438; }
        table { width: 100%; border-collapse: collapse; background: white; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }
        th { background: #0078d4; color: white; padding: 12px; text-align: left; }
        td { padding: 12px; border-bottom: 1px solid #ddd; }
        tr:hover { background: #f5f5f5; }
    </style>
</head>
<body>
    <div class="header">
        <h1>MAOS Test Results</h1>
        <p>Generated: $(Get-Date -Format "yyyy-MM-dd HH:mm:ss")</p>
    </div>
    
    <div class="stats">
        <div class="stat">
            <div>Total Tests</div>
            <div class="stat-value">$script:totalTests</div>
        </div>
        <div class="stat">
            <div>Passed</div>
            <div class="stat-value pass">$script:passedTests</div>
        </div>
        <div class="stat">
            <div>Failed</div>
            <div class="stat-value fail">$script:failedTests</div>
        </div>
        <div class="stat">
            <div>Success Rate</div>
            <div class="stat-value">$([math]::Round(($script:passedTests / $script:totalTests) * 100, 2))%</div>
        </div>
    </div>
    
    <div class="summary">
        <h2>Detailed Results</h2>
        <table>
            <tr>
                <th>Test Case</th>
                <th>Mode</th>
                <th>Status</th>
                <th>Original Size</th>
                <th>Obfuscated Size</th>
                <th>Size Increase</th>
                <th>Duration (ms)</th>
            </tr>
"@

foreach ($result in $script:results) {
    $statusClass = if ($result.Status -eq "PASS") { "pass" } else { "fail" }
    $html += @"
            <tr>
                <td>$($result.TestCase)</td>
                <td>$($result.Mode)</td>
                <td class="$statusClass">$($result.Status)</td>
                <td>$($result.OriginalSize) bytes</td>
                <td>$($result.ObfuscatedSize) bytes</td>
                <td>$($result.SizeIncrease)%</td>
                <td>$([math]::Round($result.Duration, 2))</td>
            </tr>
"@
}

$html += @"
        </table>
    </div>
</body>
</html>
"@

$html | Out-File -FilePath $htmlPath -Encoding UTF8
Write-Host "✓ HTML report saved to: $htmlPath" -ForegroundColor Green

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Testing complete!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Results location: $resultsDir" -ForegroundColor White
Write-Host "  - Binaries: $resultsDir\binaries" -ForegroundColor White
Write-Host "  - Logs: $resultsDir\logs" -ForegroundColor White
Write-Host "  - Reports: $resultsDir\reports" -ForegroundColor White
Write-Host ""
Write-Host "View HTML report: $htmlPath" -ForegroundColor Cyan
Write-Host ""
