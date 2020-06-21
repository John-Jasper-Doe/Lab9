/**
 *
 *
 */

#include "checker.hpp"
#include "config/config.hpp"

#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>

#include <algorithm>
#include <iostream>
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
    check(p);
  }
}

void checker::check(const boost::filesystem::path& path) {
  std::vector<std::string> comparable_files;
  bf::path new_file = bf::canonical(path).string();

  {
    std::lock_guard<std::mutex> lock(mtx_checked_paths_);
    comparable_files.reserve(checked_paths_.size());
    comparable_files.insert(comparable_files.begin(), checked_paths_.begin(), checked_paths_.end());
    checked_paths_.insert(new_file.string());
  }

  for (const auto& checked_file : comparable_files) {
    if (bf::file_size(new_file) != bf::file_size(checked_file))
      continue;

    /* size is same */
    if (is_same(new_file, checked_file)) {
      std::unique_lock<std::mutex> lock1(mtx_same_file_, std::defer_lock);
      std::unique_lock<std::mutex> lock2(mtx_path_to_hash_, std::defer_lock);
      std::lock(lock1, lock2);

      same_files_[path_to_hash_[new_file.string()]].insert(new_file.string());
      same_files_[path_to_hash_[new_file.string()]].insert(checked_file);
      break;
    }
  }
}

bool checker::is_same(const boost::filesystem::path& path1, const boost::filesystem::path& path2) {
  common::hash_stat hash1;
  common::hash_stat hash2;

  {
    std::lock_guard<std::mutex> lock(mtx_path_to_hash_);
    hash1 = path_to_hash_[path1.string()];
    hash2 = path_to_hash_[path2.string()];
  }

  /* check what has already been counted */
  std::size_t i = 0;
  for (; i < std::min(hash1.value.size(), hash2.value.size()); ++i) {
    if (hash1.value[i] != hash2.value[i]) {
      return false;
    }
  }

  const auto& cfg = config::config::instance();
  std::ifstream fstrm1(path1.string(), std::ios::in | std::ios::binary | std::ios::ate);
  std::ifstream fstrm2(path2.string(), std::ios::in | std::ios::binary | std::ios::ate);

  fstrm1.seekg(static_cast<std::ios::off_type>(i * cfg->block_size), std::ios_base::beg);
  fstrm2.seekg(static_cast<std::ios::off_type>(i * cfg->block_size), std::ios_base::beg);

  bool stored1 = false;
  bool stored2 = false;
  bool same = true;

  auto buffer = std::make_unique<char[]>(cfg->block_size);
  for (; i < std::ceil(static_cast<double>(bf::file_size(path1)) / cfg->block_size); ++i) {
    if (hash1.value.size() <= i) {
      std::memset(buffer.get(), 0, cfg->block_size);
      fstrm1.read(buffer.get(), static_cast<std::streamsize>(cfg->block_size));
      hash1.value.emplace_back(calc_hash(buffer.get(), cfg->block_size));
      stored1 = true;
    }

    if (hash2.value.size() <= i) {
      std::memset(buffer.get(), 0, cfg->block_size);
      fstrm2.read(buffer.get(), static_cast<std::streamsize>(cfg->block_size));
      hash2.value.emplace_back(calc_hash(buffer.get(), cfg->block_size));
      stored2 = true;
    }

    if (hash1.value[i] != hash2.value[i]) {
      same = false;
      break;
    }
  }

  fstrm1.close();
  fstrm2.close();

  {
    std::lock_guard<std::mutex> lock(mtx_path_to_hash_);
    if (stored1) {
      path_to_hash_[path1.string()] = hash1;
    }
    if (stored2) {
      path_to_hash_[path2.string()] = hash2;
    }
  }

  return same;
}

void checker::print() noexcept {
  auto it = same_files_.cbegin();
  for (; it != same_files_.cend(); ++it) {
    for (auto& file : it->second) {
      std::cout << file << std::endl;
    }
    std::cout << std::endl;
  }
}

} /* core:: */
} /* bayan:: */
