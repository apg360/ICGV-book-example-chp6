
#pragma once // include guard

//# -----------------------------------------------------
// Step 7 - SetupVertexBuffer
//
//________//________// START Variables and Functions before main function of this step
//________//________// END Variables and Functions before main function of this step

void SetupVertexBuffer(VkDevice          device,
                       VkPhysicalDevice  physicalDevice,
                       int*              outVertexSize,
                       int*              outNumberOfTriangles,
                       VkBuffer*         outVertexInputBuffer)
{
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const float verticesForCube[] =
    { // see figure 6.9 for details
        -1.0f,-1.0f,-1.0f,  // triangle 1 : begin
        -1.0f,-1.0, 1.0f,
        -1.0f, 1.0f, 1.0f,  // triangle 1 : end
        1.0f, 1.0f,-1.0f,    // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,  // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
    
    // Vertex info
    struct vertex
    {
        float x, y, z, w;  // position
        float r, g, b;     // color
    };
    
    *outVertexSize = sizeof(struct vertex);
    
    {
        const int numberOfVertices = sizeof( verticesForCube ) / (sizeof( float ) * 3);
        *outNumberOfTriangles = numberOfVertices / 3;
        
        // create your vertex buffer:
        VkBufferCreateInfo vertexInputBufferInfo       = {};
        vertexInputBufferInfo.sType                    = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        // size in bytes of your data
        // a single triangle requires 3 vertices
        vertexInputBufferInfo.size                     = sizeof(struct vertex) * numberOfVertices;
        vertexInputBufferInfo.usage                    = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        vertexInputBufferInfo.sharingMode              = VK_SHARING_MODE_EXCLUSIVE;
        vertexInputBufferInfo.queueFamilyIndexCount    = 0;
        vertexInputBufferInfo.pQueueFamilyIndices      = NULL;
        
        VkResult result =
          vkCreateBuffer( device,
                          // logical device that creates the buffer object
                          &vertexInputBufferInfo,
                          // pointer VkBufferCreateInfo structure containing parameters for the buffer creation
                          NULL,
                          // optional control host memory allocation
                          outVertexInputBuffer );
                          // pointer to returned VkBuffer handle object
        
        ERR_VULKAN_EXIT( result, "Failed to create vertex input buffer." );
        
        // allocate memory for buffers:
        VkMemoryRequirements vertexBufferMemoryRequirements = {};
        vkGetBufferMemoryRequirements( device,
                                       // logical device that owns the buffer
                                       *outVertexInputBuffer,
                                       // buffer to query
                                       &vertexBufferMemoryRequirements );
                                       // pointer to VkMemoryRequirements structure with returned memory requirements
        
        VkMemoryAllocateInfo bufferAllocateInfo = {};
        bufferAllocateInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        bufferAllocateInfo.pNext                = NULL;
        bufferAllocateInfo.allocationSize       = vertexBufferMemoryRequirements.size;
        
        // read the device memory properties
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties( physicalDevice,
                                             // handle to the device to query
                                             &memoryProperties );
                                             // pointer to returned VkPhysicalDeviceMemoryProperties structure
        
        for (uint32_t index=0; index < VK_MAX_MEMORY_TYPES; ++index )
        {
            VkMemoryType memoryType = memoryProperties.memoryTypes[index];
            // is this the memory type you are looking for ?
            if ( (memoryType.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) )
            {
                // save location
                bufferAllocateInfo.memoryTypeIndex = index;
                break;
            }
        }// END for loop index
        
        VkDeviceMemory vertexBufferMemory;
        result =
          vkAllocateMemory( device,
                            // logical device that owns the memory
                            &bufferAllocateInfo,
                            // pointer VkMemoryAllocateInfo strutcure describing allocation parameters
                            NULL,
                            // optional control host memory allocation
                            &vertexBufferMemory );
                            // pointer to returned VkDeviceMemory handle
        
        ERR_VULKAN_EXIT( result, "Failed to allocate buffer memory." );
        
        // set buffer content:
        void *mapped = NULL;
        
        result =
          vkMapMemory( device,
                       // logical device that owns the memory
                       vertexBufferMemory,
                       // VkDeviceMemory object to be mapped
                       0,
                       // zero-based byte offset from the beginning of the memory object
                       VK_WHOLE_SIZE,
                       // size of the memory range to map (or VK_WHOLE_SIZE to map the whole allocation)
                       0,
                       // reserved for future use
                       &mapped );
                       // pointer in which is returned a host-accessible pointer to the beginning of the mapped range
        
        ERR_VULKAN_EXIT( result, "Failed to map buffer memory." );
        
        struct vertex *trianglevertices = (struct vertex *) mapped;
        
        for (int index=0; index<numberOfVertices; ++index )
        {
            // position (Homogeneous Coordinates see Section 4.3.1 )
            trianglevertices[index].x = verticesForCube[index*3 + 0];
            trianglevertices[index].y = verticesForCube[index*3 + 1];
            trianglevertices[index].z = verticesForCube[index*3 + 2];
            trianglevertices[index].w = 1;
            
            // color (mix up the colors based on the x/z-position and loop counter)
            trianglevertices[index].r = trianglevertices[index].x;
            // small changing value for the green
            trianglevertices[index].g = ( (float)(index%10) * 0.1f );
            trianglevertices[index].b = trianglevertices[index].z;
        }//END for loop index
        
        vkUnmapMemory( device,
                       // logical device that owns the memory
                       vertexBufferMemory);
                       // memory object to be unmapped
        
        result = 
          vkBindBufferMemory( device,
                              // logical device that owns the buffer and memory
                              *outVertexInputBuffer,
                              // buffer to be attached to memory
                              vertexBufferMemory,
                              // VkDeviceMemory object describing the device memory to attach
                              0 );
                              // start offset of the region of memory which is to be bound to the buffer
        
        ERR_VULKAN_EXIT( result, "Failed to bind buffer memory." );
    }
    
}//END SetupVertexBuffer(..)
