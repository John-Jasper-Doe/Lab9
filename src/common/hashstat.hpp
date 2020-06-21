/**
 *
 *
 */

#ifndef HASHSTAT_HPP_
#define HASHSTAT_HPP_

#include <vector>
#include <string>

namespace bayan {
namespace common {

struct hash_stat {
  std::vector<std::string> value;

  bool operator<(const hash_stat& rhs) const {
    return value < rhs.value;
  }
};

} /* common:: */
} /* bayan:: */

#endif /* HASHSTAT_HPP_ */
