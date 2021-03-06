cmake_minimum_required(VERSION 3.18.X)
project(ICGV_Project LANGUAGES C)
add_executable(ICGV main.c)

#Compile with no optimization and adding debug symbols
set(CMAKE_BUILD_TYPE debug)
#Compile in release mode optimized but adding debug symbols
#set(CMAKE_BUILD_TYPE RelWithDebInfo)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-c -Wall -Wextra -pedantic -Werror)
endif()
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_DEBUG -D__DEBUG -O0")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wunused-variable -Wunused-function -Wmissing-braces -Wunused-command-line-argument -g -O0 -fstack-protector-all")
#set(LINKER_FLAGS "-lpthread")
#target_link_libraries(ICGV microkernel ${LINKER_FLAGS})
#set(LD_FLAGS ${LINKER_FLAGS})
#set(LDFLAGS ${LINKER_FLAGS})

# Compiler Condition (clang ; clang++) LLVM Clang (clang.llvm.org)
#if ( "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" )
  #message( STATUS "Setting the flags for ${CMAKE_C_COMPILER_ID} compiler" )
  #add_definitions( --std=c99 )
#endif()

#https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_C_KNOWN_FEATURES.html
target_compile_features(ICGV PRIVATE c_std_11) #Compiler mode is at least C 11.

# -----------------------------------------------------
# (*) brew install glfw3 --HEAD
# (*) LunarG SDK Vulkan

#Finding and linking libpthread
set(PTHREAD_LIBS "")
include(CheckLibraryExists)
#-- Looking for pthread_once in pthread
check_library_exists(pthread pthread_once "" LIBPTHREAD)
if(LIBPTHREAD)
    list(APPEND PTHREAD_LIBS "pthread")
endif()
target_link_libraries(ICGV PUBLIC ${PTHREAD_LIBS})

#Finding and linking libm
if(APPLE)
  find_library(MATH_LIBRARY NAMES libm.dylib)
elseif(UNIX)
  find_library(MATH_LIBRARY NAMES libm.a)
endif()
if(MATH_LIBRARY)
  get_filename_component(MATH_LIB_DIR ${MATH_LIBRARY} PATH)
  link_directories(${MATH_LIB_DIR})
  message(STATUS "libm FOUND ${MATH_LIBRARY}")
  # Name the library both with a full path and as "-lm" to
  # activate the link type switching code for both cases.
  # If the second one links shared then the link will fail.
  set(MATH_LIBRARIES ${MATH_LIBRARY} -lm)
  
  target_link_libraries(ICGV PUBLIC ${MATH_LIBRARIES})
else()
  message(FATAL_ERROR "Cannot find libm.a needed for this test")
endif()


#Finding and linking GLFW3 (windowing, context & input)
find_package(GLFW3 REQUIRED)
if (GLFW3_FOUND)
    message(STATUS "GLFW3 FOUND")
    #message(STATUS "GLFW3_INCLUDE_DIRS in ${GLFW3_INCLUDE_DIR}")
    target_link_libraries(ICGV PUBLIC glfw)
endif (GLFW3_FOUND)

#DLG logging static link
add_library(DLG "libs/dlg/include/dlg/dlg.h" "libs/dlg/include/dlg/dlg.hpp" "libs/dlg/include/dlg/output.h" "libs/dlg/src/dlg/dlg.c")
target_link_libraries(ICGV PUBLIC DLG)

#Volk
# Set a suitable platform define to compile volk with.
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
  message(STATUS "CMAKE_SYSTEM_NAME = Windows, VOLK_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR")
  set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
  message(STATUS "CMAKE_SYSTEM_NAME = Linux, VOLK_STATIC_DEFINES VK_USE_PLATFORM_XLIB_KHR")
  set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_XLIB_KHR)
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
  message(STATUS "CMAKE_SYSTEM_NAME = Darwin, VOLK_STATIC_DEFINES VK_USE_PLATFORM_MACOS_MVK")
  set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_MACOS_MVK)
endif()
add_subdirectory(libs/volk volk)
target_link_libraries(ICGV PUBLIC volk)

#Finding and linking Vulkan
#https://cmake.org/cmake/help/latest/module/FindVulkan.html
find_package (Vulkan REQUIRED)
if (Vulkan_FOUND)
    message(STATUS "Vulkan FOUND")
    include_directories(${Vulkan_INCLUDE_DIRS})
    target_link_libraries (ICGV PUBLIC ${Vulkan_LIBRARIES})
    message(STATUS "Vulkan_INCLUDE_DIRS : " ${Vulkan_INCLUDE_DIRS})
    message(STATUS "Vulkan_LIBRARIES : " ${Vulkan_LIBRARIES})
endif (Vulkan_FOUND)

# -----------------------------------------------------
target_include_directories(ICGV
   PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libs/dlg/include"  # logging
   PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libs/dlg/src"      # logging
   )

#Debug with lldb
#lldb build/VulkanKore
#command alias bfl breakpoint set --file %1 --line %2
#bfl main.c 1840
#run next(step over), step (step in), continue
#fr v renderPassBeginInfo   #Show local variables
