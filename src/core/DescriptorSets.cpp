// clang-format off
#include <poike/core/DescriptorSets.hpp>
#include <stddef.h>                        // for size_t
#include <stdint.h>                        // for uint32_t
#include <poike/core/DescriptorPool.hpp>       // for DescriptorPool
#include <poike/core/DescriptorSetLayout.hpp>  // for DescriptorSetLayout
#include <poike/core/Device.hpp>               // for Device
#include <poike/core/SwapChain.hpp>            // for SwapChain
#include <poike/misc/DescriptorSet.hpp>   // for descriptorSetAllocateInfo
#include <stdexcept>                       // for runtime_error
// clang-format on

using namespace vkl;

DescriptorSets::DescriptorSets(const Device& device,
                               const SwapChain& swapChain,
                               const DescriptorSetLayout& descriptorSetLayout,
                               const DescriptorPool& descriptorPool,
                               const std::vector<const IBuffer*>& buffers,
                               const std::vector<const IUniformBuffers*>& uniformBuffers)
    : m_device(device),
      m_swapChain(swapChain),
      m_descriptorSetLayout(descriptorSetLayout),
      m_descriptorPool(descriptorPool),
      m_buffers(buffers),
      m_uniformBuffers(uniformBuffers) {}

void DescriptorSets::recreate() { createDescriptorSets(); }

void DescriptorSets::allocateDescriptorSets() {
  /* Allocate */
  const size_t size = m_swapChain.numImages();

  const std::vector<VkDescriptorSetLayout> layouts(size, m_descriptorSetLayout.handle());
  const VkDescriptorSetAllocateInfo allocInfo
      = misc::descriptorSetAllocateInfo(m_descriptorPool.handle(), layouts.data(), static_cast<uint32_t>(size));

  m_descriptorSets.resize(size);
  if (vkAllocateDescriptorSets(m_device.logical(), &allocInfo, m_descriptorSets.data()) != VK_SUCCESS) {
    throw std::runtime_error("echec de l'allocation d'un set de descripteurs!");
  }
}
