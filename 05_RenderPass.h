
#pragma once // include guard

//#define DEPTH_BUFFER

//# -----------------------------------------------------
// Step 5 - SetupRenderPass
//
//________//________// START Variables and Functions before main function of this step
//________//________// END Variables and Functions before main function of this step

void SetupRenderPass(VkDevice          device,
                     VkPhysicalDevice  physicalDevice,
                     int               width,
                     int               height,
                     VkImageView*      presentImageViews,
                     VkRenderPass*     outRenderPass,
                     VkFramebuffer**   outFrameBuffers)
{
        dlg_warn("Welcome SetupRenderPass");
        // The render-pass defines the role of framebuffer resources
        
        
        //Possibilities :
        // (*) You can have more than one pass with each pass (subpass) defining which framebuffer resource to use.
        // (*) The command-buffer in the next section can be used for several render-passes
        // (*) You can also use a single command-buffer to draw a whole frames with the multiple passes contributing to techniques like
        //          shadow mapping and post-processing. Managing these process more efficiently
        //
        // Render-pass might seem like additional work, but for more complex scenes the render-pass gives you additional screen control.
        // i.e. : post-processing and deferred rendering, mapping specific regions or order of processing to different threads/GPUs
        
        
        // Below we sets a basic fullscreen render-pass. One display update with no sub-passes
        
        
        // Frame buffer
        // define your attachment points
        #ifdef DEPTH_BUFFER
          dlg_info("DEPTH_BUFFER is defined !");
          // Extension (Depth Buffer)
          VkImage               depthImage           = NULL;
          VkImageView           depthImageView       = NULL;
          
          {
              // Create a depth image:
              VkImageCreateInfo imageCreateInfo      = {};
              imageCreateInfo.sType                  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
              imageCreateInfo.imageType              = VK_IMAGE_TYPE_2D;
              imageCreateInfo.format                 = VK_FORMAT_D16_UNORM;
              VkExtent3D ef                          = { width, height, 1 };
              imageCreateInfo.extent                 = ef;
              imageCreateInfo.mipLevels              = 1;
              imageCreateInfo.arrayLayers            = 1;
              imageCreateInfo.samples                = VK_SAMPLE_COUNT_1_BIT;
              imageCreateInfo.tiling                 = VK_IMAGE_TILING_OPTIMAL;
              imageCreateInfo.usage                  = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
              imageCreateInfo.sharingMode            = VK_SHARING_MODE_EXCLUSIVE;
              imageCreateInfo.queueFamilyIndexCount  = 0;
              imageCreateInfo.pQueueFamilyIndices    = NULL;
              imageCreateInfo.initialLayout          = VK_IMAGE_LAYOUT_UNDEFINED;
              
              // Create a new image object for your depth buffer
              VkResult result =
                vkCreateImage( device,            // device
                               // logical device that creates the image
                               &imageCreateInfo,  // pCreateInfo
                               // pointer to VkImageCreateInfo structure with parameters for the created image
                               NULL,              // pAllocator
                               // optional control host memory allocation
                               &depthImage );     // pImage
                               // pointer to VkImage handle returned image object
              ERR_VULKAN_EXIT(result, "Failed to create depth image.");
              
              VkMemoryRequirements memoryRequirements = {};
              vkGetImageMemoryRequirements( device,                 // device
                                            // logical device that owns the image
                                            depthImage,             // image
                                            // image to query
                                            &memoryRequirements );  // pMemoryRequirements
                                            // instance pointer to VkMemoryRequirements structure returned memory requirements
              
              // memoryRequirements contains memoryTypeBits member which is a bitmask
              // each one of the bits is set for every supported memory type for the resource
              // Bit i is set if and only if the memory type i in the VkPhysicalDeviceMemoryProperties structure for the physical device is supported for the resource.
              
              // Allocate memory for your depth buffer
              VkMemoryAllocateInfo imageAllocateInfo = {};
              imageAllocateInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
              imageAllocateInfo.allocationSize       = memoryRequirements.size;
              
              // memoryTypeBits is a bitfield where if bit i is set, 
              // it means that the VkMemoryType i of the VkPhysicalDeviceMemoryProperties structure satisfies the memory requirements :
              // read the device memory properties
              VkPhysicalDeviceMemoryProperties memoryProperties;
              vkGetPhysicalDeviceMemoryProperties( physicalDevice,
                                                   // handle to the device to query.
                                                   &memoryProperties );
                                                   // returned pointer to instance of VkPhysicalDeviceMemoryProperties structure
              
              uint32_t memoryTypeBits = memoryRequirements.memoryTypeBits;
              for( uint32_t index = 0; index < VK_MAX_MEMORY_TYPES; ++index )
              {
                  VkMemoryType memoryType = memoryProperties.memoryTypes[index];
                  if( memoryTypeBits & 1 )
                  {
                      if( ( memoryType.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ) )
                      {
                          // save index
                          imageAllocateInfo.memoryTypeIndex = index;
                          break;
                      }
                  }
                  memoryTypeBits = memoryTypeBits >> 1;
              }// END FOR LOOP index
              
              VkDeviceMemory imageMemory = { 0 };
              result = vkAllocateMemory( device,
                                         // logical device that owns the memory
                                         &imageAllocateInfo,
                                         // pointer to VkMemoryAllocateInfo structure describing parameters of the allocation
                                         NULL,
                                         // optional control host memory allocation
                                         &imageMemory );
                                         // pointer to returned VkDeviceMemory handle with information about the allocated memory
              
              ERR_VULKAN_EXIT( result, "Failed to allocate device memory." );
              
              result = vkBindImageMemory( device,
                                          // logical device that owns the image and memory
                                          depthImage,
                                          // image to bind
                                          imageMemory, 0 );
                                          // start offset of the region of memory which is to be bound to the image
              
              ERR_VULKAN_EXIT( result, "Failed to bind image memory." );
              
              // create the depth image view:
              VkImageAspectFlags aspectMask                             = VK_IMAGE_ASPECT_DEPTH_BIT;
              VkImageViewCreateInfo imageViewCreateInfo                 = {};
              imageViewCreateInfo.sType                                 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
              imageViewCreateInfo.image                                 = depthImage;
              imageViewCreateInfo.viewType                              = VK_IMAGE_VIEW_TYPE_2D;
              imageViewCreateInfo.format                                = imageViewCreateInfo.format;
              VkComponentMapping g                                      = { VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                            VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                            VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                            VK_COMPONENT_SWIZZLE_IDENTITY
                                                                          };
              imageViewCreateInfo.components                            = g;
              imageViewCreateInfo.subresourceRange.aspectMask           = aspectMask;
              imageViewCreateInfo.subresourceRange.baseMipLevel         = 0;
              imageViewCreateInfo.subresourceRange.levelCount           = 1;
              imageViewCreateInfo.subresourceRange.baseArrayLayer       = 0;
              imageViewCreateInfo.subresourceRange.layerCount           = 1;
              result =
                vkCreateImageView( device,
                                   // logical device that creates the image view
                                   &imageViewCreateInfo,
                                   // pointer to instance of the VkImageViewCreateInfo structure containing parameters for created image view
                                   NULL,
                                   // optional control host memory allocation
                                   &depthImageView );
                                   // pointer to returned VkImageView handle object
              
              ERR_VULKAN_EXIT( result, "Failed to create image view." );
          }
        #endif // DEPTH_BUFFER
        
        // 0 - color screen buffer
        VkAttachmentDescription pass[1]     = {};
        pass[0].format                      = VK_FORMAT_B8G8R8A8_UNORM;
        pass[0].samples                     = VK_SAMPLE_COUNT_1_BIT;
        pass[0].loadOp                      = VK_ATTACHMENT_LOAD_OP_CLEAR;
        pass[0].storeOp                     = VK_ATTACHMENT_STORE_OP_STORE;
        pass[0].stencilLoadOp               = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        pass[0].stencilStoreOp              = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        pass[0].initialLayout               = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        pass[0].finalLayout                 = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        
        #ifdef DEPTH_BUFFER
          VkAttachmentDescription pass[2]     = {};
          // 1 - depth buffer
          pass[1].format                      = VK_FORMAT_D16_UNORM;
          pass[1].samples                     = VK_SAMPLE_COUNT_1_BIT;
          pass[1].loadOp                      = VK_ATTACHMENT_LOAD_OP_CLEAR;
          pass[1].storeOp                     = VK_ATTACHMENT_STORE_OP_DONT_CARE;
          pass[1].stencilLoadOp               = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
          pass[1].stencilStoreOp              = VK_ATTACHMENT_STORE_OP_DONT_CARE;
          pass[1].initialLayout               = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
          pass[1].finalLayout                 = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        #endif
        
        VkAttachmentReference car           = {};
        car.attachment                      = 0;
        car.layout                          = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        
        // create the one main subpass of your renderpass:
        VkSubpassDescription subpass        = {};
        subpass.pipelineBindPoint           = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount        = 1;
        subpass.pColorAttachments           = &car;
        subpass.pDepthStencilAttachment     = NULL;
        
        #ifdef DEPTH_BUFFER
          VkAttachmentReference dar         = {};
          dar.attachment                    = 1;
          dar.layout                        = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
          subpass.pDepthStencilAttachment   = &dar;
        #endif
        
        // create your main renderpass
        VkRenderPassCreateInfo rpci         = {};
        rpci.sType                          = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        rpci.attachmentCount                = COUNT_ARRAY_ELEMS(pass); // color (+depth if def DEPTH_BUFFER)
        rpci.pAttachments                   = pass;
        rpci.subpassCount                   = 1;
        rpci.pSubpasses                     = &subpass;
        
        dlg_warn("before vkCreateRenderPass");
        VkResult result =
          vkCreateRenderPass ( device,
                               // logical device that creates the render pass
                               &rpci,
                               // pointer to VkRenderPassCreateInfo structure describing parameters of the render pass
                               NULL,
                               // optional host memory allocation control
                               outRenderPass );
                               // pointer VkRenderPass handle in which the resulting render pass object is returned
        
        ERR_VULKAN_EXIT( result, "Failed to create renderpass" );
        dlg_warn("after vkCreateRenderPass");
        
        #ifdef DEPTH_BUFFER
          VkImageView frameBufferAttachments[2] = {0};
        #else
          VkImageView frameBufferAttachments[1] = {0};
        #endif
        // create your frame buffers:
        VkFramebufferCreateInfo fbci  = {};
        fbci.sType                    = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fbci.renderPass               = *outRenderPass;
        // must be equal to the attachment count on render pass : rpci.attachmentCount
        fbci.attachmentCount          = rpci.attachmentCount;
        fbci.pAttachments             = frameBufferAttachments;
        fbci.width                    = width;
        fbci.height                   = height;
        fbci.layers                   = 1;
        
        // Create a framebuffer per swap-chain imageView:
        *outFrameBuffers = malloc( 2 * sizeof(VkFramebuffer) );
        for( uint32_t index = 0; index < 2; ++index )
        {
            frameBufferAttachments[0] = presentImageViews[index];
            #ifdef DEPTH_BUFFER
              frameBufferAttachments[1] = depthImageView;
            #endif
            // Create a new framebuffer object
            dlg_warn("before vkCreateFramebuffer");
            result =
              vkCreateFramebuffer( device,                     // device
                                   // logical device that creates the framebuffer
                                   &fbci,                      // pCreateInfo
                                   // pointer to VkFramebufferCreateInfo structure describing framebuffer creation
                                   NULL,                       // pAllocator
                                   // optional control of host memory allocation
                                   &(*outFrameBuffers)[index]  // pFramebuffer
                                   // pointer to returned VkFramebuffer handle for the framebuffer object
                                   );
            dlg_warn("after vkCreateFramebuffer");
            ERR_VULKAN_EXIT( result, "Failed to create framebuffer." );
        }//END FOR LOOP index
    
}//END SetupRenderPass(..)

