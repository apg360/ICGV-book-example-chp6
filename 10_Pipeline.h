
#pragma once // include guard

//# -----------------------------------------------------
// Step 10 - SetupPipeline
//
//________//________// START Variables and Functions before main function of this step
//________//________// END Variables and Functions before main function of this step

void SetupPipeline(VkDevice               device,
                   int                    width,
                   int                    height,
                   int                    vertexSize,
                   VkDescriptorSetLayout  descriptorSetLayout,
                   VkShaderModule         vertShaderModule,
                   VkShaderModule         fragShaderModule,
                   VkRenderPass           renderPass,
                   VkPipeline*            outPipeline,
                   VkPipelineLayout*      outPipelineLayout)
{
    // Graphics pipeline:
    VkPipelineLayoutCreateInfo layoutCreateInfo                = {};
    layoutCreateInfo.sType                                     = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutCreateInfo.setLayoutCount                            = 1;
    layoutCreateInfo.pSetLayouts                               = &descriptorSetLayout;
    layoutCreateInfo.pushConstantRangeCount                    = 0;
    layoutCreateInfo.pPushConstantRanges                       = NULL;
    
    VkResult result =
      vkCreatePipelineLayout( device,
                              // logical device that creates the pipeline layout
                              &layoutCreateInfo,
                              // pointer to VkPipelineLayoutCreateInfo structure specifying pipeline layout object
                              NULL,
                              // optional controling host memory allocation
                              outPipelineLayout );
                              // pointer to return VkPipelineLayout handle for pipeline layout
    
    ERR_VULKAN_EXIT( result, "Failed to create pipeline layout." );
    
    // setup shader stages in the pipeline:
    VkPipelineShaderStageCreateInfo shaderStageCreateInfo[2]   = {};
    shaderStageCreateInfo[0].sType                             = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageCreateInfo[0].stage                             = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStageCreateInfo[0].module                            = vertShaderModule;
    // shader entry point function name
    shaderStageCreateInfo[0].pName                             = "main"; // shader entry point
    shaderStageCreateInfo[0].pSpecializationInfo               = NULL;
    
    shaderStageCreateInfo[1].sType                             = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageCreateInfo[1].stage                             = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStageCreateInfo[1].module                            = fragShaderModule;
    // shader entry point function name
    shaderStageCreateInfo[1].pName                             = "main"; // shader entry point
    shaderStageCreateInfo[1].pSpecializationInfo               = NULL;
    
    // vertex input configuration:
    VkVertexInputBindingDescription vertexBindingDescription   = {};
    vertexBindingDescription.binding                           = 0;
    vertexBindingDescription.stride                            = vertexSize;
    vertexBindingDescription.inputRate                         = VK_VERTEX_INPUT_RATE_VERTEX;
    
    VkVertexInputAttributeDescription vertexAttributeDescription[2];
    
    // position:
    vertexAttributeDescription[0].location                     = 0;
    vertexAttributeDescription[0].binding                      = 0;
    vertexAttributeDescription[0].format                       = VK_FORMAT_R32G32B32A32_SFLOAT;
    vertexAttributeDescription[0].offset                       = 0;
    
    // colors:
    vertexAttributeDescription[1].location                     = 1;
    vertexAttributeDescription[1].binding                      = 0;
    vertexAttributeDescription[1].format                       = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttributeDescription[1].offset                       = 4 * sizeof(float);
    
    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = {};
    vertexInputStateCreateInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateCreateInfo.vertexBindingDescriptionCount   = 1;
    // attribute indexing is a function of the vertex index
    vertexInputStateCreateInfo.pVertexBindingDescriptions      = &vertexBindingDescription;
    vertexInputStateCreateInfo.vertexAttributeDescriptionCount = 2;
    vertexInputStateCreateInfo.pVertexAttributeDescriptions    = vertexAttributeDescription;
    
    // vertex topology config:
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = {};
    inputAssemblyStateCreateInfo.sType                         = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyStateCreateInfo.topology                      = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyStateCreateInfo.primitiveRestartEnable        = VK_FALSE;
    
    // viewport config:
    VkViewport viewport                                        = {};
    viewport.x                                                 = 0;
    viewport.y                                                 = 0;
    viewport.width                                             = (float) width;
    viewport.height                                            = (float) height;
    viewport.minDepth                                          = 0;
    viewport.maxDepth                                          = 1;
    
    VkRect2D scissors                                          = {};
    VkOffset2D k                                               = {0,0};
    VkExtent2D m                                               = {width,height};
    scissors.offset                                            = k;
    scissors.extent                                            = m;
    
    VkPipelineViewportStateCreateInfo viewportState            = {};
    viewportState.sType                                        = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount                                = 1;
    viewportState.pViewports                                   = &viewport;
    viewportState.scissorCount                                 = 1;
    viewportState.pScissors                                    = &scissors;
    
    // rasterization config:
    VkPipelineRasterizationStateCreateInfo rasterizationState  = {};
    rasterizationState.sType                                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.depthClampEnable                        = VK_FALSE;
    rasterizationState.rasterizerDiscardEnable                 = VK_FALSE;
    rasterizationState.polygonMode                             = VK_POLYGON_MODE_FILL;
    rasterizationState.cullMode                                = VK_CULL_MODE_NONE;
    rasterizationState.frontFace                               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizationState.depthBiasEnable                         = VK_FALSE;
    rasterizationState.depthBiasConstantFactor                 = 0;
    rasterizationState.depthBiasClamp                          = 0;
    rasterizationState.depthBiasSlopeFactor                    = 0;
    rasterizationState.lineWidth                               = 1;
    
    // sampling config:
    VkPipelineMultisampleStateCreateInfo multisampleState      = {};
    multisampleState.sType                                     = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.rasterizationSamples                      = VK_SAMPLE_COUNT_1_BIT;
    multisampleState.sampleShadingEnable                       = VK_FALSE;
    multisampleState.minSampleShading                          = 0;
    multisampleState.pSampleMask                               = NULL;
    multisampleState.alphaToCoverageEnable                     = VK_FALSE;
    multisampleState.alphaToOneEnable                          = VK_FALSE;
    
    // color blend config: (Actually off for tutorial)
    VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
    colorBlendAttachmentState.blendEnable                      = VK_FALSE;
    colorBlendAttachmentState.srcColorBlendFactor              = VK_BLEND_FACTOR_SRC_COLOR;
    colorBlendAttachmentState.dstColorBlendFactor              = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    colorBlendAttachmentState.colorBlendOp                     = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.srcAlphaBlendFactor              = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.dstAlphaBlendFactor              = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.alphaBlendOp                     = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.colorWriteMask                   = 0xf;
    
    VkPipelineColorBlendStateCreateInfo colorBlendState        = {};
    colorBlendState.sType                                      = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.logicOpEnable                              = VK_FALSE;
    colorBlendState.logicOp                                    = VK_LOGIC_OP_CLEAR;
    colorBlendState.attachmentCount                            = 1;
    colorBlendState.pAttachments                               = &colorBlendAttachmentState;
    colorBlendState.blendConstants[0]                          = 0.0;
    colorBlendState.blendConstants[1]                          = 0.0;
    colorBlendState.blendConstants[2]                          = 0.0;
    colorBlendState.blendConstants[3]                          = 0.0;
    
    // configure dynamic state:
    VkDynamicState dynamicState[2]                             = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo    = {};
    dynamicStateCreateInfo.sType                               = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount                   = 2;
    dynamicStateCreateInfo.pDynamicStates                      = dynamicState;
    
    // add depth buffer options to the pipeline
    VkPipelineDepthStencilStateCreateInfo depthStencil         = {};
    depthStencil.sType                                         = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable                               = VK_TRUE;
    depthStencil.depthWriteEnable                              = VK_TRUE;
    depthStencil.depthCompareOp                                = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable                         = VK_FALSE;
    depthStencil.minDepthBounds                                = 0.0f; // Optional
    depthStencil.maxDepthBounds                                = 1.0f; // Optional
    depthStencil.stencilTestEnable                             = VK_FALSE;
    
    // add finally, pipeline config and creation:
    VkGraphicsPipelineCreateInfo pipelineCreateInfo            = {};
    pipelineCreateInfo.sType                                   = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount                              = 2;
    pipelineCreateInfo.pStages                                 = shaderStageCreateInfo;
    pipelineCreateInfo.pVertexInputState                       = &vertexInputStateCreateInfo;
    pipelineCreateInfo.pInputAssemblyState                     = &inputAssemblyStateCreateInfo;
    pipelineCreateInfo.pTessellationState                      = NULL;
    pipelineCreateInfo.pViewportState                          = &viewportState;
    pipelineCreateInfo.pRasterizationState                     = &rasterizationState;
    pipelineCreateInfo.pMultisampleState                       = &multisampleState;
    pipelineCreateInfo.pDepthStencilState                      = &depthStencil;
    pipelineCreateInfo.pColorBlendState                        = &colorBlendState;
    pipelineCreateInfo.pDynamicState                           = &dynamicStateCreateInfo;
    pipelineCreateInfo.layout                                  = *outPipelineLayout;
    pipelineCreateInfo.renderPass                              = renderPass;
    pipelineCreateInfo.subpass                                 = 0;
    pipelineCreateInfo.basePipelineHandle                      = NULL;
    pipelineCreateInfo.basePipelineIndex                       = 0;
    
    result =
      vkCreateGraphicsPipelines( device,
                                 // logical device that creates the graphics pipelines.
                                 VK_NULL_HANDLE,
                                 // either VK_NULL_HANDLE indicating caching is disabled; or handle of a pipeline cache object
                                 1,
                                 // length of the pCreateInfos and pPipelines arrays
                                 &pipelineCreateInfo,
                                 // array of VkGraphicsPipelineCreateInfo structures
                                 NULL,
                                 // controls host memory allocation
                                 outPipeline );
                                 // pointer to an array in which the resulting graphics pipeline objects are returned
    
    ERR_VULKAN_EXIT( result, "Failed to create graphics pipeline." );
}// END SetupPipeline(..)
