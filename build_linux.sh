#!/bin/sh

PROGRAM_NAME=game_of_life
CFLAGS="-Wall -Wextra -std=c11"

rm -rf build
mkdir -p build

cc $CFLAGS -o build/$PROGRAM_NAME src/main.c \
    ./thirdparty/linux-raylib/lib/libraylib.a -I./thirdparty/linux-raylib/include -L./thirdparty/linux-raylib/lib -lGL -lm -lpthread -ldl -lrt -lX11

