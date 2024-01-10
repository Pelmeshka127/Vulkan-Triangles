#include "../inc/app.hpp"
#include "../inc/render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>

namespace Vulkan 
{

//-------------------------------------------------------------------------------//

App::App() 
{
    LoadObjects();
}
//-------------------------------------------------------------------------------//

App::~App() {}

//-------------------------------------------------------------------------------//

void App::RunApplication() 
{
    RenderSystem render_system{device_, render_.GetSwapChainRenderPass()};

    while (!window_.ShouldClose()) 
    {
        glfwPollEvents();
        
        if (auto command_buffer = render_.BeginFrame())
        {
            render_.BeginSwapChainRenderPass(command_buffer);

            render_system.RenderObjects(command_buffer, objects_);

            render_.EndSwapChainRenderPass(command_buffer);

            render_.EndFrame();
        }
    }

    vkDeviceWaitIdle(device_.device());
}

//-------------------------------------------------------------------------------//

void App::LoadObjects() 
{
    std::vector<Model::Vertex> vertices{
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, 
        {{0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},

        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, 
        {{-0.5f, 0.5f},  {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
    };
    
    auto model = std::make_shared<Model>(device_, vertices);

    auto triangle = Object::CreateObject();

    triangle.model = model;
    
    triangle.color = {.1f, .8f, .1f};
    
    triangle.transform2d.translation.x = .2f;

    triangle.transform2d.translation.y = .2f;
    
    triangle.transform2d.scale = {.4f, .4f};
    
    triangle.transform2d.rotation = .25f * glm::two_pi<float>();

    objects_.push_back(std::move(triangle));
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace