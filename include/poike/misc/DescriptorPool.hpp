#pragma once

#include <common/VulkanHeader.hpp>
#include <vector>

namespace vkl {

  /**
   * @brief Minimal instruction set computer
   *
   * Encapsulates minimal inline functions to simplify creation of vulkan structure.
   */
  namespace misc {

    inline VkDescriptorPoolSize descriptorPoolSize(VkDescriptorType type, uint32_t descriptorCount) {
      return {
          // indique les types de descripteurs
          .type = type,
          // et combien sont compris dans les sets (de descripteurs)
          .descriptorCount = descriptorCount,
      };
    }

    inline VkDescriptorPoolCreateInfo descriptorPoolCreateInfo(const std::vector<VkDescriptorPoolSize>& poolSizes,
                                                               uint32_t maxSets) {
      return {
          .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
          .maxSets       = maxSets,  // nombre maximum de sets à allouer
          .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
          .pPoolSizes    = poolSizes.data(),
      };
    }

  }  // namespace misc

}  // namespace vkl
