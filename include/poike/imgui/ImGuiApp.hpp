
#ifndef IMGUIAPP_HPP
#define IMGUIAPP_HPP

// clang-format off
#include <stdint.h>                              // for uint32_t
#include <poike/core/VulkanHeader.hpp>                  // for VkCommandBuffer, VkD...
#include <poike/core/CommandPool.hpp>                // for CommandPool
#include <poike/imgui/ImGuiCommandBuffers.hpp>  // for ImGuiCommandBuffers
#include <poike/imgui/ImGuiRenderPass.hpp>      // for ImGuiRenderPass
namespace poike { class Device; }
namespace poike { class GraphicsPipeline; }
namespace poike { class Instance; }
namespace poike { class SwapChain; }
namespace poike { class Window; }
// clang-format on

namespace poike {

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
}  // namespace poike

#endif  // IMGUIAPP_HPP