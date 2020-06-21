/**
 * @file walker.hpp
 * @brief Definition of the "walker" class.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef WALKER_HPP_
#define WALKER_HPP_

#include "checker.hpp"

#include <boost/filesystem.hpp>

/** @brief Namespace of the projet "BAYAN" */
namespace bayan {
/** @brief Namespace of the projet "Core" */
namespace core {

/**
 * @brief The walker class
 */
class walker {
  checker checker_;

public:
  walker() = default;

  void run();
  void print() noexcept;

protected:
  template <class DIR_ITER>
  void find_all(const boost::filesystem::path& path) noexcept;
};

} /* core:: */
} /* bayan:: */

#endif /* WALKER_HPP_ */
