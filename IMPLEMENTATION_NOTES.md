# ImageProcessor Enhancement - Photo Enlargement with Drag and Brush Tool

## Summary of Changes

This implementation adds new features to the ImageProcessor Qt application as requested:

### Features Implemented

#### 1. Photo Enlargement with Customizable Zoom Level
- Added a horizontal zoom slider (10% to 400%) in the main window
- Zoom level displayed as percentage next to the slider
- Real-time preview of the zoomed image in the main window
- The zoom affects how the image is displayed and what size image is passed to the edit window

#### 2. Drag Detection to Open Edit Window
- When a user clicks and drags on the enlarged photo in the main window
- A drag of at least 10 pixels triggers the opening of a new edit window
- The dragging action provides an intuitive way to "pull out" the image for editing

#### 3. New ImageEditWindow with Full Editing Capabilities
**Save As Functionality:**
- Toolbar button "另存新檔" (Save As) 
- Supports multiple formats: PNG, JPEG, BMP
- File dialog for selecting save location and filename
- Status bar feedback on successful save

**Brush Tool for Drawing:**
- Toggle button "筆刷工具" (Brush Tool) to enable/disable drawing mode
- Color picker "選擇顏色" (Choose Color) to select brush color
- Brush size slider (1-50 pixels) with real-time feedback
- Cross cursor when brush mode is active
- Smooth line drawing with anti-aliased lines
- Persistent drawing that modifies the image
- Can be saved with the "Save As" feature

### Technical Implementation

#### New Files
1. **imageeditwindow.h** - Header for the image edit window class
2. **imageeditwindow.cpp** - Implementation with brush drawing, save as, and UI

#### Modified Files
1. **imageprocessor.h** - Added zoom control members and drag detection
2. **imageprocessor.cpp** - Implemented zoom functionality and drag-to-open
3. **imagetransform.cpp** - Fixed typo (Qpainter -> QPainter)
4. **ImageProcessor.pro** - Added new source files to build

#### New Build Artifacts Control
5. **.gitignore** - Excludes build artifacts and object files

### User Workflow

1. **Open an image**: Use the "開啟檔案" (Open File) menu option
2. **Adjust zoom**: Use the zoom slider to enlarge the photo from 10% to 400%
3. **Drag to edit**: Click and drag on the image in the main window
4. **Edit window opens**: A new window appears with the zoomed image
5. **Enable brush**: Click the "筆刷工具" (Brush Tool) button
6. **Choose color**: Click "選擇顏色" (Choose Color) to select a brush color
7. **Adjust brush size**: Use the slider to change brush size (1-50 pixels)
8. **Draw on image**: Click and drag to draw on the image
9. **Save**: Click "另存新檔" (Save As) to save the edited image

### Code Quality
- Follows Qt best practices
- Proper memory management with Qt parent-child relationships
- Uses Qt's signal/slot mechanism for event handling
- Comprehensive status bar feedback for user actions
- Smooth, anti-aliased drawing with QPainter
- Supports multiple image formats

### Build Status
✓ Successfully builds with Qt 6.4.2
✓ No errors, only minor warnings about deprecated Qt APIs (Qt5->Qt6 migration related)
✓ All source files compile correctly
✓ Executable generated successfully

### Testing Recommendations
To test the application:
1. Build: `qmake6 ImageProcessor.pro && make`
2. Run: `./ImageProcessor`
3. Load an image file (PNG, BMP, or JPG)
4. Adjust the zoom slider to see real-time image scaling
5. Click and drag on the image to open the edit window
6. Try the brush tool with different colors and sizes
7. Save the edited image to verify Save As functionality

### Language Note
The application uses Traditional Chinese (繁體中文) for UI text, consistent with the existing codebase.
