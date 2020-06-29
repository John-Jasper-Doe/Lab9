/**
 * @file config.cpp
 * @brief The implementation of the class describing the parameters of the
 * project.
 *
 * @date 2020
 */

/* See the license in the file "LICENSE.txt" in the root directory. */

#include "config.hpp"

/** @brief Namespace of the projet "BAYAN" */
namespace bayan {
namespace config {

config* config::instance() {
  static config main_conf;
  return &main_conf;
}

} /* config:: */
} /* bayan:: */
