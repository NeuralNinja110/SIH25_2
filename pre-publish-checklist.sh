#!/bin/bash

echo "╔════════════════════════════════════════════════════════════════════════════╗"
echo "║                                                                            ║"
echo "║                   NPM PUBLISH PRE-FLIGHT CHECKLIST                         ║"
echo "║                                                                            ║"
echo "╚════════════════════════════════════════════════════════════════════════════╝"
echo ""

# Check 1: Package.json exists
echo "📋 Checking package.json..."
if [ -f "package.json" ]; then
    NAME=$(node -p "require('./package.json').name")
    VERSION=$(node -p "require('./package.json').version")
    echo "   ✅ Found: $NAME@$VERSION"
else
    echo "   ❌ package.json not found!"
    exit 1
fi
echo ""

# Check 2: CLI executable
echo "🔧 Checking CLI executable..."
if [ -f "cli/index.js" ]; then
    if [ -x "cli/index.js" ]; then
        echo "   ✅ cli/index.js is executable"
    else
        echo "   ⚠️  Making cli/index.js executable..."
        chmod +x cli/index.js
        echo "   ✅ Fixed"
    fi
else
    echo "   ❌ cli/index.js not found!"
    exit 1
fi
echo ""

# Check 3: Config files
echo "⚙️  Checking config files..."
CONFIGS=("config/balanced.yaml" "config/maximum_security.yaml" "config/size_conservative.yaml")
for config in "${CONFIGS[@]}"; do
    if [ -f "$config" ]; then
        echo "   ✅ $config"
    else
        echo "   ❌ $config not found!"
        exit 1
    fi
done
echo ""

# Check 4: Documentation
echo "📚 Checking documentation..."
DOCS=("README.md" "LICENSE" "PHANTRON_README.md")
for doc in "${DOCS[@]}"; do
    if [ -f "$doc" ]; then
        echo "   ✅ $doc"
    else
        echo "   ⚠️  $doc not found"
    fi
done
echo ""

# Check 5: Dependencies
echo "📦 Checking dependencies..."
if npm ls --depth=0 > /dev/null 2>&1; then
    echo "   ✅ All dependencies installed"
else
    echo "   ⚠️  Running npm install..."
    npm install
fi
echo ""

# Check 6: Test package
echo "🧪 Testing CLI..."
if node cli/index.js --version > /dev/null 2>&1; then
    VERSION_OUT=$(node cli/index.js --version)
    echo "   ✅ CLI works: $VERSION_OUT"
else
    echo "   ❌ CLI test failed!"
    exit 1
fi
echo ""

# Check 7: Package name availability
echo "🔍 Checking package name availability on npm..."
if npm view "$NAME" > /dev/null 2>&1; then
    PUBLISHED_VERSION=$(npm view "$NAME" version)
    echo "   ⚠️  Package already exists: $NAME@$PUBLISHED_VERSION"
    echo "   ℹ️  Your version: $VERSION"
    if [ "$VERSION" == "$PUBLISHED_VERSION" ]; then
        echo "   ❌ Version conflict! Update version in package.json"
        exit 1
    else
        echo "   ✅ Version is different, can update"
    fi
else
    echo "   ✅ Package name available for first publish"
fi
echo ""

# Check 8: npm login status
echo "🔐 Checking npm login status..."
if npm whoami > /dev/null 2>&1; then
    NPM_USER=$(npm whoami)
    echo "   ✅ Logged in as: $NPM_USER"
else
    echo "   ❌ Not logged in to npm!"
    echo "   ℹ️  Run: npm login"
    exit 1
fi
echo ""

# Check 9: Package size
echo "📊 Checking package size..."
npm pack --dry-run > /tmp/npm-pack.log 2>&1
PACKAGE_SIZE=$(grep "package size:" /tmp/npm-pack.log | awk '{print $4, $5}')
UNPACKED_SIZE=$(grep "unpacked size:" /tmp/npm-pack.log | awk '{print $4, $5}')
TOTAL_FILES=$(grep "total files:" /tmp/npm-pack.log | awk '{print $4}')
echo "   ✅ Package size: $PACKAGE_SIZE"
echo "   ✅ Unpacked size: $UNPACKED_SIZE"
echo "   ✅ Total files: $TOTAL_FILES"
echo ""

# Check 10: Git status
echo "🔄 Checking git status..."
if [ -n "$(git status --porcelain)" ]; then
    echo "   ⚠️  Uncommitted changes detected:"
    git status --short | head -5
    echo "   ℹ️  Consider committing before publishing"
else
    echo "   ✅ Working directory clean"
fi
echo ""

echo "╔════════════════════════════════════════════════════════════════════════════╗"
echo "║                                                                            ║"
echo "║                        ✅ PRE-FLIGHT CHECK PASSED                          ║"
echo "║                                                                            ║"
echo "╚════════════════════════════════════════════════════════════════════════════╝"
echo ""
echo "Ready to publish!"
echo ""
echo "To publish, run:"
echo "  npm publish --access public    (for first publish)"
echo "  npm publish                    (for updates)"
echo ""
