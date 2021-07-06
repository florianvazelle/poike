[![Actions Status](https://github.com/florianvazelle/poike/workflows/build/badge.svg)](https://github.com/florianvazelle/poike/actions)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/fdf05c587b994f008692d4e6fd1f0acd)](https://www.codacy.com/gh/florianvazelle/poike/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=florianvazelle/poike&amp;utm_campaign=Badge_Grade)
![Platform](https://img.shields.io/badge/platform-windows%20%7C%20linux-blue) 
[![License](https://img.shields.io/github/license/florianvazelle/poike)](https://github.com/florianvazelle/poike/blob/main/LICENSE)
[![Lines of Code](https://tokei.rs/b1/github/florianvazelle/poike?category=code)](https://tokei.rs)

# Poike

This is a work in progress comprehensible Vulkan framework.

## Building

### Build and run the standalone target

Use the following command to build and run the executable target.

```bash
cmake -Bbuild
cmake --build build
```

### Build and run test suite

Use the following commands from the project's root directory to run the test suite.

```bash
cmake -Bbuild
cmake --build build
./build/bin/poikeTests
```

### Developement

To run it with [include-what-you-use](https://github.com/include-what-you-use/include-what-you-use) :
```bash
sudo apt install llvm-10-dev libclang-10-dev clang-10
git clone https://github.com/include-what-you-use/include-what-you-use.git iwyu

cd iwyu
git checkout clang_10

mkdir build
cd build

cmake -G "Unix Makefiles" -DCMAKE_PREFIX_PATH=/usr/lib/llvm-10 ..

cd ..
CC="clang" CXX="clang++" cmake -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE="iwyu/build/bin/include-what-you-use;-Xiwyu;any;-Xiwyu;iwyu;-Xiwyu;args" -Bbuild
cmake --build build 2> iwyu.log
```

## Dependencies

- C++20 compiler :
  - Visual Studio 2019
  - GCC 9+ or Clang 10+
- [CMake](https://cmake.org/) for build system creation (>= 3.16.3)
- [Conan](https://conan.io/) for install packages (>= 1.0)
- [Doxygen](https://doxygen.org/) for generate documentation (>= 1.8, optional)

## References

- [Writing an Efficient Vulkan Renderer](https://zeux.io/2020/02/27/writing-an-efficient-vulkan-renderer/)
- [Vulkan Shader Resource-Binding](https://developer.nvidia.com/vulkan-shader-resource-binding)
- [Vulkan Input Attachments and Sub Passes](https://www.saschawillems.de/blog/2018/07/19/vulkan-input-attachments-and-sub-passes/)
- [Yet Another Blog Explaining Vulkan Synchronization](https://themaister.net/blog/2019/08/14/yet-another-blog-explaining-vulkan-synchronization/)
