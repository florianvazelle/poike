/**
 * @file QueueFamily.hpp
 * @brief Define QueueFamily class and QueueFamilyIndices struct
 */

#ifndef QUEUEFAMILY_HPP
#define QUEUEFAMILY_HPP

// clang-format off
#include <stdint.h>              // for uint32_t
#include <common/VulkanHeader.hpp>  // for VkPhysicalDevice, VkSurfaceKHR
#include <optional>              // for optional
// clang-format on

namespace vkl {
  struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;  // Drawing command support
    std::optional<uint32_t> computeFamily;
    std::optional<uint32_t> transferFamily;
    std::optional<uint32_t> presentFamily;  // Support for drawing to surface

    inline bool isComplete() {
      return graphicsFamily.has_value() && computeFamily.has_value() && transferFamily.has_value()
             && presentFamily.has_value();
    }
  };

  class QueueFamily {
  public:
    QueueFamily()  = delete;
    ~QueueFamily() = delete;

    static QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
  };
}  // namespace vkl

#endif  // QUEUEFAMILY_HPP
