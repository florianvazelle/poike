/**
 * @file GraphicsPipeline.hpp
 * @brief Define GraphicsPipeline base class
 */

#ifndef GRAPHICSPIPELINE_HPP
#define GRAPHICSPIPELINE_HPP

// clang-format off
#include <stdint.h>              // for uint32_t
#include <common/VulkanHeader.hpp>  // for VK_FALSE, VkPipelineLayout, VK_TRUE
#include <common/NoCopy.hpp>       // for NoCopy
#include <common/SwapChain.hpp>  // for SwapChain
#include <vector>                // for vector
namespace vkl { class DescriptorSetLayout; }
namespace vkl { class Device; }
namespace vkl { class RenderPass; }
// clang-format on

namespace vkl {

  class GraphicsPipeline : public NoCopy {
  public:
    GraphicsPipeline(const Device& device,
                     const SwapChain& swapChain,
                     const RenderPass& renderPass,
                     const DescriptorSetLayout& descriptorSetLayout);
    virtual ~GraphicsPipeline() = default;

    virtual void recreate();

    inline const VkPipeline& pipeline() const { return m_pipeline; }
    inline const VkPipelineLayout& layout() const { return m_layout; }

  protected:
    /**
     * Note Exposé : les VkPipeline peuvent partagé le meme VkPipelineLayout
     */
    VkPipeline m_pipeline;
    VkPipelineLayout m_layout;
    VkPipelineLayout m_oldLayout;

    // so that they are not destroyed in initDefaultPipeline
    VkVertexInputBindingDescription m_bindingDescription;
    std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
    VkViewport m_viewport;
    VkRect2D m_scissor;
    VkPipelineColorBlendAttachmentState m_colorBlendAttachment;

    const Device& m_device;
    const SwapChain& m_swapChain;
    const RenderPass& m_renderPass;
    const DescriptorSetLayout& m_descriptorSetLayout;

    template <typename T> void initDefaultPipeline(VkPipelineVertexInputStateCreateInfo& vertexInputInfo,
                                                   VkPipelineInputAssemblyStateCreateInfo& inputAssembly,
                                                   VkPipelineViewportStateCreateInfo& viewportState,
                                                   VkPipelineRasterizationStateCreateInfo& rasterizer,
                                                   VkPipelineMultisampleStateCreateInfo& multisampling,
                                                   VkPipelineColorBlendStateCreateInfo& colorBlending,
                                                   VkPipelineDepthStencilStateCreateInfo& depthStencil) {
      /**
       * Vertex Input Info
       */
      m_bindingDescription    = T::getBindingDescription();
      m_attributeDescriptions = T::getAttributeDescriptions();

      vertexInputInfo = {
          .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
          .vertexBindingDescriptionCount   = 1,
          .pVertexBindingDescriptions      = &m_bindingDescription,
          .vertexAttributeDescriptionCount = static_cast<uint32_t>(m_attributeDescriptions.size()),
          .pVertexAttributeDescriptions    = m_attributeDescriptions.data(),
      };

      /**
       * On spécifie comment le GPU doit assemblée les données en entré
       * Ici on organise nos sommets en triangles
       */
      inputAssembly = {
          .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
          .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
          .primitiveRestartEnable = VK_FALSE,
      };

      /**
       * Viewport
       */

      // Pipeline viewport
      m_viewport = {
          .x      = 0.0f,
          .y      = 0.0f,
          .width  = static_cast<float>(m_swapChain.extent().width),
          .height = static_cast<float>(m_swapChain.extent().height),
          // Depth buffer range
          .minDepth = 0.0f,
          .maxDepth = 1.0f,
      };

      // Pixel boundary cutoff
      m_scissor = {
          .offset = {0, 0},
          .extent = m_swapChain.extent(),
      };

      // Combine viewport(s) and scissor(s) (some graphics cards allow multiple of each)
      viewportState = {
          .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
          .viewportCount = 1,
          .pViewports    = &m_viewport,
          .scissorCount  = 1,
          .pScissors     = &m_scissor,
      };

      /**
       * Rasterizer
       */
      rasterizer = {
          .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
          // Clip fragments instead of clipping them to near and far planes
          .depthClampEnable = VK_FALSE,
          // Don't allow the rasterizer to discard geometry
          .rasterizerDiscardEnable = VK_FALSE,
          // Fill fragments
          .polygonMode = VK_POLYGON_MODE_FILL,
          .cullMode    = VK_CULL_MODE_BACK_BIT,
          .frontFace   = VK_FRONT_FACE_COUNTER_CLOCKWISE,
          // Bias depth values
          // This is good for shadow mapping, but we're not doing that currently
          // so we'll disable for now
          .depthBiasEnable = VK_FALSE,
          .lineWidth       = 1.0f,
      };

      /**
       * Multisampling
       */
      multisampling = {
          .sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
          .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
          .sampleShadingEnable  = VK_FALSE,
          .minSampleShading     = 1.0f,
      };

      /**
       * Color Blending
       */
      m_colorBlendAttachment = {
          // Disable blending
          .blendEnable = VK_FALSE,
          .colorWriteMask
          = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
      };

      colorBlending = {
          .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
          .attachmentCount = 1,
          .pAttachments    = &m_colorBlendAttachment,
      };

      /**
       * Depth Stencil
       */
      depthStencil = {
            .sType            = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .depthTestEnable  = VK_TRUE,
            .depthWriteEnable = VK_TRUE,
            .depthCompareOp   = VK_COMPARE_OP_LESS_OR_EQUAL,  // Cull front faces
            .stencilTestEnable = VK_FALSE,
            .back = {
                .compareOp   = VK_COMPARE_OP_ALWAYS,
            },
        };
    }
    virtual void createPipeline() = 0;
    void destroyPipeline();

    VkShaderModule createShaderModule(const std::vector<unsigned char>& code);
    void deleteShaderModule(const std::vector<VkPipelineShaderStageCreateInfo>& shaderStages);
  };
}  // namespace vkl

#endif  // GRAPHICSPIPELINE_HPP