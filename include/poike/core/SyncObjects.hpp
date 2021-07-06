/**
 * @file SyncObjects.hpp
 * @brief Define SyncObjects class
 */

#ifndef SYNCOBJECTS_HPP
#define SYNCOBJECTS_HPP

// clang-format off
#include <stdint.h>              // for uint32_t
#include <poike/core/VulkanHeader.hpp>  // for VkFence, VkSemaphore
#include <poike/meta/NoCopy.hpp>       // for NoCopy
#include <vector>                // for vector
namespace vkl { class Device; }
// clang-format on

namespace vkl {
  class Device;

  /**
   * Note Exposé : Il y a deux manières de synchroniser les évènements de la swap chain : les fences
   * et les sémaphores.
   *
   * Une fence peut être accédé depuis le programme à l'aide de fonctions telles que vkWaitForFences
   * alors que les sémaphores ne le permettent pas.
   * Les fences sont généralement utilisées pour synchroniser votre programme avec les opérations
   * alors que les sémaphores synchronisent les opérations entre elles.
   */

  class SyncObjects : public NoCopy {
  public:
    SyncObjects(const Device& device, uint32_t numImages, uint32_t maxFramesInFlight);
    ~SyncObjects();

    inline VkSemaphore& imageAvailable(uint32_t index) { return m_imageAvailable[index]; }
    inline VkSemaphore& renderFinished(uint32_t index) { return m_renderFinished[index]; }
    inline VkFence& inFlightFence(uint32_t index) { return m_inFlightFences[index]; }
    inline VkFence& imageInFlight(uint32_t index) { return m_imagesInFlight[index]; }

  private:
    const Device& m_device;

    uint32_t m_numImages, m_maxFramesInFlight;

    std::vector<VkSemaphore> m_imageAvailable;
    std::vector<VkSemaphore> m_renderFinished;
    std::vector<VkFence> m_inFlightFences;
    std::vector<VkFence> m_imagesInFlight;
  };

}  // namespace vkl

#endif  // SYNCOBJECTS_HPP