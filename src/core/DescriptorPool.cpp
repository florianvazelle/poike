// clang-format off
#include <poike/core/DescriptorPool.hpp>
#include <stdexcept>               // for runtime_error
#include <poike/core/Device.hpp>       // for Device
#include <poike/core/QueueFamily.hpp>  // for vkl
// clang-format on

using namespace vkl;

DescriptorPool::DescriptorPool(const Device& device, const VkDescriptorPoolCreateInfo& poolInfo)
    : m_device(device), m_poolInfo(poolInfo) {
  createDescriptorPool();
}

DescriptorPool::~DescriptorPool() { cleanup(); }

void DescriptorPool::cleanup() { vkDestroyDescriptorPool(m_device.logical(), m_pool, nullptr); }

void DescriptorPool::recreate() {
  cleanup();
  createDescriptorPool();
}

void DescriptorPool::createDescriptorPool() {
  if (vkCreateDescriptorPool(m_device.logical(), &m_poolInfo, nullptr, &m_pool) != VK_SUCCESS) {
    throw std::runtime_error("echec de la creation de la pool de descripteurs!");
  }
}