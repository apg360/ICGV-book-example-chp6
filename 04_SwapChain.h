
#pragma once // include guard

//# -----------------------------------------------------
// Step 4 - SetupSwapChain
//
//________//________// START Variables and Functions before main function of this step
//________//________// END Variables and Functions before main function of this step

void SetupSwapChain(VkDevice          device,
                    VkPhysicalDevice  physicalDevice,
                    VkSurfaceKHR      surface,
                    int*              outWidth,
                    int*              outHeight,
                    VkSwapchainKHR*   outSwapChain,
                    VkImage**         outPresentImages,
                    VkImageView**     outPresentImageViews)
{
        // To display something you'll need to create a set of render buffers.
        // These buffers and their properties are called Swap-Chain
        //--//--//--//
        {
            // Create Swap-Chain
            VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
            // You'll query the basic capabilities of the surface in order to create a swapchain
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice,           // physicalDevice
                                                      // physical device that will be associated with the swapchain to be created
                                                      surface,                 // surface
                                                      // surface that will be associated with the swapchain
                                                      &surfaceCapabilities);   // pSurfaceCapabilities
                                                      // pointer to the VkSurfaceCapabilitiesKHR structure with retrieved data
            VkExtent2D surfaceResolution   = surfaceCapabilities.currentExtent;
            *outWidth                      = surfaceResolution.width;
            *outHeight                     = surfaceResolution.height;
            
            dlg_info("surface resolution width : %u", *outWidth);
            dlg_info("surface resolution height : %u", *outHeight );
            
            VkSwapchainCreateInfoKHR ssci  = {};
            ssci.sType                     = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            ssci.surface                   = surface;
            // You'll use 2 for 'double' buffering
            ssci.minImageCount             = 2;
            ssci.imageFormat               = VK_FORMAT_B8G8R8A8_UNORM;
            ssci.imageColorSpace           = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
            ssci.imageExtent               = surfaceResolution;
            ssci.imageArrayLayers          = 1;
            ssci.imageUsage                = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            ssci.imageSharingMode          = VK_SHARING_MODE_EXCLUSIVE;
            ssci.preTransform              = 1;//VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
            ssci.compositeAlpha            = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            ssci.presentMode               = VK_PRESENT_MODE_MAILBOX_KHR;
            // If you want clipping outside the extents
            ssci.clipped                   = true;
            ssci.oldSwapchain              = NULL;
            
            VkResult result =
              vkCreateSwapchainKHR( device,         // device
                                   // VkDevice to associate the swapchain
                                   &ssci,           // pCreateInfo
                                   // pointer to VkSwapchainCreateInfoKHR structure with swapchain creation parameters
                                   NULL,            // pAllocator
                                   // optional allocator used for host memory
                                   outSwapChain );  // pSwapchain
                                   // resulting swapchain
            
            ERR_VULKAN_EXIT(result, "Failed to create swapchain." );
        }//END Create Swap-Chain
        //--//--//--//
        {
            // Create your images 'double' buffering
            uint32_t imageCount    =0;
            // You'll need to obtain the array of presentable images associated
            // with the swapchain you created. First, you pass in 'NULL' to 
            // obtain the number of images (i.e., should be 2)
            vkGetSwapchainImagesKHR( device,         // device
                                     //device associated with swapchain
                                     *outSwapChain,  // swapchain
                                     //swapchain to query
                                     &imageCount,    // pSwapchainImageCount
                                     // pointer to an integer related to the number of format pairs available
                                     NULL );         //pSwapchainImages
                                     // either NULL or a pointer to an array of VkSwapchainImageKHR structures
            assert(imageCount == 2); // If image count not equal 2, abort the software
            
            // this should be 2 for double-buffering
            *outPresentImages = malloc( imageCount * sizeof(VkImage) );
            
            // Obtain the presentable images and link them to
            // the images in the swapchain
            VkResult result =
              vkGetSwapchainImagesKHR( device,               // device
                                       // device associated with swapchain
                                       *outSwapChain,        // swapchain
                                       // swapchain to query
                                       &imageCount,          // pSwapchainImageCount
                                       // pointer to an integer related to the number of format pairs available
                                       *outPresentImages);  // pSwapchain
                                       // either NULL or a pointer to an array of VkSwapchainImageKHR structures
            ERR_VULKAN_EXIT( result, "Failed to create swap-chain images");
        }
        //--//--//--//
        {
            // You have 2 for double buffering
            *outPresentImageViews = malloc( 2 * sizeof(VkImageView) );
            for(uint32_t index = 0; index < 2; ++index )
            {
                // Create VkImageViews for your swap-chain
                // VkImages buffers
                VkImageViewCreateInfo ivci                = {};
                ivci.sType                                = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                ivci.viewType                             = VK_IMAGE_VIEW_TYPE_2D;
                ivci.format                               = VK_FORMAT_B8G8R8A8_UNORM;
                ivci.components.r                         = VK_COMPONENT_SWIZZLE_R;
                ivci.components.g                         = VK_COMPONENT_SWIZZLE_G;
                ivci.components.b                         = VK_COMPONENT_SWIZZLE_B;
                ivci.components.a                         = VK_COMPONENT_SWIZZLE_A;
                ivci.subresourceRange.aspectMask          = VK_IMAGE_ASPECT_COLOR_BIT;
                ivci.subresourceRange.baseMipLevel        = 0;
                ivci.subresourceRange.levelCount          = 1;
                ivci.subresourceRange.baseArrayLayer      = 0;
                ivci.subresourceRange.layerCount          = 1;
                ivci.image                                = (*outPresentImages)[index];
                
                // Create an image view from an existing image
                VkResult result =
                  vkCreateImageView( device,               // device
                                     // logical device that creates the image view
                                     &ivci,                // pCreateInfo
                                     // pointer to instance of the VkImageViewCreateInfo structure containing parameters for the image view
                                     NULL,                 // pAllocator
                                     // optional controls host memory allocation
                                     &(*outPresentImageViews[1])  );
                                     // pointer to VkImageView handle for returned image view object
                ERR_VULKAN_EXIT( result, "Could not create ImageView.");
            }//END for loop
            
        }
        //--//--//--//
        
}//END SetupSwapChain(..)
