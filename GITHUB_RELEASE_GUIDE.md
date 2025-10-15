# GitHub Release Creation Guide

## Step-by-Step Instructions to Create GitHub Release

### Option 1: Using GitHub Web Interface (Recommended)

1. **Go to Your Repository**
   ```
   https://github.com/NeuralNinja110/SIH25_2
   ```

2. **Navigate to Releases**
   - Click on "Releases" in the right sidebar
   - Or go to: https://github.com/NeuralNinja110/SIH25_2/releases

3. **Create New Release**
   - Click "Draft a new release" button

4. **Fill in Release Details**
   
   **Tag version:** `v2.0.5`
   - Click "Choose a tag" → Type "v2.0.5" → Click "Create new tag: v2.0.5 on publish"
   
   **Release title:** `Phantron Obfuscator v2.0.5 - Standalone Distributions`
   
   **Description:** Copy the content from `release/RELEASE_NOTES.md`

5. **Upload Distribution Files**
   
   Drag and drop or click "Attach binaries" to upload these files from the `release/` directory:
   
   - ✅ `phantron-obfuscator-windows-x64-v2.0.5.zip` (18.22 MB)
   - ✅ `phantron-obfuscator-windows-arm64-v2.0.5.zip` (13.22 MB)
   - ✅ `phantron-obfuscator-linux-x64-v2.0.5.tar.gz` (21.46 MB)
   - ✅ `phantron-obfuscator-linux-arm64-v2.0.5.tar.gz` (21.27 MB)
   - ✅ `SHA256SUMS.txt`

6. **Options**
   - ☑ Set as the latest release
   - ☐ Set as a pre-release (unchecked)
   - ☐ Create a discussion for this release (optional)

7. **Publish Release**
   - Click "Publish release" button

---

### Option 2: Using GitHub CLI (If Available)

If you install GitHub CLI later, you can use:

```bash
# Install GitHub CLI first
# Windows: winget install GitHub.cli
# Or download from: https://cli.github.com/

# Login to GitHub
gh auth login

# Create release
gh release create v2.0.5 \
  release/phantron-obfuscator-windows-x64-v2.0.5.zip \
  release/phantron-obfuscator-windows-arm64-v2.0.5.zip \
  release/phantron-obfuscator-linux-x64-v2.0.5.tar.gz \
  release/phantron-obfuscator-linux-arm64-v2.0.5.tar.gz \
  release/SHA256SUMS.txt \
  --title "Phantron Obfuscator v2.0.5 - Standalone Distributions" \
  --notes-file release/RELEASE_NOTES.md
```

---

### Option 3: Using Git Tags + Manual Upload

```bash
# Create and push tag
git tag -a v2.0.5 -m "Release v2.0.5 - Standalone Distributions"
git push origin v2.0.5

# Then manually upload files via web interface
```

---

## Files Ready for Upload

All files are located in the `release/` directory:

```
release/
├── phantron-obfuscator-windows-x64-v2.0.5.zip       (18.22 MB)
├── phantron-obfuscator-windows-arm64-v2.0.5.zip     (13.22 MB)
├── phantron-obfuscator-linux-x64-v2.0.5.tar.gz      (21.46 MB)
├── phantron-obfuscator-linux-arm64-v2.0.5.tar.gz    (21.27 MB)
├── SHA256SUMS.txt                                    (checksums)
└── RELEASE_NOTES.md                                  (release description)
```

**Total Upload Size:** ~74.17 MB

---

## Release Description Template

Use this as the release description (or copy from `RELEASE_NOTES.md`):

```markdown
# Phantron Obfuscator v2.0.5 - Standalone Distributions

## 🎉 What's New

### Standalone CLI Distributions
Complete standalone executables with no dependencies required! Download, extract, and run immediately.

### Consistent Branding
- Renamed to **Phantron LLVM Obfuscator** throughout
- Professional, consistent naming across all components

### Repository Cleanup
- Removed 96+ unnecessary files
- Professional structure for open-source distribution

## 📦 Downloads

Choose the package for your platform:

**Windows:**
- `phantron-obfuscator-windows-x64-v2.0.5.zip` - For Intel/AMD PCs
- `phantron-obfuscator-windows-arm64-v2.0.5.zip` - For ARM devices

**Linux:**
- `phantron-obfuscator-linux-x64-v2.0.5.tar.gz` - For Intel/AMD systems
- `phantron-obfuscator-linux-arm64-v2.0.5.tar.gz` - For ARM/Raspberry Pi

## 🚀 Quick Start

**Windows:**
1. Download and extract ZIP
2. Run `install.bat` as Administrator
3. Run `phantron --version`

**Linux:**
1. Download and extract: `tar -xzf *.tar.gz`
2. Install: `cd phantron-* && sudo ./install.sh`
3. Run `phantron --version`

## 📚 Documentation

- Installation: See INSTALLATION.md
- Usage: Run `phantron --help`
- Repository: https://github.com/NeuralNinja110/SIH25_2

## 👥 Team ELEVATE_1

Rahul Ashok | Pritham Devaprasad | Anish R | Siddarth S | Sahana Mutharasan | Sanjana Suresh
```

---

## Verification Steps

After creating the release:

1. **Verify Release Page**
   - Check: https://github.com/NeuralNinja110/SIH25_2/releases/tag/v2.0.5

2. **Test Download Links**
   - Download one package and verify it extracts correctly
   - Verify checksums match

3. **Update Documentation**
   - Update README.md with release link
   - Update INSTALLATION.md if needed

4. **Announce Release**
   - Tweet/share on social media
   - Post in relevant communities
   - Update project website

---

## Troubleshooting

**"Tag already exists"**
- Delete existing tag: `git tag -d v2.0.5 && git push origin :refs/tags/v2.0.5`

**"File too large"**
- GitHub has a 2GB limit per file (we're well under)
- All our files are < 50MB

**"Upload failed"**
- Try uploading files one at a time
- Check your internet connection
- Ensure you have write access to the repository

---

## Next Steps After Release

1. ☐ Update README.md with download links
2. ☐ Announce on social media
3. ☐ Submit to package managers (if applicable)
4. ☐ Update project documentation
5. ☐ Create release announcement blog post
6. ☐ Monitor for issues and feedback

---

**Ready to create your release! Follow the steps above.**
