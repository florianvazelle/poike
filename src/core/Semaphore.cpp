// clang-format off
#include <poike/core/Semaphore.hpp>
#include <stdexcept>               // for runtime_error
#include <poike/core/Device.hpp>       // for Device
#include <poike/core/QueueFamily.hpp>  // for poike
// clang-format on

using namespace poike;

Semaphore::Semaphore(const Device& device) : m_device(device) {
  VkSemaphoreCreateInfo semaphoreCreateInfo = {
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
  };

  if (vkCreateSemaphore(m_device.logical(), &semaphoreCreateInfo, nullptr, &m_semaphore)) {
    throw std::runtime_error("Failed to create semaphore");
  }
}

Semaphore::~Semaphore() { vkDestroySemaphore(m_device.logical(), m_semaphore, nullptr); }
