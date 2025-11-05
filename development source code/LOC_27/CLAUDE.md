# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**Level of Confidence** is an interactive art installation by Rafael Lozano-Hemmer (2015) that uses facial recognition to match live camera input with portraits of 43 disappeared students from Ayotzinapa. This is an openFrameworks application (v0.12-2.0) running on macOS, supporting both Intel and ARM (M1/M2) architectures.

## Build and Run

### Building the project
```bash
# Build using make (recommended)
make

# Or build in Xcode
open LOC_26.xcodeproj
# Then build from Xcode (Cmd+B)
```

### Running
```bash
# Run the compiled application
open bin/LOC_26.app

# Or run from command line
./bin/LOC_26.app/Contents/MacOS/LOC_26

# Or from Xcode: Product > Run (Cmd+R)
```

### Compilation requirements
- openFrameworks v0.11.2 or of_v20230326_osx_release
- C++17 (set in config.make)
- macOS 10.15 or newer (minimum for C++17 support)
- **Compatible with macOS 13 and Apple Silicon (ARM64)**
- Xcode 13.4.1 or newer
- Face recognition model file: `bin/data/model_eigen_500_Tarrlab_rot+crop.yml` (~340MB)
- Face landmark detector: `bin/data/model/shape_predictor_68_face_landmarks.dat`

### Build Notes
- The project builds natively for Apple Silicon (ARM64) on M1/M2 Macs
- All dependencies (dlib, OpenCV) are included locally and self-contained
- No external system libraries required - everything builds from local_addons/
- The dlib library (ofxFaceTracker2) includes universal binary support (x86_64 + arm64)

## Architecture Overview

### Core Application Structure

The application follows a state-machine approach with multiple zoom stages controlling the visual presentation:

1. **ofApp** (`src/ofApp.{h,cpp}`) - Main application class coordinating all subsystems
2. **imageGrid** (`src/imageGrid.{h,cpp}`) - Manages the grid display of all 43 student portraits
3. **oneImage** (`src/oneImage.{h,cpp}`) - Handles rendering of individual student portraits with text

### Key Subsystems

**Face Detection & Tracking**
- Uses `ofxFaceTracker2` (dlib-based) for detecting faces in live camera feed
- Tracks 68 facial landmarks for feature extraction
- Handles camera rotation (0°, 90°, 180°, 270°) for portrait/landscape modes

**Face Recognition**
- `ofxFaceRecognizer` implements eigenfaces method with 500 training faces
- Maps 2319 training images to 43 unique students via `lookUpTable[]`
- Accumulates predictions in `prediction_array[]` and confidence scores in `avgConfidence_array[]`
- "Finale" mode triggers when any student gets 100+ matches (`bFinale = true`)

**Zoom State Machine** (controlled by `zoomStage` variable)
- Stage 1: Fully zoomed in (single large portrait)
- Stage 2: Mid zoom (view 3 portraits)
- Stage 3: Transition to new random student
- Stage 4: Full grid overview (all 43 portraits)
- Transitions based on face presence/absence timing (`faceLostDuration`, `faceFoundDuration`)

**Portrait Cropping**
- Live face detected → crop with `horizPadding` around face
- Grows/shrinks cropped rect using `growRect()` with lerp-based animation
- Maintains aspect ratio (`livePortrait_ratio`) for display

### Data Flow

```
Camera Input → Face Detection (ofxFaceTracker2)
            ↓
      Face Found?
            ↓ Yes
   Crop Face Region → Face Recognition (ofxFaceRecognizer)
            ↓
   Update Prediction Arrays (2319 faces → 43 students)
            ↓
   Select Matched Student (pickedID)
            ↓
   Display: Live Portrait + Matched Student Portrait + Info Text
```

### Critical Arrays & Lookups

- `prediction_array[2319]` - Hit count for each training image
- `avgConfidence_array[2319]` - Accumulated confidence per training image
- `prediction_array43[43]` - Aggregated hits per student (displayed as bars in grid)
- `lookUpTable[]` - Maps training image index → student ID (0-42)

## GUI Configuration

GUI settings are saved/loaded from XML files in `bin/data/GUIs/`:
- `gui_main.xml` - Main app settings
- `gui_tracking.xml` - Face tracking parameters (iterations, attempts, tolerance)
- `gui_livePortrait.xml` - Live portrait display settings
- `gui_grid.xml` - Student grid layout and fonts
- `gui_gridMovement.xml` - Zoom levels and animation speeds
- `gui_image.xml` - Picked portrait display settings
- `gui_infoText.xml` - Info text positioning and language

### Key Controls

- `g` - Toggle GUI visibility (saves settings on hide)
- `f` - Toggle fullscreen
- `Space` - Toggle language (Spanish/English)
- `Tab` - Reset to loading screen for 30 seconds
- `Ctrl+Alt+c` - Toggle camera debug view
- Double-click in bottom-right corner (200×200px) - Toggle GUI

### Camera Selection

The app uses **ofxAVFoundationGrabber** with unique device IDs for reliable camera selection:
- Camera selection dropdown in GUI (accessible via `g` key)
- Selected camera persists between sessions (`GUIs/gui_camera.xml`)
- Automatically prefers non-FaceTime cameras (external USB cameras)
- No restart required to switch cameras - just select from dropdown
- See `CAMERA_SELECTION_UPDATE.md` for full details

### Multi-Display and Responsive Layout

The app supports multiple displays and responsive layout:
- **Automatic display positioning:** Detects all connected monitors and positions the window on the leftmost display
- **Responsive layout:** Adapts grid and portrait displays to different window sizes and resolutions
- **Portrait mode optimized:** Default 1080x1920 resolution for portrait HD displays
- **Window resize handling:** `windowResized()` callback automatically updates layout when changing displays or resolutions
- **Configurable:** Monitor selection logic can be changed in `src/main.cpp` (leftmost, rightmost, or primary)
- See `MULTI_DISPLAY_AND_RESPONSIVE_LAYOUT.md` for full details

## Local Addons

The project uses custom/local versions of addons in `local_addons/`:
- **ofxCv** - OpenCV wrapper for openFrameworks
- **ofxFaceRecognizer** - Face recognition using OpenCV's eigenfaces/fisherfaces
- **ofxFaceTracker2** - Face detection and landmark tracking using dlib
- **ofxFontStash** - TrueType font rendering

## Important Implementation Details

### Camera Setup
The application auto-selects the first non-FaceTime camera (external USB webcams preferred). Camera rotation is applied via `rotate90To()` based on `camRotateAmt` parameter (requires app restart to take effect).

### Face Recognition Workflow
Recognition only runs when `bFinale == false`. Once a student reaches 100 matches, the system locks onto that student (`finalePickedID`) and stops updating predictions.

### Portrait Exclusion
The system can mark "exclusion zones" to avoid false positives - when face is stationary (`distTo_leftEye_center < 1`) for `exclusionDuration` seconds, eye positions are added to `exclusionArray` and marked on the camera image.

### Bilingual Support
Info text supports Spanish (0) and English (1) via `infoText_language` parameter. Text content loaded from `bin/data/infoText.txt` with specific line indices for each language.

## Data Requirements

The application requires a trained face recognition model and portrait images:
- Training database: `bin/data/Tarrlab_rot+crop/` (2319 images)
- Student portraits: `bin/data/portraits/` and `bin/data/CroppedStudents/` (43 students)
- Student names: `bin/data/names.txt` (one name per line)
- Model file must be pre-generated or downloaded (see readme.md for links)
