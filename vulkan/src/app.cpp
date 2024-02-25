#include "app.hpp"

namespace Vulkan
{

//-------------------------------------------------------------------------------//

void App::run() 
{
    glfwGetCursorPos(window.GetGLFWwindow(), &m_initial_mouse_pos_x, &m_initial_mouse_pos_y);

    camera.set_viewport_size(static_cast<float>(WIDTH), static_cast<float>(HEIGHT));

    while (!window.ShouldClose()) 
    {
        glfwPollEvents();
        glfwSetKeyCallback (window.GetGLFWwindow(), key_callback);
        glfwSetMouseButtonCallback(window.GetGLFWwindow(), mouse_button_callback);
        drawFrame();
        on_update();
        std::cout << camera.get_position().x << std::endl;
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

    uniformBuffer.update(render.currentFrame_, camera.get_projection_matrix(), camera.get_view_matrix(), camera.get_position());
    
    if (vkResetFences(device.getDevice(), 1, &swapChain.getInFlightFences()[render.currentFrame_]) != VK_SUCCESS)
        throw std::runtime_error("failed to reset Fences!");

    render.record(render.getCommandBuffers()[render.currentFrame_], imageIndex);

    VkSemaphore waitSemaphores[] = {swapChain.getImageAvailableSemaphores()[render.currentFrame_]};
    
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    
    VkSemaphore signalSemaphores[] = {swapChain.getRenderFinishedSemaphores()[render.currentFrame_]};

    VkSubmitInfo submitInfo{};
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

void App::mouse_button_callback (GLFWwindow* window, int button, int action, int mods) noexcept
{
    if (action == GLFW_PRESS)        
        Keyboard::press_mouse_button(button);
    
    else if (action == GLFW_RELEASE) 
        Keyboard::release_mouse_button(button);
}

//-------------------------------------------------------------------------------//

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT) 
        Keyboard::press_keyboard_key(key);
    
    else if (action == GLFW_RELEASE)                   
        Keyboard::release_keyboard_key(key);
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

void App::on_update()
{
    glm::vec3 movement_delta{ 0, 0, 0 };
    glm::vec3 rotation_delta{ 0, 0, 0 };

    if (Keyboard::is_keyboard_key(GLFW_KEY_W))
    {
        movement_delta.x += delta;
    }
    if (Keyboard::is_keyboard_key(GLFW_KEY_S))
    {
        movement_delta.x -= delta;
    }
    if (Keyboard::is_keyboard_key(GLFW_KEY_A))
    {
        movement_delta.y -= delta;
    }
    if (Keyboard::is_keyboard_key(GLFW_KEY_D))
    {
        movement_delta.y += delta;
    }
    if (Keyboard::is_keyboard_key(GLFW_KEY_E))
    {
        movement_delta.z += delta;
    }
    if (Keyboard::is_keyboard_key(GLFW_KEY_Q))
    {
        movement_delta.z -= delta;
    }

    if (Keyboard::is_keyboard_key(GLFW_KEY_UP))
    {
        rotation_delta.y -= 0.5f;
    }
    if (Keyboard::is_keyboard_key(GLFW_KEY_DOWN))
    {
        rotation_delta.y += 0.5f;
    }
    if (Keyboard::is_keyboard_key(GLFW_KEY_RIGHT))
    {
        rotation_delta.z -= 0.5f;
    }
    if (Keyboard::is_keyboard_key(GLFW_KEY_LEFT))
    {
        rotation_delta.z += 0.5f;
    }


    camera.add_movement_and_rotation(movement_delta, rotation_delta);
}

} // end of Vulkan namespace