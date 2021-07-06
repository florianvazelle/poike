/**
 * @file Buffer.hpp
 * @brief Define Buffer template class
 * @todo Refactor so as not to have to define a class each time we want to change the update method
 */

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <poike/core/VulkanHeader.hpp>
#include <cstring>  // memcpy
#include <stdexcept>

#include <poike/core/Device.hpp>
#include <poike/core/buffer/StorageBuffer.hpp>

namespace vkl {

  /**
   * @brief A templating class to define buffer
   */
  template <typename T> class Buffer : public StorageBuffer {
  public:
    Buffer(const Device& device,
           const std::vector<T>& bufferData,
           VkDeviceSize size,
           VkBufferUsageFlags usage,
           VkMemoryPropertyFlags properties)
        : StorageBuffer(device, size, usage, properties), m_bufferData(bufferData) {}

    Buffer(const Device& device,
           const std::vector<T>& bufferData,
           VkBufferUsageFlags usage,
           VkMemoryPropertyFlags properties)
        : Buffer(device, bufferData, sizeof(T) * bufferData.size(), usage, properties) {
      // Step 3 - Remplissage du vertex buffer
      void* data;

      /**
       * Note Exposé : La fonction vkMapMemory permet d'accéder à une région spécifique d'une ressource.
       * Il faut indiquer un décalage et une taille. On met ici 0 et bufferInfo.size.
       * L'avant-dernier paramètre est un champ de bits (flags) pour l'instant non implémenté par
       * Vulkan. Il est impératif de le laisser à 0. "is reserved for future use"
       * Le dernier paramètre permet de fournir un pointeur vers la mémoire mappée.
       */
      vkMapMemory(m_device.logical(), m_bufferMemory, 0, m_bufferSize, 0, &data);
      memcpy(data, m_bufferData.data(), (size_t)m_bufferSize);
      vkUnmapMemory(m_device.logical(), m_bufferMemory);
    }

    inline std::vector<T>& data() { return m_bufferData; }
    inline const std::vector<T>& data() const { return m_bufferData; }

    /**
     * @brief Just update the buffer, for example if in imgui with change the value of bufferData
     */
    void update(float time, uint32_t currentImage) {
      void* data;
      vkMapMemory(m_device.logical(), m_bufferMemory, 0, m_bufferSize, 0, &data);
      memcpy(data, m_bufferData.data(), m_bufferSize);
      vkUnmapMemory(m_device.logical(), m_bufferMemory);
    }

  protected:
    std::vector<T> m_bufferData;  // normaly is struct type like Depth, MVP, Vertex, Material ...
  };

}  // namespace vkl

#endif  // BUFFER_HPP