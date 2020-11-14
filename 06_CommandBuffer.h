
#pragma once // include guard

//# -----------------------------------------------------
// Step 6 - SetupCommandBuffer
//
//________//________// START Variables and Functions before main function of this step
//________//________// END Variables and Functions before main function of this step

void SetupCommandBuffer(VkDevice          device,
                        VkPhysicalDevice  physicalDevice,
                        VkCommandBuffer*  outCommandBuffer)
{
     // Give your device some commands (orders)
    {
        // You can now get the device queue you will be submitting work to:
        VkQueue presentQueue = NULL;
        vkGetDeviceQueue( device,
                          // logical device that owns the queue
                          0,
                          // index of the queue family to which the queue belongs
                          0,
                          // index within this queue family of the queue to retrieve
                          &presentQueue );
                          // pointer to a VkQueue object filled with the handle for the requested queue
        
        // Create your command buffers:
        VkCommandPoolCreateInfo cpci = {};
        cpci.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        cpci.flags                   = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        cpci.queueFamilyIndex        = 0;
        
        VkCommandPool commandPool;
        VkResult result =
          vkCreateCommandPool( device,
                               // logical device that creates the command pool
                               &cpci,
                               // contains information used to create the command pool
                               NULL,
                               // controls host memory allocation (if needed)
                               &commandPool );
                               // pointer to VkCommandPool handle in which the created pool is returned
        
        ERR_VULKAN_EXIT( result, "Failed to create command pool." );
        
        VkCommandBufferAllocateInfo cbai = {};
        cbai.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cbai.commandPool                 = commandPool;
        cbai.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        cbai.commandBufferCount          = 1;
        
        // Allocate command buffers from the command pool you just created
        result =
          vkAllocateCommandBuffers( device,          // device
                                    // logical device that owns the command pool
                                    &cbai,           // pAllocateInfo
                                    // pointer to VkCommandBufferAllocateInfo structure describing allocation
                                    outCommandBuffer );
                                    // pointer to an array of returned VkCommandBuffer handles
        
        ERR_VULKAN_EXIT( result, "Failed to allocate draw command-buffer." );
        
    }
}//END SetupCommandBuffer(..)
