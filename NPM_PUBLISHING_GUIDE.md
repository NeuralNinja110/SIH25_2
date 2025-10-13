# Publishing to NPM - Complete Guide

## 📦 NPM Package: @maos/auto-tuner

This guide walks you through publishing the MAOS Auto-Tuner CLI to npm.

## 🚀 Pre-Publication Checklist

### 1. Prerequisites

- [ ] Node.js >= 14.0.0 installed
- [ ] npm account created (https://www.npmjs.com/signup)
- [ ] npm CLI logged in (`npm login`)
- [ ] Package name available on npm registry

### 2. Package Preparation

- [ ] `package.json` configured correctly
- [ ] `NPM_README.md` renamed to `README.md` for npm
- [ ] `LICENSE` file present
- [ ] `.npmignore` configured to exclude unnecessary files
- [ ] CLI entry point (`cli/index.js`) has shebang and is executable
- [ ] All dependencies listed in `package.json`

### 3. Testing

- [ ] Test CLI locally: `node cli/index.js --help`
- [ ] Test with `npm link` globally
- [ ] Test all commands work
- [ ] Test on clean system if possible

## 📝 Step-by-Step Publishing

### Step 1: Prepare README for NPM

```bash
# Rename NPM_README.md to README.md for npm package
cp NPM_README.md README.npm.md
```

### Step 2: Update package.json

Ensure these fields are correct:

```json
{
  "name": "@maos/auto-tuner",
  "version": "2.0.0",
  "description": "Intelligent auto-tuning system for MAOS obfuscation parameters",
  "main": "cli/index.js",
  "bin": {
    "maos-tune": "./cli/index.js"
  }
}
```

### Step 3: Install Dependencies Locally

```bash
npm install --production
```

### Step 4: Test Package Locally

```bash
# Link package globally for testing
npm link

# Test commands
maos-tune --version
maos-tune --help
maos-tune examples
maos-tune check

# Unlink after testing
npm unlink -g @maos/auto-tuner
```

### Step 5: Login to npm

```bash
npm login
# Enter username, password, and email
```

### Step 6: Publish to npm

#### For Scoped Package (Recommended)

```bash
# First time publishing a scoped package (public)
npm publish --access public

# Subsequent updates
npm publish
```

#### For Unscoped Package

```bash
# Change package name in package.json to "maos-auto-tuner"
npm publish
```

### Step 7: Verify Publication

```bash
# Check on npm registry
npm view @maos/auto-tuner

# Install globally to test
npm install -g @maos/auto-tuner

# Test
maos-tune --version
```

## 📊 Package Structure for NPM

After `.npmignore` is applied, the package includes:

```
@maos/auto-tuner/
├── cli/
│   └── index.js              # Main CLI entry point
├── config/
│   ├── maximum_security.yaml
│   └── size_conservative.yaml
├── docs/
│   └── AUTO_TUNING.md        # Complete documentation
├── package.json              # Package metadata
├── README.md                 # NPM README (from NPM_README.md)
└── LICENSE                   # MIT License
```

## 🔄 Updating the Package

### Version Bumping

```bash
# Patch release (bug fixes): 2.0.0 -> 2.0.1
npm version patch

# Minor release (new features): 2.0.0 -> 2.1.0
npm version minor

# Major release (breaking changes): 2.0.0 -> 3.0.0
npm version major
```

### Publish Update

```bash
# After version bump
npm publish
```

## 📋 Pre-Publish Checklist Script

Create a file `scripts/pre-publish.sh`:

```bash
#!/bin/bash

echo "🔍 Pre-Publish Checklist"
echo ""

# Check Node.js version
echo "✓ Checking Node.js version..."
node --version | grep -E "v1[4-9]|v[2-9][0-9]" || {
    echo "❌ Node.js >= 14.0.0 required"
    exit 1
}

# Check package.json
echo "✓ Checking package.json..."
[ -f package.json ] || {
    echo "❌ package.json not found"
    exit 1
}

# Check CLI entry point
echo "✓ Checking CLI entry point..."
[ -f cli/index.js ] || {
    echo "❌ cli/index.js not found"
    exit 1
}

# Check shebang
grep -q "^#!/usr/bin/env node" cli/index.js || {
    echo "❌ Missing shebang in cli/index.js"
    exit 1
}

# Check LICENSE
echo "✓ Checking LICENSE..."
[ -f LICENSE ] || {
    echo "❌ LICENSE file not found"
    exit 1
}

# Check README
echo "✓ Checking README..."
[ -f NPM_README.md ] || {
    echo "❌ NPM_README.md not found"
    exit 1
}

# Test CLI
echo "✓ Testing CLI..."
node cli/index.js --version > /dev/null 2>&1 || {
    echo "❌ CLI test failed"
    exit 1
}

echo ""
echo "✅ All pre-publish checks passed!"
echo ""
echo "Ready to publish:"
echo "  npm publish --access public"
```

## 🔐 Security Best Practices

### 1. Use npm 2FA

```bash
npm profile enable-2fa auth-and-writes
```

### 2. Audit Dependencies

```bash
npm audit
npm audit fix
```

### 3. Check for Vulnerabilities

```bash
npm install -g snyk
snyk test
```

## 📢 Post-Publication Tasks

### 1. Create Git Tag

```bash
git tag v2.0.0
git push origin v2.0.0
```

### 2. Create GitHub Release

- Go to GitHub repository
- Click "Releases" → "Create a new release"
- Select tag `v2.0.0`
- Add release notes
- Publish release

### 3. Update Documentation

- Update main README.md with npm install instructions
- Update CHANGELOG.md with new version changes
- Announce on social media/forums

### 4. Monitor Package

- Check npm download stats: https://npm-stat.com/charts.html?package=@maos/auto-tuner
- Monitor issues and pull requests
- Respond to user feedback

## 🐛 Troubleshooting

### Issue: "You do not have permission to publish"

**Solution:**
```bash
# Login again
npm logout
npm login

# Or check if you're logged in to correct account
npm whoami
```

### Issue: "Package name already exists"

**Solution:**
```bash
# Use scoped package
# Change name in package.json to "@your-username/auto-tuner"
# Or choose different unscoped name
```

### Issue: "ENOENT: no such file or directory"

**Solution:**
```bash
# Check .npmignore isn't excluding necessary files
cat .npmignore

# Test what files will be included
npm pack --dry-run
```

### Issue: "Module not found" after installation

**Solution:**
```bash
# Check main entry point in package.json
# Ensure all dependencies are listed
npm install
```

## 📊 npm Package Metrics

After publication, monitor:

- **Downloads**: https://npm-stat.com/charts.html?package=@maos/auto-tuner
- **Bundle Size**: https://bundlephobia.com/package/@maos/auto-tuner
- **Package Health**: https://snyk.io/advisor/npm-package/@maos/auto-tuner

## 🎯 Quick Publish Commands

```bash
# Complete publish workflow
npm install                      # Install dependencies
npm test                         # Run tests
npm version patch                # Bump version
npm publish --access public      # Publish to npm
git push && git push --tags      # Push to Git
```

## 📝 Example: First Time Publishing

```bash
# 1. Prepare
cd /workspaces/SIH25_2
npm install

# 2. Test locally
npm link
maos-tune --help
npm unlink -g @maos/auto-tuner

# 3. Login to npm
npm login

# 4. Publish
npm publish --access public

# 5. Verify
npm view @maos/auto-tuner

# 6. Test installation
npm install -g @maos/auto-tuner
maos-tune --version
```

## ✅ Success!

Your package is now live on npm! Users can install it with:

```bash
npm install -g @maos/auto-tuner
```

## 🔗 Useful Links

- **npm Documentation**: https://docs.npmjs.com/
- **Publishing Scoped Packages**: https://docs.npmjs.com/creating-and-publishing-scoped-public-packages
- **CLI Best Practices**: https://github.com/lirantal/nodejs-cli-apps-best-practices
- **Semantic Versioning**: https://semver.org/

---

**Last Updated:** October 13, 2025
**Package Version:** 2.0.0
**Status:** Ready for Publication ✅
