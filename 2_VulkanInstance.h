
#pragma once // include guard


//# -----------------------------------------------------
// Step 2 - Initialize Vulkan
//
//________//________// START Variables and Functions before main function of this step
//Vulkan report/debug callback (validation layer)
VkDebugReportCallbackEXT debugReportCallback_Error      = VK_NULL_HANDLE;
VkDebugReportCallbackEXT debugReportCallback_Warning    = VK_NULL_HANDLE;
VkDebugUtilsMessengerEXT debugUtilsMessenger            = VK_NULL_HANDLE;

static VKAPI_ATTR VkBool32 VKAPI_CALL funcDebugUtilsMessenger
(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    dlg_error("validation layer: %s \n", pCallbackData->pMessage);
    return VK_FALSE;
}

static int validation_error = 0;
static VKAPI_ATTR VkBool32 VKAPI_CALL funcDebugReportCallback
(   VkFlags msgFlags, VkDebugReportObjectTypeEXT objType,
    uint64_t srcObject, size_t location, int32_t msgCode,
    const char *pLayerPrefix, const char *pMsg, void *pUserData)
{
    char *message = (char *)malloc(strlen(pMsg) + 100);
    assert(message);
    validation_error = 1;

    if (msgFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        //sprintf(message, "ERROR: [%s] Code %d : %s", pLayerPrefix, msgCode, pMsg);
        dlg_error("%s", message);
        dlg_error("ERROR: [%s] Code %d : %s", pLayerPrefix, msgCode, pMsg);
    } else if (msgFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        //sprintf(message, "WARNING: [%s] Code %d : %s", pLayerPrefix, msgCode, pMsg);
        dlg_warn("%s", message);
        dlg_warn("WARNING: [%s] Code %d : %s", pLayerPrefix, msgCode, pMsg);
    } else {
        return false;
    }

    //printf("%s\n", message);
    dlg_error("%s\n", message);
    fflush(stdout);
    free(message);

    /*
    * false indicates that layer should not bail-out of an
    * API call that had validation failures. This may mean that the
    * app dies inside the driver due to invalid parameter(s).
    * That's what would happen without validation layers, so we'll
    * keep that behavior here.
    */
    return false;
}
//________//________// END Variables and Functions before main function of this step

void SetupVulkanInstance(GLFWwindow*    windowHandle,
                         VkInstance*    outInstance,
                         VkSurfaceKHR*  outSurface)
{
    
    // VOLK won't compile if the appropriate Vulkan platform define isn't set.
    // Is this required because already done in CMAKE file
    void* ptr = 
    #if defined(_WIN32)
        &vkCreateWin32SurfaceKHR;
    #elif defined(__linux__) || defined(__unix__)
        &vkCreateXlibSurfaceKHR;
    #elif defined(__APPLE__)
        &vkCreateMacOSSurfaceMVK;
    #else
        //Platform not recogized for testing.
        NULL;
    #endif
    
    // Initialize VULKAN through VOLK
    // call the following function to initialize VOLK before calling any Vulkan APIs (including instance creation):
    VkResult result = volkInitialize();
    assert( VK_SUCCESS == result ); // fail the application if volkInitialize failed
    uint32_t version = volkGetInstanceVersion();
    dlg_info("Volk version : %u", version);
    dlg_info("Vulkan version %d.%d.%d initialized.", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));

    if (glfwVulkanSupported())
    {
        dlg_info("glfw Vulkan is available, at least for compute");
    }
    
    // Layer properties
    uint32_t count = 0;
    
    // Returns the number of layer properties available, 
    // If the VkLayerProperties*  is NULL, then the number of layer properties available is returned
    result =
      vkEnumerateInstanceLayerProperties (&count, // uint32_t*
                                          // pointer to the number of layer properties available
                                          NULL);  // VkLayerProperties*
                                          // pointer to an array of VkLayerProperties structures


    assert( VK_SUCCESS == result );
    assert( count > 0 );
    dlg_info("Number of layer properties available : %u", count);
    
    VkLayerProperties* instanceLayers = malloc( count * sizeof(VkLayerProperties) );
    // As the VkLayerProperties structure array is not NULL the function returns the layers properties
    //
    result =
      vkEnumerateInstanceLayerProperties(&count, // uint32_t*
                                         // pointer to the number of layer properties available
                                         &instanceLayers[0]); // VkLayerProperties*
                                         // pointer to an array of VkLayerProperties structures
    dlg_info("Layers properties available : %u", count);
    // Enumerate list of Layers
    for (uint32_t index = 0; index < count; index++) {
        dlg_info("Retrievable Layer name : %s", instanceLayers[index].layerName);
    }
    
    // Extension properties
    // vkEnumerateInstanceExtensionProperties - Returns the requested number
    // of global extension properties. The count relates to the number of
    // extension properties available
    result = 
      vkEnumerateInstanceExtensionProperties(NULL, // const char*
                                             // pointer to the name of the layer to retrieve extensions for
                                             &count, // uint32_t*
                                             // pointer to the number of extension properties available
                                             NULL); // VkExtensionProperties*
                                             // pointer to an array of VkExtensionProperties structures
    
    assert( VK_SUCCESS == result );
    assert( count > 0 );
    dlg_info("Extension Properties available : %u", count);
    
    VkExtensionProperties* instanceExtension = malloc( count * sizeof(VkExtensionProperties) );
    // Array of LayerNames not NULL so returns an array of null-terminated UTF-8 strings
    // names for the retrievable extensions.
    // The VkExtensionProperties structures is not NULL so returns the extension properties
    result =
      vkEnumerateInstanceExtensionProperties(NULL, // const char*
                                             // pointer to the name of the layer to retrieve extensions for
                                             &count, // uint32_t*
                                             // pointer to the number of extension properties available
                                             instanceExtension); // VkExtensionProperties*
                                             // pointer ro an array of VkExtensionProperties structures
    dlg_info("Names for the retrievable extensions : %u", count);
    
    for (uint32_t index = 0; index < count; index++) {
        //the function strcmp returns zero if the strings are the same,
        //less than zero if the first string is less than the second,
        //and greater than zero if the string array is greater.
        if (
            strcmp(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, instanceExtension[index].extensionName) == 0
            || strcmp(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, instanceExtension[index].extensionName) == 0
            )
        {
            dlg_info("Retrievable extension for debug : %s",instanceExtension[index].extensionName);
        } else {
            dlg_info("Retrievable extension name : %s", instanceExtension[index].extensionName);
        }
    }
    
    /*
       e.g., VK_LAYER_NV_optimus or VK_LAYER_LUNARG_standard_validation
    */
    
    const char *layers[] = { "VK_LAYER_KHRONOS_validation" }; //"VK_LAYER_NV_optimus" or "VK_LAYER_LUNARG_standard_validation"
    
    if (COUNT_ARRAY_ELEMS(layers) != 0) dlg_info("Size of Vulkan instance enabled layers : %u", COUNT_ARRAY_ELEMS(layers));
    
    /* Automatically check and list the required extensions for this Vulkan instance to start */
    uint32_t required_extension_count = 0;
    const char **required_extensions = glfwGetRequiredInstanceExtensions(&required_extension_count);
    if (!required_extensions) {
        dlg_error("glfwGetRequiredInstanceExtensions failed to find the "
                 "platform surface extensions.\n\nDo you have a compatible "
                 "Vulkan installable client driver (ICD) installed?\nPlease "
                 "look at the Getting Started guide for additional "
                 "information.\n"
                 "vkCreateInstance Failure");
    }
    // Now prepare the extensions list
    uint32_t enabled_extension_count=0;
    #ifdef ENABLE_VULKAN_DEBUG_CALLBACK // access debug callbacks
      // VK_EXT_debug_report + the other required extensions
      const char *extensions[2+required_extension_count];
      dlg_info("VK_EXT_debug_report + VK_EXT_debug_utils will be enabled because ENABLE_VULKAN_DEBUG_CALLBACK is defined");
      extensions[0] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME; // VK_EXT_debug_report
      extensions[1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME; // VK_EXT_debug_utils
      enabled_extension_count=2;//COUNT_ARRAY_ELEMS(extensions);
    #else
      enabled_extension_count=0;
      const char *extensions[required_extension_count];
    #endif
    
    // Build extensions list
    for ( uint32_t index=0; index < required_extension_count; index++) {
        extensions[enabled_extension_count] = required_extensions[index];
        enabled_extension_count++;
    }
    
    //List the enabled extensions
    dlg_info("Enabled extension count %u",enabled_extension_count);
    for (uint32_t index = 0;  index < enabled_extension_count;  index++)
    {
        dlg_info("Extensions no %u : %s",index,extensions[index]);
    }
    
    /* Example for MacOS *
    #ifdef ENABLE_VULKAN_DEBUG_CALLBACK // access debug callbacks
      const char *extensions[] = { "VK_EXT_debug_report",
                                   "VK_KHR_surface",
                                   "VK_EXT_metal_surface",
                                  };
    #else
      const char *extensions[] = { "VK_KHR_surface",
                                   "VK_EXT_metal_surface",
                                 };
    #endif
    */
    
    //--//--//--//
    {
        VkApplicationInfo  ai               = {};
        ai.sType                            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        ai.pApplicationName                 = "VulkanKore";
        ai.applicationVersion               = 1;
        ai.pEngineName                      = "VulkanKore";
        ai.engineVersion                    = 1;
        ai.apiVersion                       = VK_API_VERSION_1_2;
        
        VkInstanceCreateInfo  ici           = {};
        ici.sType                           = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        ici.flags                           = 0;
        ici.pApplicationInfo                = &ai;
        ici.enabledExtensionCount           = COUNT_ARRAY_ELEMS(extensions);
        ici.ppEnabledExtensionNames         = extensions;
        ici.enabledLayerCount               = COUNT_ARRAY_ELEMS(layers);
        ici.ppEnabledLayerNames             = layers;
        
        //Optional - if you want Vulkan to tell you if something wrong, you must set the callback
        /*
        What are validation layers?
        The Vulkan API is designed around the idea of minimal driver overhead and one of the manifestations of that goal is that there is very limited error checking in the API by default. Even mistakes as simple as setting enumerations to incorrect values or passing null pointers to required parameters are generally not explicitly handled and will simply result in crashes or undefined behavior. Because Vulkan requires you to be very explicit about everything you're doing, it's easy to make many small mistakes like using a new GPU feature and forgetting to request it at logical device creation time.
        
        However, that doesn't mean that these checks can't be added to the API. Vulkan introduces an elegant system for this known as validation layers. Validation layers are optional components that hook into Vulkan function calls to apply additional operations. Common operations in validation layers are:
        
        Checking the values of parameters against the specification to detect misuse
        Tracking creation and destruction of objects to find resource leaks
        Checking thread safety by tracking the threads that calls originate from
        Logging every call and its parameters to the standard output
        Tracing Vulkan calls for profiling and replaying
        Here's an example of what the implementation of a function in a diagnostics validation layer
        */
        #ifdef ENABLE_VULKAN_DEBUG_CALLBACK
            // Register your error logging function (defined at the top of the file)
            //PFN_vkCreateDebugReportCallbackEXT  vkCreateDebugReportCallbackEXT = NULL;
            //*(void **)& vkCreateDebugReportCallbackEXT        = vkGetInstanceProcAddr( *outInstance, "vkCreateDebugReportCallbackEXT");
            //assert(vkCreateDebugReportCallbackEXT);
            
            VkDebugReportCallbackCreateInfoEXT debugReportCreateInfo = {};
            debugReportCreateInfo.sType                              = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
            debugReportCreateInfo.flags                              = VK_DEBUG_REPORT_ERROR_BIT_EXT;
            debugReportCreateInfo.pfnCallback                        = funcDebugReportCallback;//&MyDebugReportCallback;
            
            // Setup error callback function notifications
            dlg_warn("HERE1");
            VkResult result = 
              vkCreateDebugReportCallbackEXT ( *outInstance,
                                                     // Valid VkIstance handle
                                                     &debugReportCreateInfo,
                                                     // pointer to a valid vkCreateDebugReportCallbackEXT structure
                                                     NULL,
                                                     // If pointer is not NULL then allocator callback manager
                                                     &debugReportCallback_Error);
                                                     // pointer to a VkCreateDebugReportCallbackEXT handle
            dlg_warn("HERE2");
            ERR_VULKAN_EXIT(result, "VkCreateDebugReportCallbackEXT(ERROR) failed " );
            
            // Capture warning as well as errors
            debugReportCreateInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
            debugReportCreateInfo.pfnCallback = funcDebugReportCallback;//&MyDebugReportCallback;
            
            // Setup warning callback function notifications
            result = 
              vkCreateDebugReportCallbackEXT ( *outInstance,
                                                     // Valid VkIstance handle
                                                     &debugReportCreateInfo,
                                                     // pointer to a valid vkCreateDebugReportCallbackEXT structure
                                                     NULL,
                                                     // If pointer is not NULL then allocator callback manager
                                                     &debugReportCallback_Warning);
                                                     // pointer to a VkCreateDebugReportCallbackEXT handle
            
            dlg_warn("HERE3");
            ERR_VULKAN_EXIT(result, "VkCreateDebugReportCallbackEXT(WARN) failed " );
            switch (result) {
            case VK_SUCCESS:
                break;
            case VK_ERROR_OUT_OF_HOST_MEMORY:
                ERR_EXIT("CreateDebugReportCallback Failure: out of host memory\n");
                break;
            default:
                ERR_EXIT("CreateDebugReportCallback Failure: unknown failure\n");
                break;
            }
            
            // Debug utils
            //PFN_VkDebugUtilsMessengerCreateInfoEXT  VkDebugUtilsMessengerCreateInfoEXT = NULL;
            //*(void **)& VkDebugUtilsMessengerCreateInfoEXT        = vkGetInstanceProcAddr( *outInstance, "VkDebugUtilsMessengerCreateInfoEXT");
            //assert(VkDebugUtilsMessengerCreateInfoEXT);
            /*
            VkDebugUtilsMessengerCreateInfoEXT debugUtilsCreateInfo;
            //debugUtilsCreateInfo = {};
            debugUtilsCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debugUtilsCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debugUtilsCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debugUtilsCreateInfo.pfnUserCallback = funcDebugUtilsMessenger;
           
            dlg_warn("test");
            
            result = 
              vkCreateDebugUtilsMessengerEXT(*outInstance, &debugUtilsCreateInfo, NULL, &debugUtilsMessenger);
            
            dlg_warn(" result : %d",result);
            ERR_VULKAN_EXIT(result, "failed to set up debug messenger!" );
            */
        
        #endif //END Vulkan debug callback
        
        //ici.pNext = &debugUtilsMessenger; // (VkDebugUtilsMessengerCreateInfoEXT*) &debugUtilsMessenger;
        ici.pNext = &debugReportCreateInfo;
        
        // vkCreateInstance verifies that the requested layers exists. If not,
        // vkCreateInstance will return VK_ERROR_LAYER_NOT_PRESENT
        result =
          vkCreateInstance( &ici,           //const VkInstanceCreateInfo*
                            // points to an instance of VkInstanceCreateInfo controlling creation
                            NULL,           //const VkAllocationCallbacks*
                            // controls host memory allocation
                            outInstance ); // VkInstance*
                            // pointer to a VkInstance handle for the returning resulting instance
        
        //assert( *outInstance!=NULL);     // This does not give much detail on failure reason
        if (result == VK_SUCCESS) { dlg_info("Vulkan initialized successfully !"); }
        else if (result == VK_ERROR_INCOMPATIBLE_DRIVER) {
            dlg_fatal("Cannot find a compatible Vulkan installable client driver (ICD).\n\n"
                      "Please look at the Getting Started guide for additional information.\n" \
                      "Failed to create vulkan instance.");
            assert( VK_SUCCESS == result ); // if result != VK_SUCCESS then make application fail & exit
        } else if (result == VK_ERROR_EXTENSION_NOT_PRESENT) {
            dlg_fatal("Cannot find a specified extension library" \
                      ".\nMake sure your layers path is set appropriately\n" \
                      "Failed to create vulkan instance.");
            assert( VK_SUCCESS == result ); // if result != VK_SUCCESS then make application fail & exit
        }else{ //*outInstance == NULL 
            dlg_fatal("Vulkan failed to initialize \n"  \
                      "vkCreateInstance failed.\n\nDo you have a compatible Vulkan " \
                      "installable client driver (ICD) installed?\nPlease look at " \
                      "the Getting Started guide for additional information.\n" \
                      "Failed to create vulkan instance.");
            assert( VK_SUCCESS == result ); // if result != VK_SUCCESS then make application fail & exit
        }
        
        
        //Vulkan Instance created, now make VOLK load it
        volkLoadInstance(*outInstance);
        
        // You need to define what type of window surface you'll be rendering into.
        // To create a VkSurfaceKHR object for window, this is different for each operating system and depend on every computer
        // Reason why we leave the work to GLFW.
        result = glfwCreateWindowSurface( *outInstance,     //VkInstance
                                          // Instance to associate with the surface
                                          windowHandle,     // GLFWwindow* windowHandle
                                          // Window handle
                                          NULL,             // const VkAllocationCallbacks*
                                          // allocator used for host memory allocated for the surface object
                                          outSurface);      // VkSurfaceKHR*
                                          // VkSurfaceKHR handle in which the created surface object is returned
        
        ERR_VULKAN_EXIT( result, "Could not create surface." );
        assert(outSurface!=NULL);
        
    }
    //--//--//--//
    
}// END SetupVulkanInstance(..)

