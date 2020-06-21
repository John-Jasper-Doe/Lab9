/**
 *
 *
 */

#ifndef CHECKER_HPP_
#define CHECKER_HPP_

#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>

#include <condition_variable>
#include <queue>

namespace bayan {
namespace core {

class checker {
  boost::thread_group pool_;
  std::atomic_bool is_finished{false};

  /* queue path */
  std::mutex mtx_queue_path_;
  std::condition_variable cond_add_path_;
  std::queue<boost::filesystem::path> queue_path_;

public:
  checker() = default;

  void prepare() noexcept;
  void append(const boost::filesystem::path& path) noexcept;
  void wait() noexcept;

protected:
  void worker() noexcept;
};

} /* core:: */
} /* bayan:: */

#endif /* CHECKER_HPP_ */
