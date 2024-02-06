#include "render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>
#include <cassert>

namespace Vulkan 
{

struct SimplePushConstantData { 
    glm::mat4 transform{1.f}; 
    glm::mat4 modelMatrix{1.f};
}; 

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
    VkPushConstantRange pushConstantRange{}; 
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT; 
    pushConstantRange.offset = 0; 
    pushConstantRange.size = sizeof(SimplePushConstantData);

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
        "vert.spv",
        "frag.spv",
        pipelineConfig);
}

//-------------------------------------------------------------------------------//

void RenderSystem::RenderObjects(std::vector<Object>& objects, FrameInfo& frameInfo)
{
    pipeline_->Bind(frameInfo.commandBuffer);

    auto projection_view = frameInfo.camera.GetProjection() * frameInfo.camera.GetView();

    for (auto& obj : objects) 
    {

        SimplePushConstantData push{};
        auto modelMatrix    = obj.transform.mat4();
        push.transform      = projection_view * modelMatrix;
        push.modelMatrix    = modelMatrix;

        vkCmdPushConstants(
            frameInfo.commandBuffer,
            pipeline_layout_,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(SimplePushConstantData),
            &push);

        obj.model->Bind(frameInfo.commandBuffer);
        obj.model->Draw(frameInfo.commandBuffer);
    }
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace