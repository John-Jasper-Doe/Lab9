/**
 *
 *
 */

#include "checker.hpp"
#include "walker.hpp"

#include <algorithm>
#include <thread>

namespace bayan {
namespace core {

const std::size_t kMaxThread = std::max(std::thread::hardware_concurrency(), 1u);

void checker::prepare() noexcept {
  for (std::size_t i = 0; i < kMaxThread; ++i) {
    pool_.create_thread(boost::bind(&checker::worker, this));
  }
}

void checker::worker() noexcept {}

} /* core:: */
} /* bayan:: */
