// clang-format off
#include <common/Semaphore.hpp>
#include <stdexcept>               // for runtime_error
#include <common/Device.hpp>       // for Device
#include <common/QueueFamily.hpp>  // for vkl
// clang-format on

using namespace vkl;

Semaphore::Semaphore(const Device& device) : m_device(device) {
  VkSemaphoreCreateInfo semaphoreCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
  };

  if (vkCreateSemaphore(m_device.logical(), &semaphoreCreateInfo, nullptr, &m_semaphore)) {
    throw std::runtime_error("Failed to create semaphore");
  }
}

Semaphore::~Semaphore() { vkDestroySemaphore(m_device.logical(), m_semaphore, nullptr); }
