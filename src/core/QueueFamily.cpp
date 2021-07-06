// clang-format off
#include <poike/core/QueueFamily.hpp>
#include <vector>  // for vector
#include <iostream>
// clang-format on

using namespace vkl;

QueueFamilyIndices QueueFamily::FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {
  QueueFamilyIndices indices;

  // Get queue families
  uint32_t familyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
  std::vector<VkQueueFamilyProperties> families(familyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families.data());

  // Iterate through families until one that supports requirements is found
  bool found = false;
  for (size_t i = 0; !found && i < families.size(); ++i) {
    const auto& family = families.at(i);

    // Check for graphics support
    if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }

    // Dedicated queue for compute
    // Try to find a queue family index that supports compute but not graphics
    if ((family.queueFlags & VK_QUEUE_COMPUTE_BIT) && ((family.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)) {
      indices.computeFamily = i;
    }

    // Dedicated queue for transfer
    // Try to find a queue family index that supports transfer but not graphics and compute
    if ((family.queueFlags & VK_QUEUE_TRANSFER_BIT) && ((family.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)
        && ((family.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)) {
      indices.transferFamily = i;
    }

    // Check for surface presentation support
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
    if (presentSupport) {
      indices.presentFamily = i;
    }

    found = indices.isComplete();
  }

  if (!indices.graphicsFamily.has_value()) {
    std::cout << "graphicsFamily: " << indices.graphicsFamily.value() << std::endl;
    indices.graphicsFamily = VK_NULL_HANDLE;
  }

  if (!indices.computeFamily.has_value()) {
    indices.computeFamily = indices.graphicsFamily;
  }

  if (!indices.transferFamily.has_value()) {
    indices.transferFamily = indices.graphicsFamily;
  }

  return indices;
}
