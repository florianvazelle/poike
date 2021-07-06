#pragma once

#include <poike/core/VulkanHeader.hpp>
#include <vector>

namespace poike {

  namespace misc {

    inline VkDescriptorSetAllocateInfo descriptorSetAllocateInfo(VkDescriptorPool descriptorPool,
                                                                 const VkDescriptorSetLayout* pSetLayouts,
                                                                 uint32_t descriptorSetCount) {
      return {
          .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
          .descriptorPool     = descriptorPool,
          .descriptorSetCount = descriptorSetCount,
          .pSetLayouts        = pSetLayouts,
      };
    }

    inline VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet dstSet,
                                                   VkDescriptorType type,
                                                   uint32_t binding,
                                                   const VkDescriptorBufferInfo* bufferInfo,
                                                   uint32_t descriptorCount = 1) {
      return {
          .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
          .dstSet          = dstSet,   // set à mettre à jour
          .dstBinding      = binding,  // indice du binding
          .dstArrayElement = 0,        // indice du premier élément du tableau à modifier
          .descriptorCount = descriptorCount,
          .descriptorType  = type,
          .pBufferInfo     = bufferInfo,
      };
    }

    inline VkWriteDescriptorSet writeDescriptorSet(VkDescriptorSet dstSet,
                                                   VkDescriptorType type,
                                                   uint32_t binding,
                                                   const VkDescriptorImageInfo* imageInfo,
                                                   uint32_t descriptorCount = 1) {
      return {
          .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
          .dstSet          = dstSet,
          .dstBinding      = binding,
          .descriptorCount = descriptorCount,
          .descriptorType  = type,
          .pImageInfo      = imageInfo,
      };
    }

  }  // namespace misc

}  // namespace poike
