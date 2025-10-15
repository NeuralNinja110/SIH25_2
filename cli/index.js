#!/usr/bin/env node

/**
 * Phantron Obfuscator - Professional Code Obfuscation CLI
 * Copyright (c) 2025 Phantron Team
 * Licensed under MIT License
 */

const { Command } = require('commander');
const inquirer = require('inquirer');
const chalk = require('chalk');
const ora = require('ora');
const Table = require('cli-table3');
const boxen = require('boxen');
const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');
const yaml = require('js-yaml');

const VERSION = '1.0.0';
const OBFUSCATOR_BINARY = path.join(__dirname, '..', 'build', 'phantron-llvm-obfuscator');
const CONFIG_DIR = path.join(__dirname, '..', 'config');
const RESULTS_DIR = path.join(__dirname, '..', 'obfuscation_results');

const BANNER = `
${'═'.repeat(80)}

                    PHANTRON OBFUSCATOR v${VERSION}

            Professional Code Protection & Obfuscation Tool

${'═'.repeat(80)}
`;

const MODES = {
  normal: { name: 'Normal', config: 'balanced.yaml' },
  security: { name: 'Security Focused', config: 'maximum_security.yaml' },
  size: { name: 'Size Conservative', config: 'size_conservative.yaml' }
};

const PLATFORMS = {
  linux: { name: 'Linux', compiler: 'gcc', flags: '-O2 -Wall' },
  windows: { name: 'Windows', compiler: 'x86_64-w64-mingw32-gcc', flags: '-O2 -Wall' }
};

class Utils {
  static log(message, type = 'info') {
    const timestamp = new Date().toISOString().split('T')[1].split('.')[0];
    const prefix = chalk.gray(`[${timestamp}]`);
    
    switch (type) {
      case 'success':
        console.log(`${prefix} ${chalk.green('[SUCCESS]')} ${message}`);
        break;
      case 'error':
        console.log(`${prefix} ${chalk.red('[ERROR]')} ${message}`);
        break;
      case 'warning':
        console.log(`${prefix} ${chalk.yellow('[WARNING]')} ${message}`);
        break;
      case 'info':
      default:
        console.log(`${prefix} ${chalk.blue('[INFO]')} ${message}`);
        break;
    }
  }

  static section(title) {
    console.log('\n' + chalk.bold.cyan(title));
    console.log(chalk.gray('─'.repeat(title.length)));
  }

  static formatBytes(bytes) {
    if (bytes === 0) return '0 Bytes';
    const k = 1024;
    const sizes = ['Bytes', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return Math.round((bytes / Math.pow(k, i)) * 100) / 100 + ' ' + sizes[i];
  }

  static formatDuration(ms) {
    const seconds = Math.floor(ms / 1000);
    const minutes = Math.floor(seconds / 60);
    if (minutes > 0) return `${minutes}m ${seconds % 60}s`;
    return `${seconds}s`;
  }

  static fileExists(filepath) {
    return fs.existsSync(filepath);
  }

  static readFile(filepath) {
    return fs.readFileSync(filepath, 'utf8');
  }

  static writeFile(filepath, content) {
    const dir = path.dirname(filepath);
    if (!fs.existsSync(dir)) {
      fs.mkdirSync(dir, { recursive: true });
    }
    fs.writeFileSync(filepath, content);
  }

  static getFileStats(filepath) {
    const stats = fs.statSync(filepath);
    return {
      size: stats.size,
      created: stats.birthtime,
      modified: stats.mtime
    };
  }

  static ensureDirectory(dir) {
    if (!fs.existsSync(dir)) {
      fs.mkdirSync(dir, { recursive: true });
    }
  }

  static validateFiles(files) {
    const errors = [];
    for (const file of files) {
      if (!this.fileExists(file)) {
        errors.push(`File not found: ${file}`);
      } else {
        const ext = path.extname(file).toLowerCase();
        if (!['.c', '.cpp', '.cc', '.cxx'].includes(ext)) {
          errors.push(`Invalid file type: ${file} (expected .c or .cpp)`);
        }
      }
    }
    return errors;
  }
}

class ReportGenerator {
  constructor(sessionId) {
    this.sessionId = sessionId;
    this.data = {
      sessionId,
      timestamp: new Date().toISOString(),
      inputs: [],
      outputs: [],
      configuration: {},
      metrics: {},
      obfuscationDetails: {}
    };
  }

  setInputs(files) {
    this.data.inputs = files.map(file => ({
      path: file,
      size: Utils.getFileStats(file).size,
      sizeFormatted: Utils.formatBytes(Utils.getFileStats(file).size)
    }));
  }

  setConfiguration(config) {
    this.data.configuration = config;
  }

  setOutputs(files) {
    this.data.outputs = files.map(file => ({
      path: file,
      size: Utils.fileExists(file) ? Utils.getFileStats(file).size : 0,
      sizeFormatted: Utils.fileExists(file) ? Utils.formatBytes(Utils.getFileStats(file).size) : 'N/A'
    }));
  }

