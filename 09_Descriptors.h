
#pragma once // include guard

//# -----------------------------------------------------
// Step 9 - SetupDescriptors
//
//________//________// START Variables and Functions before main function of this step
//________//________// END Variables and Functions before main function of this step

void SetupDescriptors(VkDevice               device,
                      VkBuffer               buffer,
                      VkDescriptorSet*       outDescriptorSet,
                      VkDescriptorSetLayout* outDescriptorSetLayout)
{
    {
        // Define and create your descriptors:
        VkDescriptorSetLayoutBinding bindings[1];
        
        // uniform buffer for your matrices:
        bindings[0].binding                  = 0;
        bindings[0].descriptorType           = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        bindings[0].descriptorCount          = 1;
        bindings[0].stageFlags               = VK_SHADER_STAGE_VERTEX_BIT;
        bindings[0].pImmutableSamplers       = NULL;
        
        VkDescriptorSetLayoutCreateInfo setLayoutCreateInfo = {};
        setLayoutCreateInfo.sType            = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        setLayoutCreateInfo.bindingCount     = 1;
        setLayoutCreateInfo.pBindings        = bindings;
        
        VkResult result =
          vkCreateDescriptorSetLayout( device,
                                       // logical device that creates the descriptor set layout
                                       &setLayoutCreateInfo,
                                       // pointer to structure specifying the state of the descriptor set layout
                                       NULL,
                                       // optional controling host memory allocation
                                       outDescriptorSetLayout );
                                       // pointer to VkDescriptorSetLayout handle returned descriptor set layout object
        
        ERR_VULKAN_EXIT( result, "Failed to create DescritorSetLayout." );
        
        // descriptor pool creation:
        VkDescriptorPoolSize uniformBufferPoolSize[1];
        uniformBufferPoolSize[0].type             = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uniformBufferPoolSize[0].descriptorCount  = 1;
        
        VkDescriptorPoolCreateInfo poolCreateInfo = {};
        poolCreateInfo.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolCreateInfo.maxSets                    = 1;
        poolCreateInfo.poolSizeCount              = 1;
        poolCreateInfo.pPoolSizes                 = uniformBufferPoolSize;
        
        VkDescriptorPool descriptorPool;
        result = vkCreateDescriptorPool( device,
                                         // logical device that creates the descriptor pool
                                         &poolCreateInfo,
                                         // pointer to VkDescriptorPoolCreateInfo structure specifying the descriptor pool
                                         NULL,
                                         // optional controling host memory allocation
                                         &descriptorPool );
                                         // pointer to returning VkDescriptorPool handle for resulting descriptor pool object
        
        ERR_VULKAN_EXIT( result, "Failed to create descriptor pool." );
        
        // allocate your descriptor from the pool:
        VkDescriptorSetAllocateInfo dsai           = {};
        dsai.sType                                 = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        dsai.descriptorPool                        = descriptorPool;
        dsai.descriptorSetCount                    = 1;
        dsai.pSetLayouts                           = outDescriptorSetLayout;
        
        result = vkAllocateDescriptorSets( device,
                                           // logical device that owns the descriptor pool
                                           &dsai,
                                           // pointer to VkDescriptorSetAllocateInfo structure describing allocation details
                                           outDescriptorSet );
                                           // pointer return array of VkDescriptorSet handles for the resulting descriptor sest objects
        
        ERR_VULKAN_EXIT( result, "Failed to allocate descriptor sets." );
        
        // When a set is allocated all values are undefined and all descriptors are not initialized.
        // You must init all statically used bindings:
        VkDescriptorBufferInfo dbi                     = {};
        dbi.buffer                                     = buffer;
        dbi.offset                                     = 0;
        dbi.range                                      = VK_WHOLE_SIZE;
        
        VkWriteDescriptorSet wd                        = {};
        wd.sType                                       = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        wd.dstSet                                      = *outDescriptorSet;
        wd.dstBinding                                  = 0;
        wd.dstArrayElement                             = 0;
        wd.descriptorCount                             = 1;
        wd.descriptorType                              = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        wd.pImageInfo                                  = NULL;
        wd.pBufferInfo                                 = &dbi;
        wd.pTexelBufferView                            = NULL;
        
        vkUpdateDescriptorSets( device,
                                // logical device that updates the descriptor sets
                                1,
                                // number of elements in the pDescriptorWrites array
                                &wd,
                                // pointer to an array of VkWriteDescriptorSet structures describing the descriptor sets
                                0,
                                // number of elements in the pDescriptorCopies array
                                NULL );
                                // pointer to an array of VkCopyDescriptorSet structures describing the descriptor sets to copy

    }
}// END SetupDescriptors(..)
