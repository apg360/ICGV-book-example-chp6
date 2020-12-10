
#pragma once // include guard

//# -----------------------------------------------------
// Step 11 - RenderLoop
//
//________//________// START Variables and Functions before main function of this step
//________//________// END Variables and Functions before main function of this step

void RenderLoop(VkDevice          device,
                int               width,
                int               height,
                int               numberOfTriangles,
                VkSwapchainKHR    swapChain,
                VkCommandBuffer   commandBuffer,
                VkImage*          presentImages,
                VkFramebuffer*    frameBuffers,
                VkRenderPass      renderPass,
                VkBuffer          vertexInputBuffer,
                VkDescriptorSet   descriptorSet,
                VkPipelineLayout  pipelineLayout,
                VkPipeline        pipeline)
{
    dlg_warn("Welcome RenderLoop");
    
/*
https://github.com/chaoticbob/tinyrenderers/blob/810aedb94435c039b88defd11a4a757fb8d51321/samples/src/00_Simple.cpp
void draw_frame()
{
    uint32_t frameIdx = s_frame_count % m_renderer->settings.swapchain.image_count;

    tr_fence* image_acquired_fence = m_renderer->image_acquired_fences[frameIdx];
    tr_semaphore* image_acquired_semaphore = m_renderer->image_acquired_semaphores[frameIdx];
    tr_semaphore* render_complete_semaphores = m_renderer->render_complete_semaphores[frameIdx];

    tr_acquire_next_image(m_renderer, image_acquired_semaphore, image_acquired_fence);

    uint32_t swapchain_image_index = m_renderer->swapchain_image_index;
    tr_render_target* render_target = m_renderer->swapchain_render_targets[swapchain_image_index];

    tr_cmd* cmd = m_cmds[frameIdx];
*/    
    
    uint32_t nextImageIdx;
    VkResult result = vkAcquireNextImageKHR( device,
                           swapChain,
                           UINT64_MAX,
                           VK_NULL_HANDLE,
                           VK_NULL_HANDLE,
                           &nextImageIdx );
    dlg_error("nextImageIdx = %u  vkresult = %u" , nextImageIdx,result);
    ERR_VULKAN_EXIT( result, "Failed vkAcquireNextImageKHR" );
    
    exit(1);
    VkCommandBufferBeginInfo beginInfo                         = {};
    beginInfo.sType                                            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags                                            = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    
    vkBeginCommandBuffer( commandBuffer,
                          &beginInfo );
    
    // activate render pass:
    VkClearValue clearValue[]                                  = {{1.0f,1.0f,1.0f,1.0f},{1.0,0.0}};
    VkRenderPassBeginInfo renderPassBeginInfo                  = {};
    renderPassBeginInfo.sType                                  = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass                             = renderPass;
    renderPassBeginInfo.framebuffer                            = frameBuffers[nextImageIdx];
    VkOffset2D a                                               = {0, 0};
    VkExtent2D b                                               = {width, height};
    VkRect2D   c                                               = {a,b};
    renderPassBeginInfo.renderArea                             = c;
    renderPassBeginInfo.clearValueCount                        = 2;
    renderPassBeginInfo.pClearValues                           = clearValue;
    
    vkCmdBeginRenderPass( commandBuffer,
                          &renderPassBeginInfo,
                          VK_SUBPASS_CONTENTS_INLINE );
    // bind the graphics pipeline to the command buffer.
    // Any vkDraw command afterwards is affected by this pipeline.
    
    vkCmdBindPipeline( commandBuffer,
                       VK_PIPELINE_BIND_POINT_GRAPHICS,
                       pipeline );
    
    // take care of dynamic state:
    VkViewport viewport = { 0, 0, (float)width, (float)height, 0, 1 };
    vkCmdSetViewport( commandBuffer, 0, 1, &viewport );
    
    VkRect2D scissor = { 0, 0, width, height };
    vkCmdSetScissor( commandBuffer, 0, 1, &scissor);
    
    // bind your shader parameters:
    
    vkCmdBindDescriptorSets( commandBuffer,
                             VK_PIPELINE_BIND_POINT_GRAPHICS,
                             pipelineLayout,
                             0,
                             1,
                             &descriptorSet,
                             0,
                             NULL );
    
    // render the triangle:
    VkDeviceSize offsets = { 0 };
    vkCmdBindVertexBuffers( commandBuffer,
                            0,
                            1,
                            &vertexInputBuffer,
                            &offsets );
                            
    // VOLK
    //suitable for applications that want to use explicit multi-GPU by creating multiple VkDevice objects.
    struct VolkDeviceTable device_table;
    volkLoadDeviceTable(&device_table, device);
    
    //then using the functions from the table instead:
    
    device_table.vkCmdDraw( commandBuffer,
                            // vertex count
                            numberOfTriangles * 3,
                            // instance count
                            1,
                            // first index
                            0,
                            // first instance
                            0 );
    
    
    vkCmdEndRenderPass( commandBuffer );
    
    vkEndCommandBuffer( commandBuffer );
    
    // present:
    VkFence renderFence;
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType             = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    
    vkCreateFence( device,
                   &fenceCreateInfo,
                   NULL,
                   &renderFence );
    
    VkPipelineStageFlags waitStageMash = { VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT };
    
    VkQueue presentQueue = 0;
    vkGetDeviceQueue( device,
                      0,
                      0,
                      &presentQueue );
    
    VkSubmitInfo submitInfo           = {};
    submitInfo.sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount     = 0;
    submitInfo.pWaitSemaphores        = VK_NULL_HANDLE;
    submitInfo.pWaitDstStageMask      = &waitStageMash;
    submitInfo.commandBufferCount     = 1;
    submitInfo.pCommandBuffers        = &commandBuffer;
    submitInfo.signalSemaphoreCount   = 0;
    submitInfo.pSignalSemaphores      = VK_NULL_HANDLE;
    
    // Submit command buffer (drawing)
    dlg_warn("calling vkQueueSubmit");
    //dlg_warn("commandBuffer = %u",&commandBuffer);
    vkQueueSubmit( presentQueue,
                   // the queue that the command buffers will be submitted to
                   1,
                   // number of elements in the submitInfo array
                   &submitInfo,
                   // pointer to an array of VkSubmitInfo structures, each specifying a command buffer submission batch
                   renderFence );
                   // handle to a fence to be signaled once all submitted command buffers have completed execution
    
    // Wait for the command/queue to finish
    
    vkWaitForFences( device, 1, &renderFence, VK_TRUE, UINT64_MAX );
    
    vkDestroyFence( device, renderFence, NULL );
    
    dlg_warn("calling vkQueuePresentKHR");
    // Present the backbuffer
    VkPresentInfoKHR presentInfo      = {};
    presentInfo.sType                 = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext                 = NULL;
    presentInfo.waitSemaphoreCount    = 0;
    presentInfo.pWaitSemaphores       = VK_NULL_HANDLE;
    presentInfo.swapchainCount        = 1;
    presentInfo.pSwapchains           = &swapChain;
    presentInfo.pImageIndices         = &nextImageIdx;
    presentInfo.pResults              = NULL;
    vkQueuePresentKHR( presentQueue, &presentInfo );
    
    /*glClearColor(0.2, 0.8, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glRotatef(1, 0.2, 0.2, 0);
    glBegin(GL_POLYGON);
      glColor3f(0.4, 0.3, 1.0);
        glVertex3f(-0.8,  0.8, 0.0);
        glVertex3f( 0.8,  0.8, 0.0);
        glVertex3f( 0.8, -0.8, 0.0);
        glVertex3f(-0.8, -0.8, 0.0);
    glEnd();
    */
    dlg_warn("END RenderLoop");
}//END RenderLoop(..)
