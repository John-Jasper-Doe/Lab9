/**
 * @file checker.hpp
 * @brief Definition of the "checker" class.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef CHECKER_HPP_
#define CHECKER_HPP_

#include "common/hashstat.hpp"

#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>

#include <condition_variable>
#include <map>
#include <queue>
#include <set>

/** @brief Namespace of the projet "BAYAN" */
namespace bayan {
/** @brief Namespace of the projet "Core" */
namespace core {

/**
 * @brief The checker class
 */
class checker {
  boost::thread_group pool_;
  std::atomic_bool is_finished{false};

  /* queue path */
  std::mutex mtx_queue_path_;
  std::condition_variable cond_add_path_;
  std::queue<boost::filesystem::path> queue_path_;

  std::map<common::hash_stat, std::set<std::string>> same_files_;
  std::mutex mtx_same_file_;

  std::map<std::string, common::hash_stat> path_to_hash_;
  std::mutex mtx_path_to_hash_;

  std::set<std::string> checked_paths_;
  std::mutex mtx_checked_paths_;

public:
  checker() = default;

  void prepare() noexcept;
  void append(const boost::filesystem::path& path) noexcept;
  void wait() noexcept;
  void print() noexcept;

protected:
  void worker() noexcept;
  void check(const boost::filesystem::path& path);

  bool is_same(const boost::filesystem::path& path1, const boost::filesystem::path& path2);
};

} /* core:: */
} /* bayan:: */

#endif /* CHECKER_HPP_ */
