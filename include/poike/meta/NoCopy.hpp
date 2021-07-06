/**
 * @file common/NoCopy.hpp
 * @brief Define NoCopy class
 */

#ifndef NOCOPY_HPP
#define NOCOPY_HPP

/**
 * @brief A class to prohibit copying.
 *
 * The class which inherits from it will have no constructor per copy and no equal operator.
 * We prefer to pass an object by reference, which avoids the duplication of data members of the class.
 */
struct NoCopy {
  NoCopy() = default;

  // copy assignment operator
  NoCopy &operator=(const NoCopy &rhs) = delete;

  // copy constructor
  NoCopy(const NoCopy &rhs) = delete;

protected:
  ~NoCopy() = default;
};

#endif  // NOCOPY_HPP