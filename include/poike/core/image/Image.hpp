#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <common/VulkanHeader.hpp>
#include <common/Device.hpp>
#include <common/misc/Device.hpp>
#include <common/NoCopy.hpp>

namespace vkl {

  class Image : public NoCopy {
  public:
    Image(const Device& device) : m_device(device) {};

    ~Image() {
      vkDestroySampler(m_device.logical(), m_sampler, nullptr);
      vkDestroyImageView(m_device.logical(), m_imageView, nullptr);

      vkDestroyImage(m_device.logical(), m_image, nullptr);
      vkFreeMemory(m_device.logical(), m_bufferMemory, nullptr);
    }

    inline const VkImage& image() const { return m_image; }
    inline const VkImageView& view() const { return m_imageView; }
    inline const VkSampler& sample() const { return m_sampler; }

  protected:
    VkImage m_image;
    VkDeviceMemory m_bufferMemory;
    VkImageView m_imageView;
    VkSampler m_sampler;

    const Device& m_device;

    virtual void createImage(uint32_t width, uint32_t height) = 0;

    void allocateMemory() {
      VkMemoryRequirements memReqs;
      // Returns the memory requirements for specified Vulkan object
      vkGetImageMemoryRequirements(m_device.logical(), m_image, &memReqs);

      uint32_t memTypeIndex = misc::findMemoryType(m_device.physical(), memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

      const VkMemoryAllocateInfo memAlloc = {
          .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
          .allocationSize  = memReqs.size,
          .memoryTypeIndex = memTypeIndex,
      };

      if (vkAllocateMemory(m_device.logical(), &memAlloc, nullptr, &m_bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("vkAllocateMemory failed");
      }

      if (vkBindImageMemory(m_device.logical(), m_image, m_bufferMemory, 0) != VK_SUCCESS) {
        throw std::runtime_error("vkBindImageMemory failed");
      }
    }

    virtual void createImageView() = 0;
    virtual void createSampler() = 0;
  };

}  // namespace vkl

#endif  // IMAGE_HPP
