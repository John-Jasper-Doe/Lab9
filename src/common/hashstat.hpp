/**
 * @file hashstat.hpp
 * @brief Description of the structure with hash information.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

#ifndef HASHSTAT_HPP_
#define HASHSTAT_HPP_

#include <string>
#include <vector>

/** @brief Namespace of the projet "BAYAN" */
namespace bayan {
/** @brief Namespace of the projet "Common" */
namespace common {

/**
 * @brief Structures with hash information.
 */
struct hash_stat {
  std::vector<std::string> value;

  bool operator<(const hash_stat& rhs) const {
    return value < rhs.value;
  }
};

} /* common:: */
} /* bayan:: */

#endif /* HASHSTAT_HPP_ */
