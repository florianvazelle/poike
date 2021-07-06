#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <common/CommandBuffers.hpp>
#include <common/CommandPool.hpp>
#include <common/buffer/Buffer.hpp>
#include <common/image/Image.hpp>

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>

namespace vkl {

  class Texture : public Image {
  public:
    Texture(const Device& device, const CommandPool& commandPool, const std::string& filename)
        : Image(device), m_commandPool(commandPool) {
      int texWidth, texHeight, texChannels;
      stbi_uc* pixels        = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
      VkDeviceSize imageSize = texWidth * texHeight * 4;

      if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
      }

      std::vector<stbi_uc> pixelConv(pixels, pixels + imageSize);
      Buffer<stbi_uc> textureBuffer(m_device, pixelConv, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      stbi_image_free(pixels);

      createImage(texWidth, texHeight);
      allocateMemory();

      copyBufferToImage(textureBuffer.buffer(), static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

      createImageView();
      createSampler();
    };

  private:
    const CommandPool& m_commandPool;

    void createImage(uint32_t width, uint32_t height) final {
      const VkImageCreateInfo imageInfo = {
          .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
          .imageType     = VK_IMAGE_TYPE_2D,
          .format        = VK_FORMAT_R8G8B8A8_SRGB,
          .extent = {
            .width  = width,
            .height = height,
            .depth  = 1,
          },
          .mipLevels     = 1,
          .arrayLayers   = 1,
          .samples       = VK_SAMPLE_COUNT_1_BIT,
          .tiling        = VK_IMAGE_TILING_OPTIMAL,
          .usage         = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
          .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
          .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      };

      if (vkCreateImage(m_device.logical(), &imageInfo, nullptr, &m_image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
      }
    };

    void createImageView() final {
      const VkImageViewCreateInfo viewInfo = {
          .sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
          .image                           = m_image,
          .viewType                        = VK_IMAGE_VIEW_TYPE_2D,
          .format                          = VK_FORMAT_R8G8B8A8_SRGB,
          .subresourceRange = {
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1,
          },
      };

      if (vkCreateImageView(m_device.logical(), &viewInfo, nullptr, &m_imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
      }
    };

    void createSampler() final {
      const VkSamplerCreateInfo samplerInfo = {
          .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
          .magFilter               = VK_FILTER_LINEAR,
          .minFilter               = VK_FILTER_LINEAR,
          .mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR,
          .addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
          .addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
          .addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
          .anisotropyEnable        = VK_FALSE,
          .compareEnable           = VK_FALSE,
          .compareOp               = VK_COMPARE_OP_ALWAYS,
          .borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
          .unnormalizedCoordinates = VK_FALSE,
      };

      if (vkCreateSampler(m_device.logical(), &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
      }
    };

    // Transfomation de donnée du buffer vers vkImage pour pouvoir traiter
    void copyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height) {
      CommandBuffers::SingleTimeCommands(
          m_device, m_commandPool, m_device.graphicsQueue(), [&](const VkCommandBuffer& commandBuffer) {
            const VkImageSubresourceRange subresourceRange = {
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel   = 0,
                .levelCount     = 1,
                .baseArrayLayer = 0,
                .layerCount     = 1,
            };

            const VkImageMemoryBarrier aquire_barrier = {
                .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                .srcAccessMask       = 0,
                .dstAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT,
                .oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED,
                .newLayout           = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .image               = m_image,
                .subresourceRange    = subresourceRange,
            };

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0,
                                 nullptr, 0, nullptr, 1, &aquire_barrier);

            const VkImageSubresourceLayers imageSubresource = {
                .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipLevel       = 0,
                .baseArrayLayer = 0,
                .layerCount     = 1,
            };

            const VkBufferImageCopy region = {
                .bufferOffset      = 0,
                .bufferRowLength   = 0,
                .bufferImageHeight = 0,
                .imageSubresource  = imageSubresource,
                .imageOffset       = {0, 0, 0},
                .imageExtent       = {width, height, 1},
            };

            vkCmdCopyBufferToImage(commandBuffer, buffer, m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            const VkImageMemoryBarrier release_barrier = {
                .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                .srcAccessMask       = VK_ACCESS_TRANSFER_WRITE_BIT,
                .dstAccessMask       = VK_ACCESS_SHADER_READ_BIT,
                .oldLayout           = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                .newLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                .image               = m_image,
                .subresourceRange    = subresourceRange,
            };

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                 0, 0, nullptr, 0, nullptr, 1, &release_barrier);
          });
    }
  };

}  // namespace vkl

#endif  // TEXTURE_HPP
