#include <doctest/doctest.h>

#include <shadow_mapping_frag.h>
#include <poike/core/struct/Vertex.hpp>

#include <poike/core/VulkanHeader.hpp>

#include <iostream>

// Obvious test, you can remove
TEST_CASE("Load Shader") {
  try {
    auto vertShaderCode = SHADOW_MAPPING_FRAG;
    CHECK(vertShaderCode.size() > 0);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    CHECK(false);
  }
}

TEST_CASE("Vertex") {
  VkVertexInputBindingDescription bindingDescription = vkl::Vertex::getBindingDescription();
  CHECK(bindingDescription.binding == 0);
  CHECK(bindingDescription.stride == sizeof(vkl::Vertex));
  CHECK(bindingDescription.inputRate == VK_VERTEX_INPUT_RATE_VERTEX);
}
