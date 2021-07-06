// clang-format off
#include <common/GraphicsPipeline.hpp>
#include <common/Device.hpp>     // for Device
#include <common/SwapChain.hpp>  // for vkl
#include <stdexcept>             // for runtime_error
#include <iostream>
// clang-format on

using namespace vkl;

GraphicsPipeline::GraphicsPipeline(const Device& device,
                                   const SwapChain& swapChain,
                                   const RenderPass& renderPass,
                                   const DescriptorSetLayout& descriptorSetLayout)
    : m_pipeline(VK_NULL_HANDLE),
      m_layout(VK_NULL_HANDLE),
      m_oldLayout(VK_NULL_HANDLE),

      m_device(device),
      m_swapChain(swapChain),
      m_renderPass(renderPass),
      m_descriptorSetLayout(descriptorSetLayout) {}

void GraphicsPipeline::recreate() {
  destroyPipeline();
  createPipeline();
}

void GraphicsPipeline::destroyPipeline() {
  vkDestroyPipeline(m_device.logical(), m_pipeline, nullptr);
  vkDestroyPipelineLayout(m_device.logical(), m_layout, nullptr);
}

// TODO : maybe externilize shader module
VkShaderModule GraphicsPipeline::createShaderModule(const std::vector<unsigned char>& code) {
  const VkShaderModuleCreateInfo createInfo = {
      .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
      .codeSize = code.size(),
      // Vector class already ensures that the data is correctly aligned,
      // so no need to manually do it
      .pCode = reinterpret_cast<const uint32_t*>(code.data()),
  };

  VkShaderModule module;
  if (vkCreateShaderModule(m_device.logical(), &createInfo, nullptr, &module) != VK_SUCCESS) {
    throw std::runtime_error("failed to create shader module!");
  }

  return module;
}

void GraphicsPipeline::deleteShaderModule(const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages) {
  for (auto& shader : shaderStages) {
    vkDestroyShaderModule(m_device.logical(), shader.module, nullptr);
  }
}