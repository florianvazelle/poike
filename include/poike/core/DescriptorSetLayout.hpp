/**
 * @file DescriptorSetLayout.hpp
 * @brief Define DescriptorSetLayout class
 */

#ifndef DESCRIPTORSETLAYOUT_HPP
#define DESCRIPTORSETLAYOUT_HPP

// clang-format off
#include <common/VulkanHeader.hpp>  // for VkDescriptorSetLayout, VkDescriptorS...
#include <common/NoCopy.hpp>       // for NoCopy
namespace vkl { class Device; }
// clang-format on

namespace vkl {

  class DescriptorSetLayout : public NoCopy {
  public:
    DescriptorSetLayout(const Device& device, const VkDescriptorSetLayoutCreateInfo& layoutInfo);
    ~DescriptorSetLayout();

    inline const VkDescriptorSetLayout& handle() const { return m_descriptorSetLayout; }

  private:
    VkDescriptorSetLayout m_descriptorSetLayout;

    const Device& m_device;
  };
}  // namespace vkl

#endif  // DESCRIPTORSETLAYOUT_HPP