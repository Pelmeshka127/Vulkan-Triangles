#include "../inc/app.hpp"
#include "../inc/render_system.hpp"
#include "../inc/keyboard.hpp"
#include "../inc/camera.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>
#include <cassert>
#include <chrono>

namespace Vulkan 
{

//-------------------------------------------------------------------------------//

App::App(const Model::Builder& builder) 
{
    LoadObjects(builder);
}
//-------------------------------------------------------------------------------//

App::~App() {}

//-------------------------------------------------------------------------------//

void App::RunApplication() 
{
    RenderSystem render_system{device_, render_.GetSwapChainRenderPass()};
    
    Camera camera{};

    auto viewer_object = Object::CreateObject();

    auto current_time = std::chrono::high_resolution_clock::now();

    Keyboard camera_controller{};

    while (!window_.ShouldClose()) 
    {
        glfwPollEvents();

        auto new_time = std::chrono::high_resolution_clock::now();

        float frame_time = std::chrono::duration<float, std::chrono::seconds::period>(new_time - current_time).count();

        current_time = new_time;

        camera_controller.MoveInPlainXZ(window_.GetWindow(), frame_time, viewer_object);

        camera.SetViewYXZ(viewer_object.transform.translation, viewer_object.transform.rotation);

        float aspect = render_.GetAspectRatio();

        camera.SetOrthographProjection(-aspect, aspect, -1, 1, -1, 1);

        camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);
        
        if (auto command_buffer = render_.BeginFrame())
        {
            render_.BeginSwapChainRenderPass(command_buffer);

            render_system.RenderObjects(command_buffer, objects_, camera);

            render_.EndSwapChainRenderPass(command_buffer);

            render_.EndFrame();
        }
    }

    vkDeviceWaitIdle(device_.device());
}

//-------------------------------------------------------------------------------//

std::unique_ptr<Model> App::CreateTriangleModel(Device &device, glm::vec3 offset, const Model::Builder& builder)
{
    return std::make_unique<Model>(device, builder);
}

//-------------------------------------------------------------------------------//

void App::LoadObjects(const Model::Builder& builder) 
{
    std::shared_ptr<Model> model = CreateTriangleModel(device_, {0.f, 0.f, 0.f}, builder);

    auto triangle = Object::CreateObject();

    triangle.model = model;

    objects_.push_back(std::move(triangle));
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace