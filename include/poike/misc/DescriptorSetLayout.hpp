#pragma once

#include <poike/core/VulkanHeader.hpp>
#include <vector>

namespace poike {

  namespace misc {

    inline VkDescriptorSetLayoutBinding descriptorSetLayoutBinding(VkDescriptorType type,
                                                                   VkShaderStageFlags stageFlags,
                                                                   uint32_t binding,
                                                                   uint32_t descriptorCount = 1) {
      return {
          // valeur indiquée dans le shader
          .binding = binding,
          // type de descripteur (VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER pour uniform buffer)
          .descriptorType = type,
          // nombre d'éléments qu'il contient
          .descriptorCount = descriptorCount,
          // les différents shader qui accèderont à cette ressource (VK_SHADER_STAGE_VERTEX_BIT pour vertex shader)
          .stageFlags = stageFlags,  // or VK_SHADER_STAGE_ALL_GRAPHICS
      };
    }

    inline VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(
        const std::vector<VkDescriptorSetLayoutBinding>& bindings) {
      // Les liens des descripteurs doivent être combinés en un seul objet VkDescriptorSetLayout

      // On crée un VkDescriptorSetLayoutCreateInfo pour regrouper les descripteurs
      return {
          .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
          .bindingCount = static_cast<uint32_t>(bindings.size()),
          .pBindings    = bindings.data(),  // tableau contenant les structures qui décrivent les bindings
      };
    }

  }  // namespace misc

}  // namespace poike
