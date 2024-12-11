# Compiler
CXX = clang++

# Source files
SRC = main.cpp

# Targets
TARGET_MACOS = BoxBrawl
APP_NAME = BoxBrawl
APP_DIR = $(APP_NAME)/Contents
MACOS_DIR = $(APP_DIR)/MacOS
RESOURCES_DIR = $(APP_DIR)/Resources/fonts

# macOS build settings
SDKROOT = $(shell xcrun --sdk macosx --show-sdk-path)
CXXFLAGS_MACOS = -std=c++17 -Wall -I/usr/local/include -isystem $(SDKROOT)/usr/include/c++/v1 -isysroot $(SDKROOT)
LDFLAGS_MACOS = -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system

# Default target
all: macos

# macOS build target
macos: $(APP_NAME)

$(APP_NAME): $(MACOS_DIR)/$(TARGET_MACOS)
	@echo "macOS application bundle created."

$(MACOS_DIR)/$(TARGET_MACOS): $(SRC)
	# Create the app directory structure
	mkdir -p $(MACOS_DIR) $(RESOURCES_DIR)
	# Compile the source code
	$(CXX) $(CXXFLAGS_MACOS) -o $(MACOS_DIR)/$(TARGET_MACOS) $(SRC) $(LDFLAGS_MACOS)
	# Copy resources (e.g., fonts)
	cp Pulang.ttf $(APP_NAME)/Contents/Resources/fonts

# Clean target
clean:
	rm -rf $(APP_NAME)