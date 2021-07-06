/**
 * @file CommandBuffers.hpp
 * @brief Define CommandBuffers base class
 */

#ifndef COMMANDBUFFERS_HPP
#define COMMANDBUFFERS_HPP

#include <common/CommandPool.hpp>
#include <common/DescriptorSets.hpp>
#include <common/Device.hpp>
#include <common/GraphicsPipeline.hpp>
#include <common/NoCopy.hpp>
#include <common/RenderPass.hpp>
#include <common/SwapChain.hpp>
#include <common/VulkanHeader.hpp>
#include <functional>
#include <vector>

namespace vkl {

  class CommandBuffersBase : public NoCopy {
  public:
    CommandBuffersBase(const Device& device,
                       const RenderPass& renderpass,
                       const SwapChain& swapChain,
                       const GraphicsPipeline& graphicsPipeline,
                       const CommandPool& commandPool);
    ~CommandBuffersBase();

    inline VkCommandBuffer& command(uint32_t index) { return m_commandBuffers[index]; }
    inline const VkCommandBuffer& command(uint32_t index) const { return m_commandBuffers[index]; }

  protected:
    std::vector<VkCommandBuffer> m_commandBuffers;

    const Device& m_device;
    const RenderPass& m_renderPass;
    const SwapChain& m_swapChain;
    const GraphicsPipeline& m_graphicsPipeline;
    const CommandPool& m_commandPool;

    void destroyCommandBuffers();
  };

  class CommandBuffers : public CommandBuffersBase {
  public:
    CommandBuffers(const Device& device,
                   const RenderPass& renderpass,
                   const SwapChain& swapChain,
                   const GraphicsPipeline& graphicsPipeline,
                   const CommandPool& commandPool,
                   const DescriptorSets& descriptorSets,
                   const std::vector<const IBuffer*>& buffers);

    void recreate();
    static void SingleTimeCommands(const Device& device,
                                   const CommandPool& cmdPool,
                                   const VkQueue& queue,
                                   const std::function<void(const VkCommandBuffer&)>& func);

  protected:
    const DescriptorSets& m_descriptorSets;
    const std::vector<const IBuffer*>& m_buffers;

    virtual void createCommandBuffers() = 0;
  };
}  // namespace vkl

#endif  // COMMANDBUFFERS_HPP
