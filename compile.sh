#!/bin/zsh

#Pre-requisite, must be installed in the system :
# (*) Cmake
# (*) Ninja
# (*) LLVM + Clang
# (*) Vulkan SDK from LunarG
# (*) GLFW
# (*) Command_Line_Tools_for_Xcode_12.dmg

export PATH=~/MyProjects/clang+llvm-10.0.0-x86_64-apple-darwin/bin:~/MyProjects/Ninja1.10.1/ninja:$PATH
export CC=clang
export CXX=clang++
export SDKROOT='/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk'
export CPATH='/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk:/opt/local/include'
export LIBRARY_PATH='/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib:/opt/local/lib'
export CFLAGS='-g -O0'
#export GLFW3_DIR='/usr/local/lib/cmake/glfw3'

#Use cmake to perform OS operation
cmake -E rm -rf build           #delete directory build
cmake -E make_directory build   #create directory build

#Generator
#Use Ninja as generator on the CMakeLists in current directory, and generate under directory 'build'
cmake -E time cmake -G Ninja . -B build/

#Use cmake to build
#ninja -j 4 -C build
cmake -E time cmake --build build/
#"-DCMAKE_C_FLAGS:STRING=-g -O0"

#rm vert.spv frag.spv
#glslangValidator -V simple.frag
#glslangValidator -V simple.vert
#cp vert.spv frag.spv build/

#Run application
echo -e "\n********* STARTING ********\n"
build/./ICGV
echo -e "\n\n"
