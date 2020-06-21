/**
 *
 *
 */

#include "checker.hpp"
#include "config/config.hpp"

#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>

#include <algorithm>
#include <mutex>
#include <thread>

namespace bayan {
namespace core {

namespace bf = boost::filesystem;

const std::size_t kMaxThread = std::max(std::thread::hardware_concurrency(), 1u);

namespace {

std::string calc_hash(const char* buffer, std::size_t size) {
  std::string res;

  const auto& cfg = config::config::instance();
  switch (cfg->method) {
  case config::config::crc32: {
    boost::crc_32_type result;

    result.process_bytes(buffer, size);
    res = std::to_string(result.checksum()) + "__";
    break;
  }

  case config::config::md5: {
    boost::uuids::detail::md5 hash;
    boost::uuids::detail::md5::digest_type digest;

    hash.process_bytes(buffer, size);
    hash.get_digest(digest);
    for (int i = 0; i < 4; i++) {
      res += std::to_string(digest[i]) + "_";
    }

    res += "__";
    break;
  }
  }

  return res;
}

} /* :: */

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

void checker::wait() noexcept {
  is_finished = true;
  cond_add_path_.notify_all();
  pool_.join_all();
}

void checker::worker() noexcept {
  while (true) {
    std::unique_lock<std::mutex> lock(mtx_queue_path_);
    cond_add_path_.wait(lock,
                        [this]() -> bool { return !this->queue_path_.empty() || is_finished; });
    if (queue_path_.empty())
      break;

    bf::path p = std::move(queue_path_.front());
    queue_path_.pop();
    lock.unlock();
  }
}

} /* core:: */
} /* bayan:: */
