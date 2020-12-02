#!/bin/zsh

#Pre-requisite, must be installed in the system :
# (*) Cmake
# (*) Ninja
# (*) LLVM + Clang
# (*) Vulkan SDK from LunarG
# (*) GLFW
# (*) Command_Line_Tools_for_Xcode_12.dmg

export CC=clang
export CXX=clang++
export PATH=~/MyProjects/clang+llvm-10.0.0-x86_64-apple-darwin/bin:~/MyProjects/Ninja1.10.1/ninja:$PATH
export SDKROOT="/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk"
export CPATH="/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk:/opt/local/include"
export LIBRARY_PATH="$LIBRARY_PATH:/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib:/opt/local/lib"
#export GLFW3_DIR='/usr/lib/cmake/glfw3'

#rather put this in CMake file
#export CFLAGS='-c -Wall -Wunused-variable -Wunused-function -Wmissing-braces -Wunused-command-line-argument'
#export LDFLAGS="-lpthread -libm -lglfw3"
#export LDFLAGS="-L/usr/local/opt/ncurses/lib"
#export CFLAGS="-I/usr/local/opt/ncurses/include"

#source Vulkan/1.2.154.0/setup-env.sh  #https://vulkan.lunarg.com/doc/sdk/1.2.154.0/linux/getting_started.html
#echo "Vulkan SDK path = $VULKAN_SDK"

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

executableFileName=ICGV
#Run application if it exists (will exist if was compiled successfully)
if [ -f build/$executableFileName ]; then
        echo -e "\n********* STARTING ********\n"
        build/./$executableFileName
else 
    echo -e "\nERROR: Compilation failed, $executableFileName does not exist.\n"
fi
echo -e "\n\n"

