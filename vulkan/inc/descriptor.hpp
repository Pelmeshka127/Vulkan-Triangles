#ifndef DESCRIPTOR_HPP_
#define DESCRIPTOR_HPP_

#include <memory>
#include <unordered_map>
#include <vector>

#include "device.hpp"

namespace Vulkan
{

//-------------------------------------------------------------------------------//

class DescriptorSetLayout 
{
    public:
    

        class Builder 
        {
            public:
                
                Builder(Device &device) : device{device} {}

                Builder& addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count = 1) 
                {
                    assert(bindings.count(binding) == 0 && "Binding already in use");
                    
                    VkDescriptorSetLayoutBinding layoutBinding{};
                    
                    layoutBinding.binding = binding;
                    
                    layoutBinding.descriptorType = descriptorType;
                    
                    layoutBinding.descriptorCount = count;
                    
                    layoutBinding.stageFlags = stageFlags;
                    
                    bindings[binding] = layoutBinding;
                    
                    return *this;
                }

                std::unique_ptr<DescriptorSetLayout> build() const 
                {
                    return std::make_unique<DescriptorSetLayout>(device, bindings);
                }
    

            private:
                Device &device;
                std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };



    public:

        DescriptorSetLayout(Device& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings) : device{device}, bindings{bindings}
        {
            std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
            
            for (auto kv : bindings) 
                setLayoutBindings.push_back(kv.second);
            
            VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
            
            descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            
            descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
            
            descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

            if (vkCreateDescriptorSetLayout(
                    device.device(),
                    &descriptorSetLayoutInfo,
                    nullptr,
                    &descriptorSetLayout) != VK_SUCCESS) 
            {
                throw std::runtime_error("failed to create descriptor set layout!");
            }
        }
        
        ~DescriptorSetLayout() 
        {
            vkDestroyDescriptorSetLayout(device.device(), descriptorSetLayout, nullptr);
        }      
        
        DescriptorSetLayout(const DescriptorSetLayout &) = delete;
        
        DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        Device &device;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

};

//-------------------------------------------------------------------------------//

class DescriptorPool 
{


    public:
        
        class Builder 
        {
            public:
                Builder(Device &device) : device{device} {}

                Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count) 
                {
                    poolSizes.push_back({descriptorType, count});
                    return *this;
                }
                
                Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags) 
                {
                    poolFlags = flags;
                    return *this;
                }
                
                Builder &setMaxSets(uint32_t count) 
                {
                    maxSets = count;
                    return *this;
                }
                
                std::unique_ptr<DescriptorPool> build() const 
                {
                    return std::make_unique<DescriptorPool>(device, maxSets, poolFlags, poolSizes);
                }

            private:
                Device &device;
                std::vector<VkDescriptorPoolSize> poolSizes{};
                uint32_t maxSets = 1000;
                VkDescriptorPoolCreateFlags poolFlags = 0;
        };


    DescriptorPool(Device &device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize> &poolSizes) : device{device} 
    {
        VkDescriptorPoolCreateInfo descriptorPoolInfo{};
        
        descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        
        descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        
        descriptorPoolInfo.pPoolSizes = poolSizes.data();
        
        descriptorPoolInfo.maxSets = maxSets;
        
        descriptorPoolInfo.flags = poolFlags;
        
        if (vkCreateDescriptorPool(device.device(), &descriptorPoolInfo, nullptr, &descriptorPool) !=
            VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }
    
    ~DescriptorPool() 
    {
        vkDestroyDescriptorPool(device.device(), descriptorPool, nullptr);
    }
    
    DescriptorPool(const DescriptorPool &) = delete;
    DescriptorPool &operator=(const DescriptorPool &) = delete;

    bool allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const 
    {
        VkDescriptorSetAllocateInfo allocInfo{};
        
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        
        allocInfo.descriptorPool = descriptorPool;
        
        allocInfo.pSetLayouts = &descriptorSetLayout;
        
        allocInfo.descriptorSetCount = 1;
        
        // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
        // a new pool whenever an old pool fills up. But this is beyond our current scope
        if (vkAllocateDescriptorSets(device.device(), &allocInfo, &descriptor) != VK_SUCCESS) 
            return false;
        
        return true;
    }

    void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const 
    {
        vkFreeDescriptorSets(device.device(), descriptorPool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
    }

    void resetPool() 
    {
        vkResetDescriptorPool(device.device(), descriptorPool, 0);
    }

    private:
        Device &device;
        VkDescriptorPool descriptorPool;

};

//-------------------------------------------------------------------------------//

class DescriptorWriter 
{
    public:
        DescriptorWriter(DescriptorSetLayout &setLayout, DescriptorPool &pool)
            : setLayout{setLayout}, pool{pool} {}

        DescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo) 
        {
            assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");
            
            auto &bindingDescription = setLayout.bindings[binding];
            
            assert(
                bindingDescription.descriptorCount == 1 &&
                "Binding single descriptor info, but binding expects multiple");
            
            VkWriteDescriptorSet write{};
            write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.descriptorType = bindingDescription.descriptorType;
            write.dstBinding = binding;
            write.pBufferInfo = bufferInfo;
            write.descriptorCount = 1;
            
            writes.push_back(write);
            return *this;
        }
 

        DescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo) 
        {
            assert(setLayout.bindings.count(binding) == 1 && "Layout does not contain specified binding");
            
            auto &bindingDescription = setLayout.bindings[binding];
            
            assert(
                bindingDescription.descriptorCount == 1 &&
                "Binding single descriptor info, but binding expects multiple");
            
            VkWriteDescriptorSet write{};
            write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.descriptorType = bindingDescription.descriptorType;
            write.dstBinding = binding;
            write.pImageInfo = imageInfo;
            write.descriptorCount = 1;
            
            writes.push_back(write);
            return *this;
        }

        bool build(VkDescriptorSet &set) 
        {
            bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
            
            if (!success) 
                return false;

            overwrite(set);

            return true;
        }
        
        
        void overwrite(VkDescriptorSet &set) 
        {
            for (auto &write : writes) 
                write.dstSet = set;
        
            vkUpdateDescriptorSets(pool.device.device(), writes.size(), writes.data(), 0, nullptr);
        }

    private:
        DescriptorSetLayout &setLayout;
        DescriptorPool &pool;
        std::vector<VkWriteDescriptorSet> writes;
};

//-------------------------------------------------------------------------------//

}  // end of Vulkan namespace

#endif