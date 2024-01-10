#include "../inc/render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>
#include <cassert>

namespace Vulkan 
{

struct SimplePushConstantData { //
    glm::mat2 transform{1.f}; //
    glm::vec2 offset; //
    alignas(16) glm::vec3 color; //
}; //

//-------------------------------------------------------------------------------//

RenderSystem::RenderSystem(Device& device, VkRenderPass render_pass) : device_{device}
{
    CreatePipelineLayout();

    CreatePipeline(render_pass);
}
//-------------------------------------------------------------------------------//

RenderSystem::~RenderSystem() 
{ 
    vkDestroyPipelineLayout(device_.device(), pipeline_layout_, nullptr); 
}

//-------------------------------------------------------------------------------//

void RenderSystem::CreatePipelineLayout() 
{ 
    VkPushConstantRange pushConstantRange{};  //
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT; // 
    pushConstantRange.offset = 0; // 
    pushConstantRange.size = sizeof(SimplePushConstantData); //

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    
    pipelineLayoutInfo.sType                    = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount           = 0;
    pipelineLayoutInfo.pSetLayouts              = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount   = 1;
    pipelineLayoutInfo.pPushConstantRanges      = &pushConstantRange;
    
    if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutInfo, nullptr, &pipeline_layout_) !=
        VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

//-------------------------------------------------------------------------------//

void RenderSystem::CreatePipeline(VkRenderPass render_pass) 
{
    assert(pipeline_layout_ != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    
    Pipeline::DefaultPipelineConfigInfo(
        pipelineConfig);
    
    pipelineConfig.renderPass       = render_pass;
    
    pipelineConfig.pipelineLayout   = pipeline_layout_;
    
    pipeline_ = std::make_unique<Pipeline>(
        device_,
        "shaders/simple_shader.vert.spv",
        "shaders/simple_shader.frag.spv",
        pipelineConfig);
}

//-------------------------------------------------------------------------------//

void RenderSystem::RenderObjects(VkCommandBuffer CommandBuffer, std::vector<Object>& objects)
{
    static double d = 0.001f;

    d += 0.001f;

    if (d >= 1)
        d = 0.001;
    
    pipeline_->Bind(CommandBuffer);

    for (auto& obj : objects) 
    {
        obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());

        SimplePushConstantData push{};
        push.offset = obj.transform2d.translation;
        push.color = {0.5 + d * 0.5, d, 1 - d};
        push.transform = obj.transform2d.mat2();

        vkCmdPushConstants(
            CommandBuffer,
            pipeline_layout_,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(SimplePushConstantData),
            &push);

        obj.model->Bind(CommandBuffer);
        obj.model->Draw(CommandBuffer);
    }
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace