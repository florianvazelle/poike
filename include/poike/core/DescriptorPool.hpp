/**
 * @file DescriptorPool.hpp
 * @brief Define DescriptorPool class
 */

#ifndef DESCRIPTORPOOL_HPP
#define DESCRIPTORPOOL_HPP

// clang-format off
#include <common/VulkanHeader.hpp>  // for VkDescriptorPool, VkDescriptorPoolCr...
#include <common/NoCopy.hpp>       // for NoCopy
namespace vkl { class Device; }
// clang-format on

namespace vkl {

  class DescriptorPool : public NoCopy {
  public:
    DescriptorPool(const Device& device, const VkDescriptorPoolCreateInfo& poolInfo);
    ~DescriptorPool();

    inline const VkDescriptorPool& handle() const { return m_pool; };

    void cleanup();
    void recreate();

  private:
    VkDescriptorPool m_pool;

    const Device& m_device;
    const VkDescriptorPoolCreateInfo& m_poolInfo;

    void createDescriptorPool();
  };
}  // namespace vkl

#endif  // DESCRIPTORPOOL_HPP