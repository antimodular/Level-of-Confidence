#!/bin/bash

# Script to create a distributable DMG for LOC_27
# This packages the entire development folder for distribution

APP_NAME="LOC_27"
DMG_NAME="${APP_NAME}"
SOURCE_APP="bin/${APP_NAME}.app"
DMG_DIR="dmg_build"
PROJECT_FOLDER="${APP_NAME}"
FINAL_DMG="${DMG_NAME}.dmg"

echo "Creating DMG for ${APP_NAME}..."

# Clean up any previous DMG builds
echo "Cleaning up previous builds..."
rm -rf "${DMG_DIR}"
rm -f "${FINAL_DMG}"
rm -f "${DMG_NAME}_temp.dmg"

# Create temporary directory for DMG contents
echo "Creating DMG directory structure..."
mkdir -p "${DMG_DIR}/${PROJECT_FOLDER}"

# Check if app exists
if [ ! -d "${SOURCE_APP}" ]; then
    echo "ERROR: ${SOURCE_APP} not found!"
    echo "Please build the app first using the Build RELEASE task."
    exit 1
fi

# Copy the entire project directory to DMG, excluding build artifacts and git
echo "Copying project files..."
rsync -av --exclude='.git' \
    --exclude='build/' \
    --exclude='obj/' \
    --exclude='.DS_Store' \
    --exclude='dmg_build/' \
    --exclude='*.dmg' \
    --exclude='.claude/' \
    ./ "${DMG_DIR}/${PROJECT_FOLDER}/"

echo "Project contents copied to DMG"

# Create the DMG
echo "Creating DMG..."
hdiutil create -volname "${DMG_NAME}" \
    -srcfolder "${DMG_DIR}" \
    -ov -format UDZO \
    "${FINAL_DMG}"

# Clean up temporary directory
echo "Cleaning up..."
rm -rf "${DMG_DIR}"

if [ -f "${FINAL_DMG}" ]; then
    echo ""
    echo "✓ SUCCESS: ${FINAL_DMG} created!"
    echo ""
    echo "You can now distribute this DMG file."
    echo "Users can:"
    echo "  1. Open the DMG"
    echo "  2. Copy the ${PROJECT_FOLDER} folder to their computer"
    echo "  3. The compiled app is in ${PROJECT_FOLDER}/bin/${APP_NAME}.app"
    echo "  4. All source code and project files are included"
    echo ""
    ls -lh "${FINAL_DMG}"
else
    echo "ERROR: Failed to create DMG"
    exit 1
fi
