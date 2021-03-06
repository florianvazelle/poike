/**
 * @file DescriptorSetLayout.hpp
 * @brief Define DescriptorSetLayout class
 */

#ifndef DESCRIPTORSETLAYOUT_HPP
#define DESCRIPTORSETLAYOUT_HPP

// clang-format off
#include <poike/core/VulkanHeader.hpp>  // for VkDescriptorSetLayout, VkDescriptorS...
#include <poike/meta/NoCopy.hpp>       // for NoCopy
namespace poike { class Device; }
// clang-format on

namespace poike {

  class DescriptorSetLayout : public NoCopy {
  public:
    DescriptorSetLayout(const Device& device, const VkDescriptorSetLayoutCreateInfo& layoutInfo);
    ~DescriptorSetLayout();

    inline const VkDescriptorSetLayout& handle() const { return m_descriptorSetLayout; }

  private:
    VkDescriptorSetLayout m_descriptorSetLayout;

    const Device& m_device;
  };
}  // namespace poike

#endif  // DESCRIPTORSETLAYOUT_HPP