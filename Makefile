# Compiler
CXX = g++

# Source files
SRC = main.cpp

# Targets
TARGET_MACOS = BoxBrawl
APP_NAME = BoxBrawl.app
APP_DIR = $(APP_NAME)/Contents
MACOS_DIR = $(APP_DIR)/MacOS
RESOURCES_DIR = $(APP_DIR)/Resources

# macOS build settings
SDKROOT = $(shell xcrun --sdk macosx --show-sdk-path)
CXXFLAGS_MACOS = -std=c++17 -Wall -I/usr/local/include -isystem $(SDKROOT)/usr/include/c++/v1 -isysroot $(SDKROOT)
LDFLAGS_MACOS = -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system

# Default target
all: macos

# macOS build target
macos: $(APP_NAME)

$(APP_NAME): $(MACOS_DIR)/$(TARGET_MACOS)

$(MACOS_DIR)/$(TARGET_MACOS): $(SRC)
	# Create the app directory structure
	mkdir -p $(MACOS_DIR) $(RESOURCES_DIR)
	# Compile the binary
	$(CXX) $(CXXFLAGS_MACOS) -o $(MACOS_DIR)/$(TARGET_MACOS) $(SRC) $(LDFLAGS_MACOS)
	# Copy resources
	cp ./icon.png $(RESOURCES_DIR)/icon.png
	# Create the Info.plist file
	@echo "Creating Info.plist..."
	@echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" > $(APP_DIR)/Info.plist
	@echo "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" >> $(APP_DIR)/Info.plist
	@echo "<plist version=\"1.0\">" >> $(APP_DIR)/Info.plist
	@echo "<dict>" >> $(APP_DIR)/Info.plist
	@echo "  <key>CFBundleExecutable</key>" >> $(APP_DIR)/Info.plist
	@echo "  <string>$(TARGET_MACOS)</string>" >> $(APP_DIR)/Info.plist
	@echo "  <key>CFBundleName</key>" >> $(APP_DIR)/Info.plist
	@echo "  <string>BoxBrawl</string>" >> $(APP_DIR)/Info.plist
	@echo "  <key>CFBundleIconFile</key>" >> $(APP_DIR)/Info.plist
	@echo "  <string>icon.png</string>" >> $(APP_DIR)/Info.plist
	@echo "</dict>" >> $(APP_DIR)/Info.plist
	@echo "</plist>" >> $(APP_DIR)/Info.plist
	@echo "$(APP_NAME) created successfully!"

# Clean build artifacts
clean:
	rm -rf $(TARGET_MACOS) $(APP_NAME)
