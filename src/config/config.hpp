/**
 * @file config.hpp
 * @brief A class declaration describing project parameters.
 *
 * @date 2020
 */

/* See the license in the file "LICENSE.txt" in the root directory. */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <boost/regex.hpp>

#include <set>
#include <stdexcept>
#include <string>
#include <vector>

/** @brief The namespace of the entire project. */
namespace bayan {

/** @brief Namespace for working with project settings. */
namespace config {

/** */
struct config {
  using opt_key_t = std::string;
  using opt_keys_t = std::vector<opt_key_t>;

  using mask_key_t = std::set<boost::regex>;

  enum check_method { crc32, md5 };

  opt_keys_t include;
  opt_keys_t exclude;
  mask_key_t mask;

  std::size_t file_size = 0;
  std::size_t block_size = 0;

  bool recursive;
  check_method method = check_method::crc32;

  bool is_valid() {
    if (include.empty() || block_size == 0)
      return false;

    return true;
  }

  static config* instance();

protected:
  config() = default;
};

} /* config */
} /* bayan */

inline std::istream& operator>>(std::istream& in, bayan::config::config::check_method& method) {
  using namespace std::string_literals;
  using namespace bayan::config;

  std::string token;
  in >> token;

  if (token == "crc32"s)
    method = config::check_method::crc32;
  else if (token == "md5"s)
    method = config::check_method::md5;
  else
    throw std::runtime_error("Invalid check method");
  return in;
}

inline std::ostream& operator<<(std::ostream& os, bayan::config::config::check_method& method) {
  using namespace std::string_literals;
  using namespace bayan::config;

  switch (method) {
  case config::check_method::crc32:
    os << "crc32"s;
    break;

  case config::check_method::md5:
    os << "md5"s;
    break;
  }

  return os;
}

inline std::istream& operator>>(std::istream& in, std::set<boost::regex>& mask) {
  std::string token;
  in >> token;

  mask.emplace(token, boost::regex::icase);

  return in;
}

inline std::ostream& operator<<(std::ostream& os, std::set<boost::regex>& mask) {
  auto cit = mask.cbegin();

  os << (*cit).str();
  mask.erase(cit);
  return os;
}

#endif /* CONFIG_HPP_ */
