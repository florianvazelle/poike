/**
 * @file Application.hpp
 * @brief Define Application base class
 */

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

// clang-format off
#include <poike/meta/NoCopy.hpp>               // for NoCopy
#include <poike/meta/NoMove.hpp>               // for NoMove
#include <poike/core/DebugUtilsMessenger.hpp>  // for DebugUtilsMessenger
#include <poike/core/Device.hpp>               // for Device
#include <poike/core/Instance.hpp>             // for Instance
#include <poike/core/SwapChain.hpp>            // for SwapChain
#include <poike/core/SyncObjects.hpp>          // for SyncObjects
#include <poike/core/Window.hpp>               // for Window
#include <string>                          // for allocator, string
#include <iostream>
// clang-format on

#define MAX_FRAMES_IN_FLIGHT 2

#define WIDTH 800
#define HEIGHT 600

#define ENGINE_NAME "No Engine"

/**
 * @brief Vulkan Loader
 *
 * Encapsulates the entire vk3DLoader library.
 */
namespace poike {

  struct DebugOption {
    int debugLevel;
    bool exitOnError;
  };

  class Application : public NoCopy, NoMove {
  public:
    Application(
#ifdef __ANDROID__
        android_app* androidApp,
#endif
        const std::string& appName,
        const DebugOption& debugOption);

#ifndef __ANDROID__
    static void initialize() {
      // need to init glfw first, to get the suitable glfw extension for the vkinstance
      glfwInit();

      // Disable OpenGL
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }

    static void terminate() { glfwTerminate(); }
#endif

  protected:
    Instance instance;
    DebugUtilsMessenger debugMessenger;
    Window window;
    Device device;
    SwapChain swapChain;
    SyncObjects syncObjects;

    size_t currentFrame = 0;

    VkResult prepareFrame(bool useFences, bool& framebufferResized, uint32_t& imageIndex);
    void submitFrame(bool useFences, bool& framebufferResized, const uint32_t& imageIndex);

    virtual void recreateSwapChain(bool& framebufferResized) = 0;
  };
}  // namespace poike

#endif  // APPLICATION_HPP
