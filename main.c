
#include "00_Common.h"                // <-- Includes required for everything
#include "01_Window.h"                // <-- GLFW window, context, keyboard, mouse
#include "02_VulkanInstance.h"        // <-- (Section 6.2)  Initialize Vulkan
#include "03_PhysicalDevice.h"        // <-- (Section 6.4)  Find/Create Device and Set-up your selected device
#include "04_SwapChain.h"             // <-- (Section 6.5)  Initialize Swap-Chain
#include "05_RenderPass.h"            // <-- (Section 6.6)  Create Render Pass
#include "06_CommandBuffer.h"         // <-- (Section 6.7)  Create Command Pool/Buffer
#include "07_VertexBuffer.h"          // <-- (Section 6.10) Vertex Data/Buffer
#include "08_ShaderandUniforms.h"     // <-- (Section 6.11) Load/Setup Shaders
#include "09_Descriptors.h"           // <-- (Section 6.13) Setup Descriptors/Sets
#include "10_Pipeline.h"             // <-- (Section 6.14) Pipeline
#include "11_RenderLoop.h"           // <-- (Section 6.16) Render Loop

//# -----------------------------------------------------
//      Put together all pieces and start Vulkan
//# -----------------------------------------------------
int main(int argc, char *argv[]) {

    // https://www.khronos.org/registry/vulkan/specs/1.2/styleguide.html#markup-word-choices

    // Step 1 - Initializing the window
    int             width               = 800;
    int             height              = 600;
    GLFWwindow*   windowHandle;
    windowHandle = SetupWindow(width, height);
    if( windowHandle == NULL ){
        printf( "Failed to open GLFW window.\n" );
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Step 2 - Initialize Vulkan          (Section 6.2)
    VkInstance    instance                  = NULL;
    VkSurfaceKHR  surface                   = NULL;
    SetupVulkanInstance(windowHandle,
                        &instance,
                        &surface);
    
    // Step 3 - Find/Create Device and     (Section 6.4)
    //          Set-up your selected device
    VkPhysicalDevice physicalDevice         = NULL;
    VkDevice         device                 = NULL;
    SetupPhysicalDevice(instance,
                        &physicalDevice,
                        &device);

     // Step 4 - Initialize Swap-Chain     (Section 6.5)
     VkSwapchainKHR swapChain               = NULL;
     VkImage*       presentImages           = NULL;
     VkImageView*   presentImageViews       = NULL;
     SetupSwapChain(device,
                    physicalDevice,
                    surface,
                    &width,
                    &height,
                    &swapChain,
                    &presentImages,
                    &presentImageViews);
    
    //dlg_warn("presentImageViews[0] = %p", &presentImageViews[0]); //Debug
    //dlg_warn("presentImageViews[1] = %p", &presentImageViews[1]); //Debug
    
     // Step 5 - Create Render Pass          (Section 6.6)
     VkRenderPass    renderPass             = NULL;
     VkFramebuffer*  frameBuffers           = NULL;
     SetupRenderPass(device,
                     physicalDevice,
                     width,
                     height,
                     presentImageViews,
                     &renderPass,
                     &frameBuffers);

     // Step 6 - Create Command Pool/Buffer  (Section 6.7)
     VkCommandBuffer  commandBuffer         = NULL;
     SetupCommandBuffer(device,
                        physicalDevice,
                        &commandBuffer);

     // Step 7 - Vertex Data/Buffer          (Section 6.10)
     VkBuffer  vertexInputBuffer            = NULL;
     int       vertexSize                   = 0;
     int       numberOfTriangles            = 0;
     SetupVertexBuffer(device,
                       physicalDevice,
                       &vertexSize,
                       &numberOfTriangles,
                       &vertexInputBuffer);

     // Step 8 - Load/Setup Shaders          (Section 6.11)
     VkShaderModule    vertShaderModule     = NULL;
     VkShaderModule    fragShaderModule     = NULL;
     VkBuffer          buffer               = NULL;
     VkDeviceMemory    memory               = NULL;
     SetupShaderandUniforms(device,
                            physicalDevice,
                            width,
                            height,
                            &vertShaderModule,
                            &fragShaderModule,
                            &buffer,
                            &memory);

     // Step 9 - Setup Descriptors/Sets      (Section 6.13)
     VkDescriptorSet       descriptorSet       = NULL;
     VkDescriptorSetLayout descriptorSetLayout = NULL;
     SetupDescriptors(device,
                      buffer,
                      &descriptorSet,
                      &descriptorSetLayout);

     // Step 10 - Pipeline                   (Section 6.14)
     VkPipeline       pipeline                    = NULL;
     VkPipelineLayout pipelineLayout              = NULL;
     SetupPipeline(device,
                   width,
                   height,
                   vertexSize,
                   descriptorSetLayout,
                   vertShaderModule,
                   fragShaderModule,
                   renderPass,
                   &pipeline,
                   &pipelineLayout);


////////////////////
//----------------------
     // Step 11 - Render Loop                 (Section 6.16)
     while ( !glfwWindowShouldClose(windowHandle) )
     {
        // Keep running
        
        //Calculate frames per second
        showFPS(windowHandle);
        
        //Processing events, normally done each frame after buffer swapping.
        //Will use event polling, which processes only those events that have already been received and then returns immediately.
        //This is the best choice when rendering continually, like most games do.
        glfwPollEvents();
        
        //Render the screen
        RenderLoop(device,
                   width,
                   height,
                   numberOfTriangles,
                   swapChain,
                   commandBuffer,
                   presentImages,
                   frameBuffers,
                   renderPass,
                   vertexInputBuffer,
                   descriptorSet,
                   pipelineLayout,
                   pipeline);
         
     }// End while(..)
//______________________
//The Cleanup
     //vkDestroyDebugUtilsMessengerEXT(instance, debugUtilsMessenger, NULL);
     vkDestroyDebugReportCallbackEXT(instance, debugReportCallback_Warning, NULL);
     vkDestroyDebugReportCallbackEXT(instance, debugReportCallback_Error, NULL);
     
     //Cleanup (for every "malloc" there must be a "free"
     free(frameBuffers);
     free(renderPass);
     free(frameBuffers);
     free(presentImages);
     free(presentImageViews);

     vkDestroyInstance(instance, NULL);
     glfwDestroyWindow(windowHandle);
     glfwTerminate();

// End
    return EXIT_SUCCESS;
}//End WinMain(..)
