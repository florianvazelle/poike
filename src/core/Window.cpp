// clang-format off
#include <common/Window.hpp>
#include <stdint.h>                  // for uint32_t
#include <common/Instance.hpp>       // for Instance
#include <stdexcept>                 // for runtime_error
#include <glm/detail/type_vec2.hpp>  // for vec<>::(anonymous)
// clang-format on

using namespace vkl;

#ifdef __ANDROID__
Window::Window(android_app* androidApp,
               const glm::ivec2& dimensions,
               const std::string& title,
               const Instance& instance)
    : m_androidApp(androidApp),
      m_dimensions(dimensions),
      m_title(title),
      m_instance(instance),
      m_surface(VK_NULL_HANDLE),
      m_framebufferResized(true),
      m_drawFrameFunc([](bool&) {}) {
  VkAndroidSurfaceCreateInfoKHR createInfo{
      .sType  = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
      .window = m_androidApp->window,
  };

  if (vkCreateAndroidSurfaceKHR(instance.handle(), &createInfo, nullptr, &m_surface) != VK_SUCCESS) {
    throw std::runtime_error("Unable to create window surface");
  }
}
#else
Window::Window(const glm::ivec2& dimensions, const std::string& title, const Instance& instance)
    : m_dimensions(dimensions),
      m_title(title),
      m_instance(instance),
      m_surface(VK_NULL_HANDLE),
      m_framebufferResized(true),
      m_drawFrameFunc([](bool&) {}) {
  m_window = glfwCreateWindow(dimensions.x, dimensions.y, title.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(m_window, this);
  glfwSetFramebufferSizeCallback(m_window, FramebufferResizeCallback);

  if (glfwCreateWindowSurface(instance.handle(), m_window, nullptr, &m_surface) != VK_SUCCESS) {
    throw std::runtime_error("Unable to create window surface");
  }
}
#endif

Window::~Window() {
  vkDestroySurfaceKHR(m_instance.handle(), m_surface, nullptr);
#ifndef __ANDROID__
  glfwDestroyWindow(m_window);
#endif
}

void Window::mainLoop() {
#ifdef __ANDROID__
  // Used to poll the events in the main loop
  int events;
  android_poll_source* source;

  do {
    if (ALooper_pollAll(1, nullptr, &events, (void**)&source) >= 0) {
      if (source != NULL) source->process(m_androidApp, source);
    }

    m_drawFrameFunc(m_framebufferResized);
  } while (m_androidApp->destroyRequested == 0);
#else
  while (!glfwWindowShouldClose(m_window)) {
    glfwPollEvents();
    m_drawFrameFunc(m_framebufferResized);
  }
#endif
}

void Window::GetRequiredExtensions(std::vector<const char*>& out) {
#ifdef __ANDROID__
  out.push_back("VK_KHR_surface");
  out.push_back("VK_KHR_android_surface");
#else
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  out.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);
#endif
}

#ifndef __ANDROID__
void Window::FramebufferResizeCallback(GLFWwindow* window, int width, int height) {
  Window* win               = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
  win->m_framebufferResized = true;
}
#endif
