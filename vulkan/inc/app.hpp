#pragma once

#include "model.hpp"
#include "window.hpp"
#include "device.hpp"
#include "swap_chain.hpp"
#include "pipeline.hpp"
#include "descriptor.hpp"
#include "uniform_buffer.hpp"
#include "render.hpp"
#include "camera.hpp"
#include "keyboard.hpp"
#include "triangle.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace Vulkan
{

//-------------------------------------------------------------------------------//

glm::vec3 GetGlmVector(const Point& point);

//-------------------------------------------------------------------------------//

glm::vec3 GetNormal(const Triangle& triangle);

//-------------------------------------------------------------------------------//

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

static double prev_x = 0.0;
static double prev_y = 0.0;
static bool lpress = false;

class App 
{
    public:
        
        void run();

        App(std::vector<Model::Vertex> vertices, std::vector<uint32_t> indices) : model(device, vertices, indices) {}

    private:
        
        Camera camera{};
        
        Window window { WIDTH, HEIGHT, "vulkan" };
        
        Device device { window };
        
        SwapChain swapChain { window, device };
        
        DescriptorSetLayout descriptorSetLayout { device };
        
        Pipeline pipeline { device, swapChain, descriptorSetLayout, "vert.spv", "frag.spv" };
        
        Model model;
        
        UniformBuffer uniformBuffer   { device, swapChain, camera, window };
        
        DescriptorPool descriptorPool { device, swapChain };
        
        DescriptorSets descriptorSets { device, swapChain, uniformBuffer, descriptorSetLayout, descriptorPool };
        
        Render render { device, swapChain, pipeline, model, descriptorSets };
        
        void drawFrame();

        static void mouse_button_callback (GLFWwindow* window, int button, int action, int mods) noexcept;
        
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace