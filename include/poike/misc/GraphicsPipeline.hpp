#pragma once

#include <common/GraphicsPipeline.hpp>
#include <common/VulkanHeader.hpp>
#include <vector>

namespace vkl {

  namespace misc {

    inline VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo(const VkShaderModule& shaderModule,
                                                                         VkShaderStageFlagBits stage) {
      return {
          .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
          .stage  = stage,
          .module = shaderModule,
          .pName  = "main",
      };
    }

    inline VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
        const std::vector<VkDynamicState>& pDynamicStates,
        VkPipelineDynamicStateCreateFlags flags = 0) {
      return {
          .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
          .flags             = flags,
          .dynamicStateCount = static_cast<uint32_t>(pDynamicStates.size()),
          .pDynamicStates    = pDynamicStates.data(),
      };
    }

  }  // namespace misc

}  // namespace vkl
