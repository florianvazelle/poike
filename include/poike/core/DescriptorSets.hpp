/**
 * @file DescriptorSets.hpp
 * @brief Define DescriptorSets base class
 */

#ifndef DESCRIPTORSETS_HPP
#define DESCRIPTORSETS_HPP

// clang-format off
#include <poike/core/VulkanHeader.hpp>  // for VkDescriptorSet, VkDescriptorSet_T
#include <poike/meta/NoCopy.hpp>       // for NoCopy
#include <poike/core/image/Image.hpp>
#include <vector>                // for vector
namespace poike { class DescriptorPool; }
namespace poike { class DescriptorSetLayout; }
namespace poike { class Device; }
namespace poike { class IBuffer; }
namespace poike { class RenderPass; }
namespace poike { class IUniformBuffers; }
namespace poike { class SwapChain; }
// clang-format on

namespace poike {

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
}  // namespace poike

#endif  // DESCRIPTORSETLAYOUT_HPP