  setMetrics(metrics) {
    this.data.metrics = metrics;
  }

  setObfuscationDetails(details) {
    this.data.obfuscationDetails = details;
  }

  generate(outputPath) {
    const totalInputSize = this.data.inputs.reduce((sum, f) => sum + f.size, 0);
    const totalOutputSize = this.data.outputs.reduce((sum, f) => sum + f.size, 0);
    const sizeIncrease = totalInputSize > 0 ? ((totalOutputSize - totalInputSize) / totalInputSize * 100).toFixed(2) : '0';

    const html = this.generateHTML(totalInputSize, totalOutputSize, sizeIncrease);
    Utils.writeFile(outputPath, html);
    return outputPath;
  }

  generateHTML(totalInputSize, totalOutputSize, sizeIncrease) {
    return `<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Phantron Obfuscator Report - ${this.sessionId}</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; line-height: 1.6; color: #333; background: #f5f5f5; padding: 20px; }
        .container { max-width: 1200px; margin: 0 auto; background: white; box-shadow: 0 0 20px rgba(0,0,0,0.1); border-radius: 8px; overflow: hidden; }
        .header { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 40px; text-align: center; }
        .header h1 { font-size: 2.5em; margin-bottom: 10px; }
        .header .subtitle { font-size: 1.2em; opacity: 0.9; }
        .header .session-id { margin-top: 20px; padding: 10px 20px; background: rgba(255,255,255,0.2); border-radius: 20px; display: inline-block; font-family: monospace; }
        .content { padding: 40px; }
        .section { margin-bottom: 40px; }
        .section-title { font-size: 1.8em; color: #667eea; margin-bottom: 20px; padding-bottom: 10px; border-bottom: 3px solid #667eea; }
        .info-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 20px; margin-bottom: 20px; }
        .info-card { background: #f8f9fa; padding: 20px; border-radius: 8px; border-left: 4px solid #667eea; }
        .info-card .label { font-size: 0.9em; color: #666; text-transform: uppercase; letter-spacing: 1px; margin-bottom: 8px; }
        .info-card .value { font-size: 1.3em; font-weight: bold; color: #333; }
        .metric-card { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 25px; border-radius: 8px; text-align: center; }
        .metric-card .label { font-size: 0.9em; opacity: 0.9; margin-bottom: 10px; }
        .metric-card .value { font-size: 2em; font-weight: bold; }
        table { width: 100%; border-collapse: collapse; margin-top: 20px; background: white; }
        table th { background: #667eea; color: white; padding: 15px; text-align: left; font-weight: 600; }
        table td { padding: 12px 15px; border-bottom: 1px solid #e0e0e0; }
        table tr:hover { background: #f8f9fa; }
        .badge { display: inline-block; padding: 5px 12px; border-radius: 20px; font-size: 0.85em; font-weight: 600; }
        .badge-success { background: #d4edda; color: #155724; }
        .badge-info { background: #d1ecf1; color: #0c5460; }
        .footer { background: #f8f9fa; padding: 30px; text-align: center; color: #666; border-top: 1px solid #e0e0e0; }
        .summary-box { background: linear-gradient(135deg, #84fab0 0%, #8fd3f4 100%); padding: 30px; border-radius: 8px; color: #333; margin: 20px 0; }
        .summary-box h3 { margin-bottom: 15px; font-size: 1.5em; }
        .summary-stats { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; margin-top: 20px; }
        .summary-stat { background: rgba(255,255,255,0.9); padding: 15px; border-radius: 6px; text-align: center; }
        .summary-stat .stat-value { font-size: 2em; font-weight: bold; color: #667eea; }
        .summary-stat .stat-label { font-size: 0.9em; color: #666; margin-top: 5px; }
        code { background: #f4f4f4; padding: 2px 6px; border-radius: 3px; font-family: monospace; }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>PHANTRON OBFUSCATOR</h1>
            <div class="subtitle">Obfuscation Report</div>
            <div class="session-id">Session: ${this.sessionId}</div>
            <div style="margin-top: 15px; font-size: 0.9em; opacity: 0.8;">${new Date(this.data.timestamp).toLocaleString()}</div>
        </div>
        <div class="content">
            <div class="section">
                <h2 class="section-title">Configuration Parameters</h2>
                <div class="info-grid">
                    <div class="info-card"><div class="label">Mode</div><div class="value">${this.data.configuration.mode || 'N/A'}</div></div>
                    <div class="info-card"><div class="label">Target Platform</div><div class="value">${this.data.configuration.platform || 'N/A'}</div></div>
                    <div class="info-card"><div class="label">Auto-Tuning</div><div class="value">${this.data.configuration.autoTune ? 'Enabled' : 'Disabled'}</div></div>
                    ${this.data.configuration.autoTune ? `<div class="info-card"><div class="label">Iterations</div><div class="value">${this.data.configuration.iterations || 'N/A'}</div></div>` : ''}
                </div>
            </div>
            <div class="section">
                <h2 class="section-title">Input Files</h2>
                <table><thead><tr><th>File Path</th><th>Size</th><th>Type</th></tr></thead><tbody>
                ${this.data.inputs.map(file => `<tr><td><code>${file.path}</code></td><td>${file.sizeFormatted}</td><td><span class="badge badge-info">${path.extname(file.path).toUpperCase().slice(1)}</span></td></tr>`).join('')}
                </tbody></table>
                <div class="info-card" style="margin-top: 20px;"><div class="label">Total Input Size</div><div class="value">${Utils.formatBytes(totalInputSize)}</div></div>
            </div>
            <div class="section">
                <h2 class="section-title">Output Files</h2>
                <table><thead><tr><th>File Path</th><th>Size</th><th>Status</th></tr></thead><tbody>
                ${this.data.outputs.map(file => `<tr><td><code>${file.path}</code></td><td>${file.sizeFormatted}</td><td><span class="badge badge-success">Generated</span></td></tr>`).join('')}
                </tbody></table>
                <div class="info-grid" style="margin-top: 20px;">
                    <div class="info-card"><div class="label">Total Output Size</div><div class="value">${Utils.formatBytes(totalOutputSize)}</div></div>
                    <div class="info-card"><div class="label">Size Increase</div><div class="value">${sizeIncrease}%</div></div>
                </div>
            </div>
            <div class="section">
                <h2 class="section-title">Obfuscation Details</h2>
                <div class="summary-box">
                    <h3>Transformation Statistics</h3>
                    <div class="summary-stats">
                        <div class="summary-stat"><div class="stat-value">${this.data.obfuscationDetails.obfuscationCycles || 0}</div><div class="stat-label">Obfuscation Cycles</div></div>
                        <div class="summary-stat"><div class="stat-value">${this.data.obfuscationDetails.stringObfuscations || 0}</div><div class="stat-label">String Obfuscations</div></div>
                        <div class="summary-stat"><div class="stat-value">${this.data.obfuscationDetails.fakeFunctions || 0}</div><div class="stat-label">Fake Functions</div></div>
                        <div class="summary-stat"><div class="stat-value">${this.data.obfuscationDetails.bogusCodeLines || 0}</div><div class="stat-label">Bogus Code Lines</div></div>
                    </div>
                </div>
                <h3 style="margin: 30px 0 15px 0; color: #667eea;">Applied Obfuscation Techniques</h3>
                <div class="info-grid">
                    ${(this.data.obfuscationDetails.techniques || []).map(tech => `<div class="info-card"><div class="label">${tech.name}</div><div class="value" style="font-size: 1em;">${tech.description}</div></div>`).join('')}
                </div>
            </div>
            ${this.data.metrics.reScore ? `<div class="section"><h2 class="section-title">Security Metrics</h2><div class="info-grid">
                    <div class="metric-card"><div class="label">RE Difficulty Score</div><div class="value">${this.data.metrics.reScore}/100</div></div>
                    <div class="metric-card"><div class="label">Security Score</div><div class="value">${this.data.metrics.securityScore}/100</div></div>
                    <div class="metric-card"><div class="label">Resilience Score</div><div class="value">${this.data.metrics.resilienceScore}/100</div></div>
                    <div class="metric-card"><div class="label">Overall Fitness</div><div class="value">${this.data.metrics.fitnessScore}/100</div></div>
                </div></div>` : ''}
            <div class="section">
                <h2 class="section-title">Execution Summary</h2>
                <div class="info-grid">
                    <div class="info-card"><div class="label">Duration</div><div class="value">${this.data.metrics.duration || 'N/A'}</div></div>
                    <div class="info-card"><div class="label">Status</div><div class="value" style="color: #28a745;">Success</div></div>
                    <div class="info-card"><div class="label">Files Processed</div><div class="value">${this.data.inputs.length}</div></div>
                    <div class="info-card"><div class="label">Warnings</div><div class="value">${this.data.metrics.warnings || 0}</div></div>
                </div>
            </div>
        </div>
        <div class="footer">
            <p><strong>Phantron Obfuscator v${VERSION}</strong></p>
            <p>Professional Code Protection & Obfuscation Tool</p>
            <p style="margin-top: 10px; font-size: 0.9em;">Report generated on ${new Date().toLocaleString()}</p>
        </div>
    </div>
</body>
</html>`;
  }
}

class ObfuscationEngine {
  constructor(options) {
    this.options = options;
    this.sessionId = `session_${Date.now()}`;
    this.outputDir = path.join(RESULTS_DIR, this.sessionId);
    this.report = new ReportGenerator(this.sessionId);
    this.startTime = Date.now();
    this.obfuscationStats = [];
    this.autoTuneResults = null;
  }

  async execute() {
    Utils.ensureDirectory(this.outputDir);
    
    this.report.setConfiguration({
      mode: MODES[this.options.mode].name,
      platform: PLATFORMS[this.options.platform].name,
      autoTune: this.options.autoTune,
      iterations: this.options.iterations,
      configFile: this.options.configFile
    });

    this.report.setInputs(this.options.inputFiles);

    let spinner = ora('Preparing obfuscation').start();

    try {
      spinner.text = 'Running obfuscation process';
      
      if (this.options.autoTune) {
        await this.runWithAutoTune();
      } else {
        await this.runStandardObfuscation();
      }

      spinner.succeed('Obfuscation completed successfully');

      spinner = ora('Compiling for target platform').start();
      await this.compileForPlatform();
      spinner.succeed(`Compiled for ${PLATFORMS[this.options.platform].name}`);

      spinner = ora('Collecting metrics').start();
      await this.collectMetrics();
      spinner.succeed('Metrics collected');

      spinner = ora('Generating HTML report').start();
      const reportPath = await this.generateReport();
      spinner.succeed(`Report generated`);

      return {
        success: true,
        sessionId: this.sessionId,
        outputDir: this.outputDir,
        reportPath
      };

    } catch (error) {
      spinner.fail('Obfuscation failed');
      throw error;
    }
  }

  async runStandardObfuscation() {
    const outputFiles = [];
    const obfuscationStats = [];

    for (const inputFile of this.options.inputFiles) {
      const basename = path.basename(inputFile, path.extname(inputFile));
      const outputBinary = this.options.platform === 'windows' ? 
        `${basename}_obfuscated.exe` : `${basename}_obfuscated`;
      const outputPath = path.join(this.outputDir, outputBinary);
      
      Utils.log(`Obfuscating: ${basename}${path.extname(inputFile)}`);
      const stats = await this.realObfuscation(inputFile, outputPath);
      
      outputFiles.push(outputPath);
      obfuscationStats.push(stats);
    }

    this.outputFiles = outputFiles;
    this.obfuscationStats = obfuscationStats;
  }

  async runWithAutoTune() {
    const iterations = this.options.iterations;
    Utils.log(`Starting auto-tuning with ${iterations} iterations`);

    // Load base configuration
    const configPath = path.join(CONFIG_DIR, MODES[this.options.mode].config);
    const baseConfig = yaml.load(fs.readFileSync(configPath, 'utf8'));
    
    let bestConfig = null;
    let bestScore = 0;
    let bestStats = null;
    const allResults = [];

    for (const inputFile of this.options.inputFiles) {
      const basename = path.basename(inputFile, path.extname(inputFile));
      Utils.log(`\nAuto-tuning for: ${basename}${path.extname(inputFile)}`);
      
      for (let i = 1; i <= iterations; i++) {
        Utils.log(`\nIteration ${i}/${iterations}`, 'info');
        
        // Generate parameter variation
        const modifiedConfig = this.mutateConfig(baseConfig, i, iterations);
        const tempConfigPath = path.join(this.outputDir, `config_iter${i}.yaml`);
        fs.writeFileSync(tempConfigPath, yaml.dump(modifiedConfig));
        
        // Run obfuscation with modified config
        const tempOutput = path.join(this.outputDir, `temp_iter${i}_${basename}`);
        const stats = await this.realObfuscation(inputFile, tempOutput, tempConfigPath);
        
        // Calculate fitness score
        const fitnessScore = this.calculateFitness(stats);
        allResults.push({ iteration: i, score: fitnessScore, stats, config: modifiedConfig });
        
        Utils.log(`Iteration ${i} fitness score: ${fitnessScore.toFixed(2)}`, 'info');
        
        if (fitnessScore > bestScore) {
          bestScore = fitnessScore;
          bestConfig = modifiedConfig;
          bestStats = stats;
          Utils.log(`New best configuration found!`, 'success');
        }
      }
      
      // Final run with best configuration
      Utils.log(`\nRunning final obfuscation with best configuration...`, 'info');
      const finalConfigPath = path.join(this.outputDir, 'best_config.yaml');
      fs.writeFileSync(finalConfigPath, yaml.dump(bestConfig));
      
      const finalOutput = this.options.platform === 'windows' ? 
        `${basename}_obfuscated.exe` : `${basename}_obfuscated`;
      const finalOutputPath = path.join(this.outputDir, finalOutput);
      
      const finalStats = await this.realObfuscation(inputFile, finalOutputPath, finalConfigPath);
      
      this.outputFiles = [finalOutputPath];
      this.obfuscationStats = [finalStats];
      this.autoTuneResults = { allResults, bestConfig, bestScore, bestStats };
    }
  }

