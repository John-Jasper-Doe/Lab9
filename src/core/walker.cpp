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

bool is_file_satisfy_conditions(const boost::filesystem::path& path) {
  (void)path;
  return true;
}

bool is_path_exclude(const boost::filesystem::path& path) {
  (void)path;
  return true;
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
