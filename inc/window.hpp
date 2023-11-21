#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

//-------------------------------------------------------------------------------//

namespace Vulkan
{

//-------------------------------------------------------------------------------//

class Window
{
    private:

        const int width_;

        const int height_;

        const std::string window_name_;

        GLFWwindow* window_;

        void InitWindow();

    public:

        Window(int width, int height, const std::string& name);

        ~Window();

        Window(const Window& rhs) = delete;

        Window& operator=(const Window& rhs) = delete;

        bool ShouldClose() { return glfwWindowShouldClose(window_); }

        VkExtent2D GetExtent() { return {static_cast<uint32_t>(width_), static_cast<uint32_t>(height_)}; }

        void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
};

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace

#endif