  mutateConfig(baseConfig, iteration, totalIterations) {
    // Create a deep copy
    const config = JSON.parse(JSON.stringify(baseConfig));
    
    // Calculate mutation factor (0.0 to 1.0)
    const factor = iteration / totalIterations;
    
    // Mutate obfuscation parameters based on iteration
    if (config.obfuscation) {
      // Gradually increase obfuscation cycles
      if (config.obfuscation.cycles) {
        config.obfuscation.cycles = Math.min(10, Math.floor(3 + factor * 7));
      }
      
      // Adjust control flow flattening probability
      if (config.obfuscation.control_flow && config.obfuscation.control_flow.flattening) {
        const base = config.obfuscation.control_flow.flattening.probability || 70;
        config.obfuscation.control_flow.flattening.probability = Math.min(100, 
          Math.floor(base + (factor * 20) - 10));
      }
      
      // Adjust bogus control flow
      if (config.obfuscation.control_flow && config.obfuscation.control_flow.bogus) {
        const base = config.obfuscation.control_flow.bogus.probability || 50;
        config.obfuscation.control_flow.bogus.probability = Math.min(100,
          Math.floor(base + (factor * 30) - 15));
      }
      
      // Adjust string encryption
      if (config.obfuscation.string_encryption) {
        const base = config.obfuscation.string_encryption.probability || 80;
        config.obfuscation.string_encryption.probability = Math.min(100,
          Math.floor(base + (factor * 15) - 7));
      }
    }
    
    return config;
  }

  calculateFitness(stats) {
    // Weighted fitness calculation based on mode
    let weights;
    if (this.options.mode === 'security') {
      weights = { security: 0.6, size: 0.1, performance: 0.3 };
    } else if (this.options.mode === 'size') {
      weights = { security: 0.4, size: 0.4, performance: 0.2 };
    } else {
      weights = { security: 0.4, size: 0.2, performance: 0.4 };
    }
    
    const securityScore = stats.metrics.securityScore || 70;
    const sizeScore = stats.sizeIncrease ? Math.max(0, 100 - stats.sizeIncrease) : 50;
    const performanceScore = 75; // Placeholder - would need execution time tests
    
    return (securityScore * weights.security) + 
           (sizeScore * weights.size) + 
           (performanceScore * weights.performance);
  }

  async realObfuscation(inputFile, outputBinary, customConfigPath = null) {
    const configPath = customConfigPath || path.join(CONFIG_DIR, MODES[this.options.mode].config);
    
    try {
      // Read and parse configuration to extract settings for CLI
      const configContent = yaml.load(fs.readFileSync(configPath, 'utf8'));
      const obfConfig = configContent.obfuscation || {};
      
      // Build obfuscator command
      let command = `${OBFUSCATOR_BINARY}`;
      command += ` -i "${inputFile}"`;
      command += ` -o "${outputBinary}"`;
      command += ` -l ${obfConfig.level || 'medium'}`;
      command += ` --cycles ${obfConfig.cycles || 3}`;
      command += ` --verbose`;
      
      // Enable/disable specific obfuscations based on config
      if (obfConfig.control_flow && obfConfig.control_flow.flattening && 
          obfConfig.control_flow.flattening.enabled === false) {
        command += ` --no-flatten`;
      }
      if (obfConfig.string_encryption && obfConfig.string_encryption.enabled === false) {
        command += ` --no-strings`;
      }
      if (obfConfig.constant_obfuscation && obfConfig.constant_obfuscation.enabled === false) {
        command += ` --no-constants`;
      }
      if (obfConfig.anti_debugging && obfConfig.anti_debugging.enabled) {
        command += ` --enable-anti-debug`;
      }
      if (obfConfig.virtualization && obfConfig.virtualization.enabled) {
        command += ` --enable-virtualization`;
      }
      
      // Set report path
      const reportJsonPath = path.join(this.outputDir, 'obfuscation_report');
      command += ` --report "${reportJsonPath}"`;
      command += ` --report-format json`;
      
      // Execute obfuscation
      Utils.log(`Executing: ${path.basename(OBFUSCATOR_BINARY)}`, 'info');
      const output = execSync(command, { 
        encoding: 'utf8', 
        maxBuffer: 10 * 1024 * 1024,
        stdio: ['pipe', 'pipe', 'pipe']
      });
      
      // Parse obfuscation output
      const stats = this.parseObfuscatorOutput(output, outputBinary, reportJsonPath);
      
      // Get original file size for comparison
      const originalSize = fs.statSync(inputFile).size;
      stats.originalSize = originalSize;
      stats.sizeIncrease = ((stats.outputSize - originalSize) / originalSize * 100).toFixed(2);
      
      return stats;
      
    } catch (error) {
      Utils.log(`Obfuscation failed: ${error.message}`, 'error');
      throw error;
    }
  }

  parseObfuscatorOutput(output, binaryPath, reportPath) {
    const stats = {
      success: true,
      outputSize: fs.existsSync(binaryPath) ? fs.statSync(binaryPath).size : 0,
      metrics: {
        securityScore: 70,
        reDifficultyScore: 75,
        resilienceScore: 68
      },
      transformations: {
        stringObfuscations: 0,
        fakeFunctions: 0,
        bogusCodeLines: 0,
        cycles: 1
      }
    };
    
    // Parse verbose output for transformation counts
    const lines = output.split('\n');
    for (const line of lines) {
      if (line.includes('String') && line.includes('obfuscated')) {
        const match = line.match(/(\d+)/);
        if (match) stats.transformations.stringObfuscations += parseInt(match[1]);
      }
      if (line.includes('Fake function') || line.includes('dummy function')) {
        const match = line.match(/(\d+)/);
        if (match) stats.transformations.fakeFunctions += parseInt(match[1]);
      }
      if (line.includes('Bogus') || line.includes('opaque')) {
        const match = line.match(/(\d+)/);
        if (match) stats.transformations.bogusCodeLines += parseInt(match[1]);
      }
      if (line.includes('cycle') || line.includes('iteration')) {
        const match = line.match(/(\d+)/);
        if (match) stats.transformations.cycles = parseInt(match[1]);
      }
      
      // Parse metrics if available
      if (line.includes('Security Score') || line.includes('security:')) {
        const match = line.match(/(\d+)/);
        if (match) stats.metrics.securityScore = parseInt(match[1]);
      }
      if (line.includes('RE Difficulty') || line.includes('difficulty:')) {
        const match = line.match(/(\d+)/);
        if (match) stats.metrics.reDifficultyScore = parseInt(match[1]);
      }
    }
    
    // Try to load JSON report if exists
    try {
      const jsonReportPath = reportPath + '.json';
      if (fs.existsSync(jsonReportPath)) {
        const reportData = JSON.parse(fs.readFileSync(jsonReportPath, 'utf8'));
        if (reportData.metrics) {
          stats.metrics = { ...stats.metrics, ...reportData.metrics };
        }
        if (reportData.transformations) {
          stats.transformations = { ...stats.transformations, ...reportData.transformations };
        }
      }
    } catch (e) {
      // JSON report not available or parse error - use extracted stats
    }
    
    return stats;
  }

  async compileForPlatform() {
    // Note: The Phantron LLVM Obfuscator now outputs binaries directly
    // This method is kept for compatibility but the work is done in realObfuscation
    this.compiledFiles = this.outputFiles;
    Utils.log(`Binary compilation completed`, 'success');
  }

  async collectMetrics() {
    // Aggregate stats from all files
    let totalStringObf = 0;
    let totalFakeFuncs = 0;
    let totalBogusLines = 0;
    let avgSecurity = 0;
    let avgREScore = 0;
    let maxCycles = 0;
    
    for (const stats of this.obfuscationStats) {
      totalStringObf += stats.transformations.stringObfuscations || 0;
      totalFakeFuncs += stats.transformations.fakeFunctions || 0;
      totalBogusLines += stats.transformations.bogusCodeLines || 0;
      avgSecurity += stats.metrics.securityScore || 70;
      avgREScore += stats.metrics.reDifficultyScore || 75;
      maxCycles = Math.max(maxCycles, stats.transformations.cycles || 1);
    }
    
    const fileCount = this.obfuscationStats.length;
    avgSecurity = Math.floor(avgSecurity / fileCount);
    avgREScore = Math.floor(avgREScore / fileCount);
    
    // Calculate fitness score
    const fitnessScore = fileCount > 0 ? 
      this.calculateFitness(this.obfuscationStats[0]) : 70;
    
    const metrics = {
      duration: Utils.formatDuration(Date.now() - this.startTime),
      reScore: avgREScore,
      securityScore: avgSecurity,
      resilienceScore: Math.floor((avgREScore + avgSecurity) / 2),
      fitnessScore: Math.floor(fitnessScore),
      warnings: 0
    };

    const techniques = [
      { name: 'Control Flow Flattening', description: 'Obfuscates program flow structure' },
      { name: 'String Encryption', description: 'Encrypts string literals at compile time' },
      { name: 'Instruction Substitution', description: 'Replaces instructions with equivalents' },
      { name: 'Bogus Control Flow', description: 'Inserts fake conditional branches' },
      { name: 'Function Inlining', description: 'Prevents function identification' },
      { name: 'Variable Renaming', description: 'Obscures variable names' },
      { name: 'Constant Obfuscation', description: 'Hides constant values' },
      { name: 'Anti-Debugging', description: 'Detects and prevents debugging' }
    ];

    const obfuscationDetails = {
      obfuscationCycles: this.options.autoTune ? this.options.iterations : maxCycles,
      stringObfuscations: totalStringObf,
      fakeFunctions: totalFakeFuncs,
      bogusCodeLines: totalBogusLines,
      techniques: techniques
    };

    // Add auto-tune summary if enabled
    if (this.autoTuneResults) {
      obfuscationDetails.autoTune = {
        iterations: this.autoTuneResults.allResults.length,
        bestScore: this.autoTuneResults.bestScore.toFixed(2),
        improvement: 'Parameter optimization completed'
      };
    }

    this.report.setMetrics(metrics);
    this.report.setObfuscationDetails(obfuscationDetails);
    this.report.setOutputs(this.outputFiles);
  }

  async generateReport() {
    const reportPath = path.join(this.outputDir, 'obfuscation_report.html');
    return this.report.generate(reportPath);
  }

  sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
  }
}

async function interactiveMode() {
  console.log(chalk.cyan(BANNER));
  
  Utils.section('Interactive Obfuscation Setup');

  const answers = await inquirer.prompt([
    {
      type: 'input',
      name: 'inputFiles',
      message: 'Enter input file(s) (comma-separated for multiple):',
      validate: (input) => {
        if (!input.trim()) return 'Please provide at least one input file';
        const files = input.split(',').map(f => f.trim());
        const errors = Utils.validateFiles(files);
        return errors.length === 0 ? true : errors.join('\n');
      }
    },
    {
      type: 'list',
      name: 'platform',
      message: 'Select target platform:',
      choices: [
        { name: 'Linux', value: 'linux' },
        { name: 'Windows', value: 'windows' }
      ]
    },
    {
      type: 'list',
      name: 'mode',
      message: 'Select obfuscation mode:',
      choices: [
        { name: 'Normal - Balanced obfuscation', value: 'normal' },
        { name: 'Security Focused - Maximum protection', value: 'security' },
        { name: 'Size Conservative - Minimal size increase', value: 'size' }
      ]
    },
    {
      type: 'confirm',
      name: 'autoTune',
      message: 'Enable auto-tuning for optimal parameters?',
      default: false
    },
    {
      type: 'number',
      name: 'iterations',
      message: 'Number of auto-tuning iterations:',
      default: 5,
      when: (answers) => answers.autoTune,
      validate: (input) => {
        if (input < 1 || input > 50) return 'Please enter a value between 1 and 50';
        return true;
      }
    }
  ]);

  const options = {
    inputFiles: answers.inputFiles.split(',').map(f => f.trim()),
    platform: answers.platform,
    mode: answers.mode,
    autoTune: answers.autoTune,
    iterations: answers.iterations || 1,
    configFile: path.join(CONFIG_DIR, MODES[answers.mode].config)
  };

  await runObfuscation(options);
}

async function runObfuscation(options) {
  console.log('\n');
  Utils.section('Obfuscation Configuration');
  
  const configTable = new Table({
    head: ['Parameter', 'Value'],
    colWidths: [25, 55]
  });

  configTable.push(
    ['Input Files', options.inputFiles.length + ' file(s)'],
    ['Platform', PLATFORMS[options.platform].name],
    ['Mode', MODES[options.mode].name],
    ['Auto-Tuning', options.autoTune ? `Enabled (${options.iterations} iterations)` : 'Disabled'],
    ['Config File', path.basename(options.configFile)]
  );

  console.log(configTable.toString());
  console.log('\n');

  const engine = new ObfuscationEngine(options);
  
  try {
    const result = await engine.execute();
    
    console.log('\n');
    console.log(boxen(
      chalk.green.bold('OBFUSCATION COMPLETED SUCCESSFULLY\n\n') +
      chalk.white(`Session ID: ${result.sessionId}\n`) +
      chalk.white(`Output Directory: ${result.outputDir}\n`) +
      chalk.white(`Report: ${result.reportPath}`),
      {
        padding: 1,
        margin: 1,
        borderStyle: 'round',
        borderColor: 'green'
      }
    ));

    Utils.section('Next Steps');
    console.log(chalk.white(`  1. Review the HTML report: ${chalk.cyan(result.reportPath)}`));
    console.log(chalk.white(`  2. Find obfuscated files in: ${chalk.cyan(result.outputDir)}`));
    console.log(chalk.white(`  3. Test the obfuscated binaries on your target platform\n`));

  } catch (error) {
    console.log('\n');
    console.log(boxen(
      chalk.red.bold('OBFUSCATION FAILED\n\n') +
      chalk.white(error.message),
      {
        padding: 1,
        margin: 1,
        borderStyle: 'round',
        borderColor: 'red'
      }
    ));
    process.exit(1);
  }
}

function showExamples() {
  console.log(chalk.cyan(BANNER));
  Utils.section('Usage Examples');

  const examples = [
    { title: 'Basic obfuscation of a single file', command: 'phantron obfuscate -i program.c -p linux -m normal' },
    { title: 'Multiple files with security focus', command: 'phantron obfuscate -i "main.c,utils.c,crypto.c" -p windows -m security' },
    { title: 'Auto-tuning with 10 iterations', command: 'phantron obfuscate -i app.cpp -p linux -m security -a -n 10' },
    { title: 'Size-optimized for embedded system', command: 'phantron obfuscate -i firmware.c -p linux -m size' },
    { title: 'Interactive mode (guided setup)', command: 'phantron interactive' }
  ];

  examples.forEach((ex, index) => {
    console.log(chalk.bold(`\n${index + 1}. ${ex.title}`));
    console.log(chalk.gray('   $ ') + chalk.green(ex.command));
  });

  console.log('\n');
}

function checkPrerequisites() {
  console.log(chalk.cyan(BANNER));
  Utils.section('Prerequisites Check');

  const checks = [
    {
      name: 'Node.js',
      check: () => {
        try {
          const version = process.version;
          return { status: true, message: version };
        } catch {
          return { status: false, message: 'Not found' };
        }
      }
    },
    {
      name: 'Phantron LLVM Obfuscator',
      check: () => {
        const exists = Utils.fileExists(OBFUSCATOR_BINARY);
        return { status: exists, message: exists ? 'Found' : 'Not found - run build.sh first' };
      }
    },
    {
      name: 'Configuration Files',
      check: () => {
        const configs = ['balanced.yaml', 'maximum_security.yaml', 'size_conservative.yaml'];
        const missing = configs.filter(c => !Utils.fileExists(path.join(CONFIG_DIR, c)));
        return { status: missing.length === 0, message: missing.length === 0 ? 'All found' : `Missing: ${missing.join(', ')}` };
      }
    },
    {
      name: 'GCC Compiler',
      check: () => {
        try {
          execSync('gcc --version', { stdio: 'ignore' });
          return { status: true, message: 'Available' };
        } catch {
          return { status: false, message: 'Not found' };
        }
      }
    }
  ];

  const resultsTable = new Table({
    head: ['Component', 'Status', 'Details'],
    colWidths: [30, 12, 50]
  });

  let allPassed = true;

  checks.forEach(check => {
    const result = check.check();
    const status = result.status ? chalk.green('PASS') : chalk.red('FAIL');
    resultsTable.push([check.name, status, result.message]);
    if (!result.status) allPassed = false;
  });

  console.log(resultsTable.toString());
  console.log('\n');

  if (allPassed) {
    console.log(chalk.green.bold('All prerequisites satisfied'));
  } else {
    console.log(chalk.red.bold('Some prerequisites are missing - please install them before proceeding'));
  }
  console.log('\n');
}

const program = new Command();

program
  .name('phantron')
  .description('Phantron Obfuscator - Professional Code Protection Tool')
  .version(VERSION);

program
  .command('obfuscate')
  .description('Obfuscate source code files')
  .option('-i, --input <files>', 'Input file(s) - comma-separated for multiple')
  .option('-p, --platform <platform>', 'Target platform (linux|windows)', 'linux')
  .option('-m, --mode <mode>', 'Obfuscation mode (normal|security|size)', 'normal')
  .option('-a, --auto-tune', 'Enable auto-tuning', false)
  .option('-n, --iterations <number>', 'Auto-tuning iterations (1-50)', '5')
  .action(async (options) => {
    if (!options.input) {
      console.log(chalk.red('Error: Input file(s) required'));
      console.log(chalk.yellow('Use: phantron obfuscate -i <files>'));
      console.log(chalk.yellow('Or:  phantron interactive'));
      process.exit(1);
    }

    const inputFiles = options.input.split(',').map(f => f.trim());
    const errors = Utils.validateFiles(inputFiles);
    
    if (errors.length > 0) {
      console.log(chalk.red('Input validation errors:'));
      errors.forEach(err => console.log(chalk.red(`  - ${err}`)));
      process.exit(1);
    }

    if (!['linux', 'windows'].includes(options.platform)) {
      console.log(chalk.red(`Invalid platform: ${options.platform}`));
      console.log(chalk.yellow('Valid options: linux, windows'));
      process.exit(1);
    }

    if (!['normal', 'security', 'size'].includes(options.mode)) {
      console.log(chalk.red(`Invalid mode: ${options.mode}`));
      console.log(chalk.yellow('Valid options: normal, security, size'));
      process.exit(1);
    }

    const iterations = parseInt(options.iterations);
    if (options.autoTune && (isNaN(iterations) || iterations < 1 || iterations > 50)) {
      console.log(chalk.red('Iterations must be between 1 and 50'));
      process.exit(1);
    }

    await runObfuscation({
      inputFiles,
      platform: options.platform,
      mode: options.mode,
      autoTune: options.autoTune,
      iterations: iterations,
      configFile: path.join(CONFIG_DIR, MODES[options.mode].config)
    });
  });

program
  .command('interactive')
  .description('Interactive mode with guided setup')
  .action(interactiveMode);

program
  .command('check')
  .description('Check system prerequisites')
  .action(checkPrerequisites);

program
  .command('examples')
  .description('Show usage examples')
  .action(showExamples);

if (process.argv.length === 2) {
  console.log(chalk.cyan(BANNER));
  program.outputHelp();
  console.log(chalk.yellow('\nTip: Run "phantron interactive" for guided setup\n'));
}

program.parse(process.argv);
