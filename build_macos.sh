#!/bin/sh

PROGRAM_NAME=game_of_life
CFLAGS="-Wall -Wextra -std=c11"
MACOS_FRAMEWORKS="-framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -framework Foundation -framework AppKit"

rm -rf build
mkdir -p build

cc $CFLAGS -o build/$PROGRAM_NAME src/main.c \
  ./thirdparty/macos-raylib/lib/libraylib.a \
  -I./thirdparty/macos-raylib/include -L./thirdparty/macos-raylib/lib \
  $MACOS_FRAMEWORKS

APP_BUNDLE=build/$PROGRAM_NAME.app
mkdir -p $APP_BUNDLE/Contents/MacOS
mkdir -p $APP_BUNDLE/Contents/Resources

cp build/$PROGRAM_NAME $APP_BUNDLE/Contents/MacOS/
cp resources/icon.ico $APP_BUNDLE/Contents/Resources/

# Info.plist
cat > "$APP_BUNDLE/Contents/Info.plist" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>CFBundleExecutable</key>
  <string>$PROGRAM_NAME</string>
  <key>CFBundleIconFile</key>
  <string>icon.ico</string>
</dict>
</plist>
EOF

