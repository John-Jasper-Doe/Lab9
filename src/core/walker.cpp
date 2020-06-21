/**
 *
 *
 */

#include "walker.hpp"
#include "config/config.hpp"

#include <boost/regex.hpp>

namespace bayan {
namespace core {

namespace bf = boost::filesystem;

namespace {

template <typename T>
void no_push(T&) {}

template <>
void no_push<bf::recursive_directory_iterator>(bf::recursive_directory_iterator& it) {
  it.no_push();
}

/**
 * @brief is_path_exclude
 * @param path
 * @return
 */
bool is_path_exclude(const boost::filesystem::path& path) {
  const auto& cfg = config::config::instance();

  for (const auto& row : {bf::canonical(path).string(), path.relative_path().string()}) {
    for (auto& ex : cfg->exclude) {
      if (row.find(ex) != std::string::npos)
        return true;
    }
  }
  return false;
}

bool is_path_match_size(const boost::filesystem::path& path) noexcept {
  std::size_t f_size = 0;
  try {
    f_size = boost::filesystem::file_size(path);
  }
  catch (boost::filesystem::filesystem_error&) {
    return false;
  }

  const auto& cfg = config::config::instance();
  return f_size >= cfg->file_size;
}

/**
 * @brief is_path_match_mask
 * @param path
 * @return
 */
bool is_path_match_mask(const boost::filesystem::path& path) noexcept {
  const auto& cfg = config::config::instance();

  if (cfg->mask.empty())
    return true;

  static boost::smatch res;
  for (const auto& mask : cfg->mask) {
    if (boost::regex_match(path.filename().string(), res, mask))
      return true;
  }

  return false;
}

/**
 * @brief is_file_satisfy_conditions
 * @param path
 * @return
 */
bool is_file_satisfy_conditions(const boost::filesystem::path& path) {
  return !bf::is_directory(path) && is_path_match_mask(path) && is_path_match_size(path)
      && is_path_exclude(path);
}

} /* :: */

void walker::run() {
  checker_.prepare();

  const auto& cfg = config::config::instance();
  for (auto& dir : cfg->include) {
    if (cfg->recursive)
      find_all<bf::recursive_directory_iterator>(dir);
    else
      find_all<bf::directory_iterator>(dir);
  }

  checker_.wait();
}

template <class DIR_ITER>
void walker::find_all(const boost::filesystem::path& path) noexcept {
  if (is_file_satisfy_conditions(path)) {
    checker_.append(path);
    return;
  }

  DIR_ITER it_dir(path);
  DIR_ITER end;
  while (it_dir != end) {
    auto entry = *it_dir;
    if (is_file_satisfy_conditions(entry)) {
      checker_.append(entry);
    }
    else {
      if (bf::is_directory(entry) && is_path_exclude(entry))
        no_push(it_dir);
    }

    ++it_dir;
  }
}

} /* core:: */
} /* bayan:: */
