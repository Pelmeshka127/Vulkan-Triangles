#include "../inc/window.hpp"

#include <stdexcept>

namespace Vulkan
{

//-------------------------------------------------------------------------------//

Window::Window(int width, int height, const std::string& name) : 
    width_{width}, 
    height_{height}, 
    window_name_{name}
{
    InitWindow();
}

//-------------------------------------------------------------------------------//

void Window::InitWindow()
{
    glfwInit();
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_ = glfwCreateWindow(width_, height_, window_name_.c_str(), nullptr, nullptr);
}

//-------------------------------------------------------------------------------//

void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(instance, window_, nullptr, surface) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed create window\n");
    }
}

//-------------------------------------------------------------------------------//

Window::~Window()
{
    glfwDestroyWindow(window_);

    glfwTerminate();
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace