// clang-format off
#include <common/DescriptorSetLayout.hpp>
#include <stdexcept>               // for runtime_error
#include <common/Device.hpp>       // for Device
#include <common/QueueFamily.hpp>  // for vkl
// clang-format on

using namespace vkl;

DescriptorSetLayout::DescriptorSetLayout(const Device& device, const VkDescriptorSetLayoutCreateInfo& layoutInfo)
    : m_device(device) {
  // On crée le VkDescriptorSetLayout
  if (vkCreateDescriptorSetLayout(m_device.logical(), &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS) {
    throw std::runtime_error("echec de la creation d'un set de descripteurs!");
  }
}

DescriptorSetLayout::~DescriptorSetLayout() {
  vkDestroyDescriptorSetLayout(m_device.logical(), m_descriptorSetLayout, nullptr);
}