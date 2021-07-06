#ifndef IMGUICOMMANDBUFFERS_HPP
#define IMGUICOMMANDBUFFERS_HPP

// clang-format off
#include <stdint.h>                   // for uint32_t
#include <common/CommandBuffers.hpp>  // for CommandBuffers
namespace vkl { class CommandPool; }
namespace vkl { class Device; }
namespace vkl { class GraphicsPipeline; }
namespace vkl { class RenderPass; }
namespace vkl { class SwapChain; }
// clang-format on

namespace vkl {

  class ImGuiCommandBuffers : public CommandBuffersBase {
  public:
    ImGuiCommandBuffers(const Device& device,
                        const RenderPass& renderPass,
                        const SwapChain& swapChain,
                        const GraphicsPipeline& graphicsPipeline,
                        const CommandPool& commandPool);

    void recordCommandBuffers(uint32_t bufferIdx);
    void recreate();

  private:
    void createCommandBuffers();
  };

}  // namespace vkl

#endif  // IMGUICOMMANDBUFFERS_HPP