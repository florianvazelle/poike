/**
 * @file NoMove.hpp
 * @brief Define NoMove class
 */

#ifndef NOMOVE_HPP
#define NOMOVE_HPP

/**
 * @brief A class to prohibit move.
 */
struct NoMove {
  NoMove() = default;

  // move assignment operator
  NoMove &operator=(NoMove &&rhs) = delete;

  // move constructor
  NoMove(const NoMove &&rhs) = delete;

protected:
  ~NoMove() = default;
};

#endif  // NOMOVE_HPP
