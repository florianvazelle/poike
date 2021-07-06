/**
 * @file Device.hpp
 * @brief Define Device class
 */

#ifndef DEVICE_HPP
#define DEVICE_HPP

// clang-format off
#include <common/VulkanHeader.hpp>    // for VkPhysicalDevice, VkQueue, VkDevice
#include <common/NoCopy.hpp>         // for NoCopy
#include <common/QueueFamily.hpp>  // for QueueFamilyIndices
#include <vector>                  // for vector
namespace vkl { class Instance; }
namespace vkl { class Window; }
// clang-format on

namespace vkl {

  /**
   * @brief  A class which allows to manage devices.
   *
   * The devices allow you to interact with our Vulkan application. There are two types of device: physical and logical.
   * A physical device is a single component of the system. It can also be several components working together to
   * function as a single device. A logical device is our interface to the physical device.
   */
  class Device : public NoCopy {
  public:
    Device(const Instance& instance, const Window& window, const std::vector<const char*>& extensions);
    ~Device();

    inline const VkPhysicalDevice& physical() const { return m_physical; }
    inline const VkDevice& logical() const { return m_logical; }
    inline const QueueFamilyIndices& queueFamilyIndices() const { return m_indices; }

    inline const VkQueue& graphicsQueue() const { return m_graphicsQueue; }
    inline const VkQueue& computeQueue() const { return m_computeQueue; }
    inline const VkQueue& transferQueue() const { return m_transferQueue; }
    inline const VkQueue& presentQueue() const { return m_presentQueue; }

  private:
    VkPhysicalDevice m_physical;
    VkDevice m_logical;

    const Instance& m_instance;
    const Window& m_window;

    QueueFamilyIndices m_indices;
    VkQueue m_graphicsQueue;
    VkQueue m_computeQueue;
    VkQueue m_transferQueue;
    VkQueue m_presentQueue;

    static bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device, const std::vector<const char*>& extensions);
    static uint32_t GetQueueFamilyIndex(const std::vector<VkQueueFamilyProperties>& families,
                                        const VkQueueFlagBits& queueFlags);

    static VkPhysicalDevice PickPhysicalDevice(const VkInstance& instance,
                                               const VkSurfaceKHR& surface,
                                               const std::vector<const char*>& requiredExtensions);

    static bool IsDeviceSuitable(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
  };
}  // namespace vkl

#endif  // DEVICE_HPP
