#include "app.hpp"

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

struct Ubo {
    glm::mat4 projectionView{1.f};
    glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
};

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
    UniformBuffer uniform_buffer {
        device_,
        sizeof(uniform_buffer),
        SwapChain::MAX_FRAMES_IN_FLIGHT,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        device_.properties.limits.minUniformBufferOffsetAlignment,
    };

    uniform_buffer.map();

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
            int frameIndex = render_.GetFrameIndex();
            FrameInfo frameInfo {frameIndex, frame_time, command_buffer, camera};
            
            Ubo ubo{};
            ubo.projectionView = camera.GetProjection() * camera.GetView();
            uniform_buffer.writeToIndex(&ubo, frameIndex);
            uniform_buffer.flushIndex(frameIndex);

            render_.BeginSwapChainRenderPass(command_buffer);
            render_system.RenderObjects(objects_, frameInfo);
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
    std::unique_ptr<Model> model = CreateTriangleModel(device_, {0.f, 0.f, 0.f}, builder);

    auto triangle = Object::CreateObject();

    triangle.model = std::move(model);

    objects_.push_back(std::move(triangle));
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