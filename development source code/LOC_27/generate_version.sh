#!/bin/bash

# Get git version info
GIT_BRANCH=$(git rev-parse --abbrev-ref HEAD 2>/dev/null || echo "unknown")
GIT_COMMITS=$(git rev-list --count HEAD 2>/dev/null || echo "0")
GIT_HASH=$(git rev-parse --short HEAD 2>/dev/null || echo "unknown")
BUILD_DATE=$(date +"%Y-%m-%d %H:%M:%S")

# Generate version string: branch_commits_hash
VERSION="${GIT_BRANCH}_${GIT_COMMITS}_${GIT_HASH}"

# Create C++ header (not tracked by git)
cat > src/version.h << EOF
// Auto-generated file - DO NOT EDIT
// Generated at build time from git information

#pragma once

#define VERSION_STRING "${VERSION}"
#define VERSION_BRANCH "${GIT_BRANCH}"
#define VERSION_COMMITS ${GIT_COMMITS}
#define VERSION_HASH "${GIT_HASH}"
#define BUILD_DATE "${BUILD_DATE}"

namespace Version {
    inline const char* getString() { return VERSION_STRING; }
    inline const char* getBranch() { return VERSION_BRANCH; }
    inline int getCommits() { return VERSION_COMMITS; }
    inline const char* getHash() { return VERSION_HASH; }
    inline const char* getBuildDate() { return BUILD_DATE; }
}
EOF

# Create human-readable VERSION file for distribution
mkdir -p bin
cat > bin/VERSION.txt << EOF
LOC_27 ${VERSION}
Built: ${BUILD_DATE}

Branch: ${GIT_BRANCH}
Commits: ${GIT_COMMITS}
Hash: ${GIT_HASH}
EOF

# Export for Makefile/Xcode
echo "VERSION=${VERSION}" > version.env

echo "Generated version: ${VERSION}"
