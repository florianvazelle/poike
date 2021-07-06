#ifndef STORAGEBUFFER_HPP
#define STORAGEBUFFER_HPP

#include <common/VulkanHeader.hpp>
#include <stdexcept>

#include <common/Device.hpp>
#include <common/NoCopy.hpp>
#include <common/buffer/IBuffer.hpp>
#include <common/misc/Device.hpp>

#include <cstring>
#include <iostream>

namespace vkl {

  class StorageBuffer : public IBuffer {
  public:
    StorageBuffer(const Device& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
        : m_bufferSize(size), m_device(device) {
      // Voir commentaires dans la fonction createBuffer
      createBuffer(m_bufferSize, usage, properties);
    }

    ~StorageBuffer() {
      // ne dépend pas de la swap chain
      vkDestroyBuffer(m_device.logical(), m_buffer, nullptr);
      vkFreeMemory(m_device.logical(), m_bufferMemory, nullptr);
    }

    inline const VkBuffer& buffer() const { return m_buffer; }
    inline const VkDeviceMemory& memory() const { return m_bufferMemory; }
    inline const VkDeviceSize& size() const { return m_bufferSize; }
    inline const VkDescriptorBufferInfo& descriptor() const { return m_descriptor; }

    /**
     * @brief Wrapper for create a new buffer
     * @param size Is the size of the buffer you want
     * @param usage Is a flag to describe for what usage the buffer is destinate
     * @param properties Is for find the memory type
     */
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
      // Step 1 - Création du m_buffer
      const VkBufferCreateInfo bufferInfo = {
          .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
          .size        = size,
          .usage       = usage,
          .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
      };

      if (vkCreateBuffer(m_device.logical(), &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS) {
        throw std::runtime_error("echec de la creation d'un m_buffer!");
      }

      // Step 2 - Allocation de la mémoire
      // On récupére les fonctionnalités dont le m_buffer a besoin
      VkMemoryRequirements memRequirements;
      vkGetBufferMemoryRequirements(m_device.logical(), m_buffer, &memRequirements);

      // On rempli la structure VkMemoryAllocateInfo
      const VkMemoryAllocateInfo allocInfo = {
          .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
          .allocationSize  = memRequirements.size,
          .memoryTypeIndex = misc::findMemoryType(m_device.physical(), memRequirements.memoryTypeBits, properties),
      };

      // Si l'allocation a réussi, nous pouvons associer cette mémoire au m_buffer
      if (vkAllocateMemory(m_device.logical(), &allocInfo, nullptr, &m_bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("echec de l'allocation de memoire!");
      }

      // Le quatrième paramètre indique le décalage entre le début de la mémoire et le début du m_buffer
      vkBindBufferMemory(m_device.logical(), m_buffer, m_bufferMemory, 0);

      // Note : Pour allouer un grand espace mémoire, il faut que ce nombre soit divisible par
      // memRequirements.alignement

      setupDescriptor();
    }

    void setupDescriptor(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) {
      m_descriptor = {
          .buffer = m_buffer,
          .offset = offset,
          .range  = size,
      };
    }

  protected:
    VkBuffer m_buffer;
    VkDeviceMemory m_bufferMemory;
    VkDeviceSize m_bufferSize;

    VkDescriptorBufferInfo m_descriptor;

    const Device& m_device;
  };

}  // namespace vkl

#endif  // STORAGEBUFFER_HPP
