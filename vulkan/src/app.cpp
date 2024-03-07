#include "app.hpp"

namespace Vulkan
{

//-------------------------------------------------------------------------------//

void App::run() 
{
    while (!window.ShouldClose()) 
    {
        glfwPollEvents();
        
        glfwSetKeyCallback(window.GetGLFWwindow(), keyCallback);
        
        drawFrame();
    }

    vkDeviceWaitIdle(device.getDevice());
}

//-------------------------------------------------------------------------------//

void App::drawFrame() 
{
    if (vkWaitForFences(device.getDevice(),1, &swapChain.getInFlightFences()[render.currentFrame_], VK_TRUE, UINT64_MAX) != VK_SUCCESS)
        throw std::runtime_error("failed to wait for Fences!");

    uint32_t imageIndex = 0;
    
    VkResult result = vkAcquireNextImageKHR(device.getDevice(), swapChain.getSwapChain(), UINT64_MAX, swapChain.getImageAvailableSemaphores()[render.currentFrame_], nullptr, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) 
    {
        swapChain.recreate();
        return;
    } 
    
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
        throw std::runtime_error("failed to acquire swap chain image!");

    uniformBuffer.update(render.currentFrame_);
    
    if (vkResetFences(device.getDevice(), 1, &swapChain.getInFlightFences()[render.currentFrame_]) != VK_SUCCESS)
        throw std::runtime_error("failed to reset Fences!");

    render.record(render.getCommandBuffers()[render.currentFrame_], imageIndex);

    VkSemaphore waitSemaphores[] = {swapChain.getImageAvailableSemaphores()[render.currentFrame_]};
    
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    
    VkSemaphore signalSemaphores[] = {swapChain.getRenderFinishedSemaphores()[render.currentFrame_]};

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &render.getCommandBuffers()[render.currentFrame_];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(device.getDevice(), 1, &swapChain.getInFlightFences()[render.currentFrame_]);

    if (vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, swapChain.getInFlightFences()[render.currentFrame_]) != VK_SUCCESS)
        throw std::runtime_error("failed to submit draw command buffer!");

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain.getSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(device.getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.WasFrameBufferResized()) 
    {
        window.ResetFrameBufferRisizedFlag();
        swapChain.recreate();
    } 
    
    else if (result != VK_SUCCESS) 
        throw std::runtime_error("failed to present swap chain image!");

    render.currentFrame_ = (render.currentFrame_ + 1) % swapChain.MAX_FRAMES_IN_FLIGHT;
}

//-------------------------------------------------------------------------------//

void App::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT) 
        Keyboard::pressKeyboardKey(key);
    
    else if (action == GLFW_RELEASE)                   
        Keyboard::releaseKeyboardKey(key);
}

//-------------------------------------------------------------------------------//

glm::vec3 GetNormal(const Triangle& triangle)
{
    glm::vec3 side1{}, side2{};

    side1.x = triangle.P2().X() - triangle.P1().X();
    side1.y = triangle.P2().Y() - triangle.P1().Y();
    side1.z = triangle.P2().Z() - triangle.P1().Z();

    side2.x = triangle.P3().X() - triangle.P1().X();
    side2.y = triangle.P3().X() - triangle.P1().X();
    side2.z = triangle.P3().X() - triangle.P1().X();

    return glm::normalize(glm::cross(side1, side2));
}

//-------------------------------------------------------------------------------//

glm::vec3 GetGlmVector(const Point& point)
{
    glm::vec3 vec{};

    vec.x = point.X();

    vec.y = point.Y();

    vec.z = point.Z();

    return vec;
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace