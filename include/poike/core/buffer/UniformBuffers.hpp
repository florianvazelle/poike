#ifndef UNIFORMBUFFER_HPP
#define UNIFORMBUFFER_HPP

#include <poike/core/VulkanHeader.hpp>
#include <deque>
#include <stdexcept>

#include <poike/core/Device.hpp>
#include <poike/meta/NoCopy.hpp>
#include <poike/core/SwapChain.hpp>
#include <poike/core/buffer/Buffer.hpp>
#include <poike/core/buffer/IBuffer.hpp>
#include <poike/core/struct/Depth.hpp>
#include <poike/core/struct/MVP.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstring>
#include <functional>

namespace vkl {

  /**
   * @brief A templating class to define a list of buffer
   *
   * Note Exposé : Comme des frames peuvent être "in flight" pendant que nous essayons de modifier
   * le contenu du buffer, nous allons avoir besoin de plusieurs buffers. Nous pouvons soit en avoir
   * un par frame, soit un par image de la swap chain.
   */
  template <typename T> class UniformBuffers : public IUniformBuffers {
  public:
    using UBOCallBack = std::function<void(const Device&, const SwapChain&, std::deque<Buffer<T>>&, float, uint32_t)>;

    UniformBuffers(const Device& device, const SwapChain& swapChain, UBOCallBack update)
        : m_update(update), m_device(device), m_swapChain(swapChain) {
      createUniformBuffers();
    }

    ~UniformBuffers() {}

    inline std::vector<T>& data(int i) { return m_uniformBuffers.at(i).data(); }
    inline const std::vector<T>& data(int i) const { return m_uniformBuffers.at(i).data(); }

    inline const VkBuffer& buffer(int index) const { return m_uniformBuffers[index].buffer(); }
    inline const VkDescriptorBufferInfo& descriptor(int index) const { return m_uniformBuffers[index].descriptor(); }

    /**
     * @brief Génére une rotation à chaque frame pour que la géométrie tourne sur elle-même
     * @param currentImage C'est la frame courante
     */
    inline void update(float time, uint32_t currentImage) {
      m_update(m_device, m_swapChain, m_uniformBuffers, time, currentImage);
    }

    /**
     * @brief Recreate all uniform buffers when swapchain is recreate
     */
    void recreate() {
      m_uniformBuffers.clear();  // -> call destructor of all Buffer
      createUniformBuffers();
    }

  protected:
    std::deque<Buffer<T>> m_uniformBuffers;
    UBOCallBack m_update;

    const Device& m_device;
    const SwapChain& m_swapChain;

    /**
     * @brief Create uniform buffers according to the swapchain's image count
     */
    void createUniformBuffers() {
      m_uniformBuffers.clear();

      for (size_t i = 0; i < m_swapChain.numImages(); i++) {
        m_uniformBuffers.emplace_back(m_device, std::vector<T>(1), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      }
    }
  };
}  // namespace vkl

#endif  // VERTEX_HPP
