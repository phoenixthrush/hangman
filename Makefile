# Directories for build and output files
BUILD_DIR = build
OSX_BUILD_DIR = $(BUILD_DIR)/osx
WIN_BUILD_DIR = $(BUILD_DIR)/windows
OUTPUT_DIR = output

# URL for duden.txt
DUDEN_URL = https://raw.githubusercontent.com/MaximilianFreitag/Zufallswort/refs/heads/main/duden.txt
DUDEN_FILE = duden.txt

# Target to run both builds and cross-compiles
all: $(OUTPUT_DIR)/main $(OUTPUT_DIR)/main.exe

# Download duden.txt
$(DUDEN_FILE):
	curl -L $(DUDEN_URL) -o $(DUDEN_FILE)
	sed -i '' 's/ä/ae/g; s/ö/oe/g; s/ü/ue/g; s/ß/ss/g; s/Ä/Ae/g; s/Ö/Oe/g; s/Ü/Ue/g' duden.txt

# macOS Build
$(OSX_BUILD_DIR)/duden.o: duden.S $(DUDEN_FILE)
	mkdir -p $(OSX_BUILD_DIR)
	clang -c duden.S -o $(OSX_BUILD_DIR)/duden.o

$(OSX_BUILD_DIR)/main.o: main.c $(DUDEN_FILE)
	mkdir -p $(OSX_BUILD_DIR)
	clang -c -o $(OSX_BUILD_DIR)/main.o main.c -Os

$(OUTPUT_DIR)/main: $(OSX_BUILD_DIR)/main.o $(OSX_BUILD_DIR)/duden.o
	mkdir -p $(OUTPUT_DIR)
	clang -o $(OUTPUT_DIR)/main $(OSX_BUILD_DIR)/main.o $(OSX_BUILD_DIR)/duden.o

# Cross-compile for Windows
$(WIN_BUILD_DIR)/duden.o: $(DUDEN_FILE)
	mkdir -p $(WIN_BUILD_DIR)
	x86_64-w64-mingw32-ld -r -b binary -o $(WIN_BUILD_DIR)/duden.o $(DUDEN_FILE)

$(WIN_BUILD_DIR)/main.o: main.c $(DUDEN_FILE)
	mkdir -p $(WIN_BUILD_DIR)
	x86_64-w64-mingw32-gcc -c -o $(WIN_BUILD_DIR)/main.o main.c -Os

$(OUTPUT_DIR)/main.exe: $(WIN_BUILD_DIR)/main.o $(WIN_BUILD_DIR)/duden.o
	mkdir -p $(OUTPUT_DIR)
	x86_64-w64-mingw32-gcc -o $(OUTPUT_DIR)/main.exe $(WIN_BUILD_DIR)/main.o $(WIN_BUILD_DIR)/duden.o
	upx --best $(OUTPUT_DIR)/main.exe

# Clean all the generated files
clean:
	rm -rf $(BUILD_DIR) $(OUTPUT_DIR) $(DUDEN_FILE)
