#include <poike/core/CommandPool.hpp>
#include <poike/core/Device.hpp>

#include <stdexcept>

using namespace vkl;

CommandPool::CommandPool(const Device& device,
                         const VkCommandPoolCreateFlags& flags,
                         std::optional<uint32_t> queueFamilyIndex)
    : m_device(device) {
  if (!queueFamilyIndex.has_value()) {
    queueFamilyIndex = m_device.queueFamilyIndices().graphicsFamily;
  }

  const VkCommandPoolCreateInfo poolInfo = {
      .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .flags            = flags,
      .queueFamilyIndex = queueFamilyIndex.value(),
  };

  if (vkCreateCommandPool(m_device.logical(), &poolInfo, nullptr, &m_pool) != VK_SUCCESS) {
    throw std::runtime_error("failed to create command pool!");
  }
}

CommandPool::~CommandPool() { vkDestroyCommandPool(m_device.logical(), m_pool, nullptr); }
