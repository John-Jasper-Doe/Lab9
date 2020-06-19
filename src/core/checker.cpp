/**
 *
 *
 */

#include "checker.hpp"
#include "walker.hpp"

#include <algorithm>
#include <mutex>
#include <thread>

namespace bayan {
namespace core {

const std::size_t kMaxThread = std::max(std::thread::hardware_concurrency(), 1u);

void checker::prepare() noexcept {
  for (std::size_t i = 0; i < kMaxThread; ++i) {
    pool_.create_thread(boost::bind(&checker::worker, this));
  }
}

void checker::append(const boost::filesystem::path& path) noexcept {
  std::lock_guard<std::mutex> lock(mtx_queue_path_);
  queue_path_.push(path);
  cond_add_path_.notify_one();
}

void checker::worker() noexcept {}

} /* core:: */
} /* bayan:: */
