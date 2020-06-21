/**
 *
 *
 */

#ifndef WALKER_HPP_
#define WALKER_HPP_

#include "checker.hpp"

#include <boost/filesystem.hpp>

namespace bayan {
namespace core {

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
