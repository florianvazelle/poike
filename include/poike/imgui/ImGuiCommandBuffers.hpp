#ifndef IMGUICOMMANDBUFFERS_HPP
#define IMGUICOMMANDBUFFERS_HPP

// clang-format off
#include <stdint.h>                   // for uint32_t
#include <poike/core/CommandBuffers.hpp>  // for CommandBuffers
namespace poike { class CommandPool; }
namespace poike { class Device; }
namespace poike { class GraphicsPipeline; }
namespace poike { class RenderPass; }
namespace poike { class SwapChain; }
// clang-format on

namespace poike {

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

}  // namespace poike

#endif  // IMGUICOMMANDBUFFERS_HPP