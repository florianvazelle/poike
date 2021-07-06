/**
 * @file DescriptorSets.hpp
 * @brief Define DescriptorSets base class
 */

#ifndef DESCRIPTORSETS_HPP
#define DESCRIPTORSETS_HPP

// clang-format off
#include <common/VulkanHeader.hpp>  // for VkDescriptorSet, VkDescriptorSet_T
#include <common/NoCopy.hpp>       // for NoCopy
#include <common/image/Image.hpp>
#include <vector>                // for vector
namespace vkl { class DescriptorPool; }
namespace vkl { class DescriptorSetLayout; }
namespace vkl { class Device; }
namespace vkl { class IBuffer; }
namespace vkl { class RenderPass; }
namespace vkl { class IUniformBuffers; }
namespace vkl { class SwapChain; }
// clang-format on

namespace vkl {

  class DescriptorSets : public NoCopy {
  public:
    DescriptorSets(const Device& device,
                   const SwapChain& swapChain,
                   const DescriptorSetLayout& descriptorSetLayout,
                   const DescriptorPool& descriptorPool,
                   const std::vector<const IBuffer*>& buffers,
                   const std::vector<const IUniformBuffers*>& uniformBuffers);
    // ~DescriptorSets(); no need destructor because VkDescriptorSet is deleted when pool is deleted

    inline const VkDescriptorSet& descriptor(int index) const { return m_descriptorSets.at(index); }

    void recreate();

  protected:
    std::vector<VkDescriptorSet> m_descriptorSets;

    const Device& m_device;
    const SwapChain& m_swapChain;
    const DescriptorSetLayout& m_descriptorSetLayout;
    const DescriptorPool& m_descriptorPool;
    const std::vector<const IBuffer*>& m_buffers;
    const std::vector<const IUniformBuffers*>& m_uniformBuffers;

    void allocateDescriptorSets();
    virtual void createDescriptorSets() = 0;
  };
}  // namespace vkl

#endif  // DESCRIPTORSETLAYOUT_HPP
