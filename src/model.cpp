#include "../inc/model.hpp"

#include <cassert>

namespace Vulkan
{

//-------------------------------------------------------------------------------//

Model::Model(Device &device, const std::vector<Vertex> &vertices) : Device_{device}
{
    CreateVertexBuffers(vertices);
}

//-------------------------------------------------------------------------------//

void Model::CreateVertexBuffers(const std::vector<Vertex>& vertices)
{
    vertexCount_ = static_cast<uint32_t>(vertices.size());

    assert(vertexCount_ >= 3 && "Vertex count must be at least 3");

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount_;

    Device_.createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        vertexBuffer_,
        vertexBufferMemory_);

    void* data;

    vkMapMemory(Device_.device(), vertexBufferMemory_, 0, bufferSize, 0, &data);

    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  
    vkUnmapMemory(Device_.device(), vertexBufferMemory_);
}

//-------------------------------------------------------------------------------//

void Model::Bind(VkCommandBuffer commandBuffer)
{
    VkBuffer buffers[] = {vertexBuffer_};

    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

//-------------------------------------------------------------------------------//
    
void Model::Draw(VkCommandBuffer commandBuffer)
{
    vkCmdDraw(commandBuffer, vertexCount_, 1, 0, 0);
}

//-------------------------------------------------------------------------------//

std::vector<VkVertexInputBindingDescription> Model::Vertex::GetBindingDescriptions() 
{
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    
    bindingDescriptions[0].binding = 0;
    
    bindingDescriptions[0].stride = sizeof(Vertex);
    
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    
    return bindingDescriptions;
}

//-------------------------------------------------------------------------------//

std::vector<VkVertexInputAttributeDescription> Model::Vertex::GetAttributeDescriptions() 
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
    
    
    attributeDescriptions[0].binding    = 0;
    
    attributeDescriptions[0].location   = 0;
    
    attributeDescriptions[0].format     = VK_FORMAT_R32G32B32_SFLOAT;
    
    attributeDescriptions[0].offset     = offsetof(Vertex, position);

    
    attributeDescriptions[1].binding    = 0;
    
    attributeDescriptions[1].location   = 1;
    
    attributeDescriptions[1].format     = VK_FORMAT_R32G32B32_SFLOAT;
    
    attributeDescriptions[1].offset     = offsetof(Vertex, color);
    
    
    return attributeDescriptions;
}

//-------------------------------------------------------------------------------//

Model::~Model()
{
    vkDestroyBuffer(Device_.device(), vertexBuffer_, nullptr);

    vkFreeMemory(Device_.device(), vertexBufferMemory_, nullptr);
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace

//-------------------------------------------------------------------------------//