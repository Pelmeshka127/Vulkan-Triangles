#include "app.hpp"

#include <array>
#include <stdexcept>

namespace Vulkan 
{

//-------------------------------------------------------------------------------//

App::App() 
{
    LoadModels();
    
    CreatePipelineLayout();
    
    CreatePipeline();
    
    CreateCommandBuffers();
}

//-------------------------------------------------------------------------------//

App::~App() 
{ 
    vkDestroyPipelineLayout(Device.device(), pipelineLayout, nullptr); 
}

//-------------------------------------------------------------------------------//

void App::RunApplication() 
{
    while (!Window.shouldClose()) 
    {
        glfwPollEvents();
        
        drawFrame();
    }

    vkDeviceWaitIdle(Device.device());
}

//-------------------------------------------------------------------------------//

void App::LoadModels() 
{
    std::vector<Model::Vertex> vertices{{{0.0f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}}};
    
    Model = std::make_unique<Model>(Device, vertices);
}

//-------------------------------------------------------------------------------//

void App::CreatePipelineLayout() {
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;
  if (vkCreatePipelineLayout(Device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

//-------------------------------------------------------------------------------//

void App::CreatePipeline() {
  PipelineConfigInfo pipelineConfig{};
  Pipeline::defaultPipelineConfigInfo(
      pipelineConfig,
      SwapChain.width(),
      SwapChain.height());
  pipelineConfig.renderPass = SwapChain.getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  Pipeline = std::make_unique<Pipeline>(
      Device,
      "shaders/simple_shader.vert.spv",
      "shaders/simple_shader.frag.spv",
      pipelineConfig);
}

//-------------------------------------------------------------------------------//

void App::CreateCommandBuffers() {
  commandBuffers.resize(SwapChain.imageCount());

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = Device.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(Device.device(), &allocInfo, commandBuffers.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }

  for (int i = 0; i < commandBuffers.size(); i++) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = SwapChain.getRenderPass();
    renderPassInfo.framebuffer = SwapChain.getFrameBuffer(i);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = SwapChain.getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    Pipeline->bind(commandBuffers[i]);
    Model->bind(commandBuffers[i]);
    Model->draw(commandBuffers[i]);

    vkCmdEndRenderPass(commandBuffers[i]);
    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
    }
  }
}

//-------------------------------------------------------------------------------//

void App::drawFrame() {
  uint32_t imageIndex;
  auto result = SwapChain.acquireNextImage(&imageIndex);
  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  result = SwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
  if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace