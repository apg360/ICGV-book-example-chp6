
#pragma once // include guard

//# -----------------------------------------------------
// Step 3 - Initialize Vulkan
//
//________//________// START Variables and Functions before main function of this step
//________//________// END Variables and Functions before main function of this step

void SetupPhysicalDevice(VkInstance           instance,
                         VkPhysicalDevice*    outPhysicalDevice,
                         VkDevice*            outDevice)
{
        // Query how many devices are present in the system
        uint32_t deviceCount = 0;
        // How many devices are present in the system and that support/accessible to Vulkan instance
        // The instance is the handle to a Vulkan instance you previously created with vkCreateInstance.
        // The VkPhysicalDevice pointer can be either NULL or a pointer to an array of VkPhysicalDevice handles.
        VkResult result =
          vkEnumeratePhysicalDevices(instance,      // VkInstance
                                     // handle to a Vulkan instance previously created with vkCreateInstance
                                     &deviceCount,  // uint32_t*
                                     // pointer to an integer related to the number of physical devices available or queried
                                     NULL);         // VkPhysicalDevice*
                                     // either NULL or pointer to an array of VkPhysicalDevice handles
        
        ERR_VULKAN_EXIT( result, "Failed to query the number of physical devices present." );
        
        // There has to be at least one device present
        if(deviceCount == 0) { ERR_EXIT("Couldn't detect any device present with Vulkan support"); } 
           else { dlg_info("GPU device with vulkan support : %u",deviceCount); };
        
        // Prepare physical devices array list
        VkPhysicalDevice* physicalDevices = malloc( deviceCount * sizeof(VkPhysicalDevice) );
        dlg_info("VkPhysicalDevice number : %u",COUNT_ARRAY_ELEMS(physicalDevices));
        assert(COUNT_ARRAY_ELEMS(physicalDevices)>0);
        
        // Gets the VkPhysicalDevice handle of the first device
        result =
          vkEnumeratePhysicalDevices(instance,                // VkInstance
                                     // handle to a Vulkan instance previously created with vkCreateInstance
                                     &deviceCount,            // uint32_t*
                                     // pointer to an integer related to the number of physical devices available or queried
                                     &physicalDevices[0]);    // VkPhysicalDevice*
                                     // either NULL or a pointer to an array of VkPhysicalDevice handles
        
        ERR_VULKAN_EXIT(result, "Failed to enumerate physical devices present" );
        
        // Use the first available device
        *outPhysicalDevice = physicalDevices[0];
        
        // Enumerate all physical devices and print out the details
        for (uint32_t index = 0; index < deviceCount; ++index)
        {
            VkPhysicalDeviceProperties deviceProperties;
            memset(&deviceProperties, 0, sizeof deviceProperties);
            
            //Gets the properties of a physical device
            vkGetPhysicalDeviceProperties(physicalDevices[index], // physicalDevice
                                          // handle to the physical device whose properties will be queried
                                          &deviceProperties);     // pProperties
                                          // pointer to VkPhysicalDeviceProperties structure, that is filled with information
            
            dlg_info("Driver Version: %d", deviceProperties.driverVersion);
            dlg_info("Device Name:    %s", deviceProperties.deviceName);
            dlg_info("Device Type:    %d", deviceProperties.deviceType);
            dlg_info("API Version:    %d.%d.%d", 
              (deviceProperties.apiVersion>>22)&0x3FF,
              (deviceProperties.apiVersion>>12)&0x3FF,
              (deviceProperties.apiVersion&0xFFF)
            );
        }// END Enumerate all physical devices
        
        // Fill up the physical device memory properties:
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties( *outPhysicalDevice,
                                             // handle to the device query
                                             &memoryProperties);
                                             // pointer to VkPhysicalDeviceMemoryProperties structure returned with properties
        // Here's where you initialize your queues
        // You'll discuss queues next - however, you need to specify the queue
        // details for the device creation info
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        // Use the first queue family in the family list
        queueCreateInfo.queueFamilyIndex   = 0;
        queueCreateInfo.queueCount         = 1;
        float queuePriorities[]            = { 1.0f };
        queueCreateInfo.pQueuePriorities   = queuePriorities;
        
        // Same extension you specified when initializing Vulkan
        const char *deviceExtensions[]     = { "VK_KHR_swapchain" };
        const char *layers[]               = { "VK_LAYER_KHRONOS_validation" }; // "VK_LAYER_LUNARG_standard_validation" or "VK_LAYER_NV_optimus"
        
        //
        VkDeviceCreateInfo dci             = {};
        dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        // Set queue info on your device
        dci.queueCreateInfoCount           = 1;
        dci.pQueueCreateInfos              = &queueCreateInfo;
        dci.enabledExtensionCount          = COUNT_ARRAY_ELEMS(deviceExtensions);
        dci.ppEnabledExtensionNames        = deviceExtensions;
        dci.enabledLayerCount              = COUNT_ARRAY_ELEMS(layers);
        dci.ppEnabledLayerNames            = layers;
        
        //
        VkPhysicalDeviceFeatures features  = {};
        features.shaderClipDistance        = VK_TRUE;
        dci.pEnabledFeatures               = &features;
        
        // Ideally, you'd want to enumerate and find the best device,
        // however for this example we use only the first device 'physicalDevices[0]',
        // which you stored in the previous section
        result =
          vkCreateDevice(*outPhysicalDevice, // physicalDevice
                         // valid handles returned from vkEnumeratePhysicalDevices
                         &dci,                // pCreateInfo
                         // pointer to a VkDeviceCreateInfo structure containing device data
                         NULL,                // pAllocator
                         // optional control of host memory allocation
                         outDevice);          // pDevice
                         // pointer to a handle in which the created VkDevice is returned
        
        ERR_VULKAN_EXIT( result, "Failed to create logical device !" );
        
        // VOLK
        //allows you to get quick gains without changing your code, but isn’t suitable for applications that want to use explicit multi-GPU
        volkLoadDevice(*outDevice);
        
        
        //Cleanup (for every "malloc" there must be a "free"
        free(physicalDevices);
        
}// END SetupPhysicalDevice(..)
