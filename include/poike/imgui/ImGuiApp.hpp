
#ifndef IMGUIAPP_HPP
#define IMGUIAPP_HPP

// clang-format off
#include <stdint.h>                              // for uint32_t
#include <common/VulkanHeader.hpp>                  // for VkCommandBuffer, VkD...
#include <common/CommandPool.hpp>                // for CommandPool
#include <common/ImGui/ImGuiCommandBuffers.hpp>  // for ImGuiCommandBuffers
#include <common/ImGui/ImGuiRenderPass.hpp>      // for ImGuiRenderPass
namespace vkl { class Device; }
namespace vkl { class GraphicsPipeline; }
namespace vkl { class Instance; }
namespace vkl { class SwapChain; }
namespace vkl { class Window; }
// clang-format on

namespace vkl {

  class ImGuiApp {
  public:
    ImGuiApp(const Instance& instance,
             Window& window,
             const Device& device,
             const SwapChain& swapChain,
             const GraphicsPipeline& graphicsPipeline);
    ~ImGuiApp();

    inline VkCommandBuffer& command(uint32_t index) { return commandBuffers.command(index); }
    inline const VkCommandBuffer& command(uint32_t index) const { return commandBuffers.command(index); }

    void recordCommandBuffers(uint32_t bufferIdx) { commandBuffers.recordCommandBuffers(bufferIdx); }

    void recreate();

  private:
    VkDescriptorPool imGuiDescriptorPool;

    ImGuiRenderPass renderPass;
    CommandPool commandPool;
    ImGuiCommandBuffers commandBuffers;

    const Instance& m_instance;
    const Device& m_device;
    const SwapChain& m_swapChain;
    const GraphicsPipeline& m_graphicsPipeline;

    void createImGuiDescriptorPool();
  };
}  // namespace vkl

#endif  // IMGUIAPP_HPP