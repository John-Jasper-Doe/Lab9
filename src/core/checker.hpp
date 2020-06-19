/**
 *
 *
 */

#ifndef CHECKER_HPP_
#define CHECKER_HPP_

#include <boost/thread/thread.hpp>

namespace bayan {
namespace core {

class checker {
  boost::thread_group pool_;

public:
  checker() = default;

  void prepare() noexcept;

protected:
  void worker() noexcept;
};

} /* core:: */
} /* bayan:: */

#endif /* CHECKER_HPP_ */
