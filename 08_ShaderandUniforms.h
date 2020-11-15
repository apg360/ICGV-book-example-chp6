
#pragma once // include guard

//# -----------------------------------------------------
// Step 8 - SetupShaderandUniforms
//
//________//________// START Variables and Functions before main function of this step
//________//________// END Variables and Functions before main function of this step

void SetupShaderandUniforms(VkDevice          device,
                            VkPhysicalDevice  physicalDevice,
                            int               width,
                            int               height,
                            VkShaderModule*   outVertShaderModule,
                            VkShaderModule*   outFragShaderModule,
                            VkBuffer*         outBuffer,
                            VkDeviceMemory*   outMemory)
{
    {
        // Simple Shaders (Vertex & Fragment)
        uint32_t codeSize = 0;
        // assume your file is less than 10,000 bytes
        char *code        = (char*) malloc( sizeof(char[10000]) );
        FILE* fileHandle  = NULL;
        
        // see Section 6.11 for details on the shaders load your vertex shaders :
        // (see Appendix for the shader text readable version)
        fileHandle = fopen( "vert.spv", "rb" );
        
        // did you successfully find file
        if (fileHandle == NULL) ERR_EXIT("Failed to open shader file." );
        
        // read the file contents
        codeSize = fread(code, 1, 10000, fileHandle);
        // close the file
        fclose(fileHandle);
        fileHandle = NULL;
        
        VkShaderModuleCreateInfo vertexShaderCreationInfo = {};
        vertexShaderCreationInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vertexShaderCreationInfo.codeSize                 = codeSize;
        vertexShaderCreationInfo.pCode                    = (uint32_t *) code;
        
        VkResult result =
          vkCreateShaderModule( device,
                                // device is the logical device that creates the shader module
                                &vertexShaderCreationInfo,
                                // pointer the VkShaderModuleCreateInfo structure
                                NULL,
                                // optional controling host memory allocation
                                outVertShaderModule );
                                // pointer to VkShaderModule handle which the shader module object is returned
        
        ERR_VULKAN_EXIT( result, "Failed to create vertex shader module." );
        
        // load your fragment shader:
        fileHandle = fopen( "frag.spv", "rb" );
        
        // did you successfully find file
        if (fileHandle == NULL) ERR_EXIT("Failed to open shader frag file." );
        
        // read the file contents
        codeSize = fread(code, 1, 10000, fileHandle);
        // close the file
        fclose(fileHandle);
        fileHandle = NULL;
        
        VkShaderModuleCreateInfo fragmentShaderCreationInfo = {};
        fragmentShaderCreationInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        fragmentShaderCreationInfo.codeSize                 = codeSize;
        fragmentShaderCreationInfo.pCode                    = (uint32_t *) code;
        
        result =
          vkCreateShaderModule( device,
                                // device is the logical device that creates the shader module
                                &fragmentShaderCreationInfo,
                                // pointer the VkShaderModuleCreateInfo structure
                                NULL,
                                // optional controling host memory allocation
                                outFragShaderModule );
                                // pointer to VkShaderModule handle which the shader module object is returned
        
        ERR_VULKAN_EXIT( result, "Failed to create vertex shader module." );
        
        free(code);
        code=NULL;
    }
    
    {
        // Create 'uniform' buffer for passing constant data to the shader (connecting shader with the data)
        const float PI     = 3.14159265359f;
        const float TORAD  = PI/180.0f;
        
        // perspective projection parameters:
        float fov          = 45.0f;
        float nearZ        = 0.1f;
        float farZ         = 1000.0f;
        float aspectRatio  = width / (float) height;
        float t            = 1.0f / tanf( fov * TORAD * 0.5f );
        float nf           = nearZ - farZ;
        
        // simple matrices (model/view/projection)
        float lprojectionMatrix[16] =
        {
            t / aspectRatio,   0,    0,                     0,
            0,                 t,    0,                     0,
            0,                 0,    (-nearZ-farZ) / nf,    (2*nearZ*farZ) / nf,
            0,                 0,    1,                     0
        };
        
        // identity matrix
        float lviewMatrix[16] = 
        {
            1,  0,  0,  0,
            0,  1,  0,  0,
            0,  0,  1,  0,
            0,  0,  0,  1
        };
        
        //position of the object
        float xp =  0;
        float yp =  2;
        float zp = 10;
        
        // matrix/transform
        float lmodelMatrix[16] = 
        {
            1,  0,  0,  xp,
            0,  1,  0,  yp,
            0,  0,  0,  zp,
            0,  0,  0,  1
        };
        
        // create your uniforms buffers:
        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        // size in bytes ( 3 matrices containing 4x4=16 floats)
        bufferCreateInfo.size               = (float) sizeof( 16 * 3 );
        bufferCreateInfo.usage              = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        bufferCreateInfo.sharingMode        = VK_SHARING_MODE_EXCLUSIVE;
        
        VkResult result =
          vkCreateBuffer( device,
                          // logical device that creates the buffer object
                          &bufferCreateInfo,
                          // pointer to returned VkBufferCreateInfo structure containing buffer creation info
                          NULL,
                          // optional controling host memory allocation
                          outBuffer );
                          // pointer to returned VkBuffer handle object
        
        ERR_VULKAN_EXIT( result, "Failed to create uniforms buffer." );
        
        // allocate memoory for buffer:
        VkMemoryRequirements bufferMemoryRequirements = {};
        vkGetBufferMemoryRequirements( device,
                                       *outBuffer,
                                       &bufferMemoryRequirements );
        
        VkMemoryAllocateInfo matrixAllocateInfo = {};
        matrixAllocateInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        matrixAllocateInfo.allocationSize       = bufferMemoryRequirements.size;
        
        VkPhysicalDeviceMemoryProperties memoryProperties;
          vkGetPhysicalDeviceMemoryProperties( physicalDevice, &memoryProperties );
        
        for ( uint32_t index = 0; index < VK_MAX_MEMORY_TYPES; ++index )
        {
            VkMemoryType memoryType = memoryProperties.memoryTypes[index];
            // is this the memory type you are looking for ?
            if ( ( memoryType.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT ) )
            {
                // save location
                matrixAllocateInfo.memoryTypeIndex = index;
                break;
            }
        }// END for loop index
        
        result = vkAllocateMemory ( device,
                                    &matrixAllocateInfo,
                                    NULL,
                                    outMemory );
        
        ERR_VULKAN_EXIT( result, "Failed to allocate uniforms buffer memory." );
        
        result = vkBindBufferMemory( device,
                                     *outBuffer,
                                     *outMemory,
                                     0 );
        
        ERR_VULKAN_EXIT( result, "Failed to bind uniforms buffer memory." );
        
        // set buffer content:
        void *matrixMapped = NULL;
        result = vkMapMemory( device,
                              // logical device that owns the memory
                              *outMemory,
                              // VkDeviceMemory object to be mapped
                              0,
                              // zero-based byte offset from the beginning of the memory object
                              VK_WHOLE_SIZE,
                              // size of the memory range to map
                              0,
                              // reserved for furture use
                              &matrixMapped );
                              // pointer in which is returned a host-accessible pointer to the beginning of the mapped range
        
        ERR_VULKAN_EXIT( result, "Failed to map uniform buffer memory." );
        
        memcpy( matrixMapped, &lprojectionMatrix[0], sizeof( lprojectionMatrix ) );
        memcpy( ((float *)matrixMapped + 16), &lviewMatrix[0], sizeof( lviewMatrix ) );
        memcpy( ((float *)matrixMapped + 32), &lmodelMatrix[0], sizeof( lmodelMatrix ) );
        
        vkUnmapMemory(device, *outMemory );
        
    }
    
}// END SetupShaderandUniforms(..)
