#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

#CXX = g++
#CXX = clang++

CXX = emcc
OUTPUT = imgui.html

SOURCES = main.cpp
SOURCES += imgui_impl_glfw.cpp imgui_impl_opengl3.cpp
SOURCES += imgui.cpp imgui_draw.cpp imgui_demo.cpp
#SOURCES += ../../imgui.cpp ../../imgui_demo.cpp ../../imgui_draw.cpp
#SOURCES += ../libs/gl3w/GL/gl3w.c


UNAME_S := $(shell uname -s)


LIBS = -lGLU -lGL


all: $(SOURCES) $(OUTPUT) 

$(OUTPUT): $(SOURCES)
	$(CXX)  $(SOURCES) --bind -std=c++11 --llvm-opts 2 -o $(OUTPUT) $(LIBS) -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1 -O2 --preload-file data

clean:
	rm -f $(OUTPUT)
