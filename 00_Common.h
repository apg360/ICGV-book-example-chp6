
#pragma once // include guard

#include <stdio.h>
#include <stdlib.h>     // EXIT_FAILURE, EXIT_SUCCESS
#include <assert.h>     // C++ DBG_ASSERT become assert
#include <string.h>     // strcpy, strcmp, strcat(concatenates), etc...
#include <dlg/dlg.h>    // logging
#include <math.h>       // tanf


/*
#include <volk.h>
//#define VK_EXT_metal_surface //VK_USE_PLATFORM_MACOS_MVK
//#define VK_NO_PROTOTYPES
//#define GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
*/

//Note that to avoid symbol conflicts, you have to make sure all translation units in your application include volk.h instead of vulkan.h, 
//  or that you define VK_NO_PROTOTYPES project-wide to make sure you aren’t accidentally picking up symbols from the real Vulkan loader. 
#define VK_NO_PROTOTYPES

// Vulkan backtrace error
#define ENABLE_VULKAN_DEBUG_CALLBACK

// If you instead want to include the Vulkan header from a custom location
// Or use your own custom Vulkan header then do this before the GLFW header.
// => https://www.glfw.org/docs/3.2/vulkan.html
#include <volk.h>
#include <GLFW/glfw3.h>


//Check vulkan error and exit application
#define ERR_VULKAN_EXIT(val, err_msg)               \
    if ( val != VK_SUCCESS ) {                      \
        dlg_fatal(err_msg);                         \
        fflush(stdout);                             \
        exit(1);                                    \
    }

/*//Assert value and exit application
#define ERR_ASSERT_EXIT(val1, val2, err_msg)    \
    if ( !( assert(val1 == val2) ) ) {                     \
        dlg_fatal(err_msg);                                \
        fflush(stdout);                                    \
        assert(false);                                     \
    }
*/

//Error and exit application
#define ERR_EXIT(err_msg)                           \
    do {                                            \
        dlg_fatal(err_msg);                         \
        fflush(stdout);                             \
        exit(1);                                    \
    } while (0)

// This is a universal formula to find number of elements present in an array
//   it will work for arrays of all data types such as int, char, float etc.
// We must use define instead of a function. Because An array sent as a parameter to a function is treated as a pointer, 
//   so sizeof will return the pointer's size, instead of the array's. : void printSizeOf(int intArray[])
#define COUNT_ARRAY_ELEMS(x) (int) (sizeof(x) / sizeof((x)[0]))


/*
    dlg_warn("This is a warning. If on a console, it will be printed yellow");
    dlg_error("Errors are red. Colors work even on windows consoles");
    dlg_info("Another feature: Utf-8 printing works automatically, even for שׁǐŉďốẅś consoles");
    dlg_fatal("This one is printed bold. For more information, read the example already");
    dlg_assertm(1 == 2, "Well, this assertion will probably %s...", "fail");
    dlg_infot(("tag1", "tag2"), "We can tag our stuff. Can be used to filter/redirect messages");
    dlg_asserttm(("tag3"), 3 == 2, "The same goes for asserts");
*/
