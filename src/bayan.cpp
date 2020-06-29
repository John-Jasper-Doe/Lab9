/**
 * @file bayan.cpp
 * @brief Main file in this project.
 *
 * @author Maxim <john.jasper.doe@gmail.com>
 * @date 2020
 */

/* See the license in the file "LICENSE.txt" in the root directory. */

#include "config/config.hpp"
#include "core/walker.hpp"
#include "version.hpp"

#include <boost/program_options.hpp>

#include <iostream>

/** @brief Getting and setting program parameters. */
void get_param(int argc, char* argv[]) {
  namespace po = boost::program_options;
  namespace bc = bayan::config;

  using namespace std::string_literals;

  // clang-format off
  bayan::config::config* cfg = bayan::config::config::instance();
  po::options_description desc("Options");
  desc.add_options()
      ("help,h", "this help.")
      ("include,i",   po::value(&cfg->include), "list directories to search ('.' by default)")
      ("exclude,x",   po::value(&cfg->exclude), "list directories to exclude")
      ("mask,s",      po::value(&cfg->mask), "list file mask to check")
      ("min_fsize,f", po::value(&cfg->file_size)->default_value(1),
                      "min file size ('1 Byte' by default)")
      (",r",          po::bool_switch(&cfg->recursive)->default_value(false),
                      "recursive ('false' by default)")
      ("bsize,b",     po::value(&cfg->block_size)->default_value(100),
                      "block size ('100 Byte' by default)")
      ("method,m",    po::value(&cfg->method)->default_value(bc::config::check_method::crc32),
                      "check algorithm (crc32/md5) ('crc32' by default)");
  // clang-format on

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help") || vm.count("h")) {
    std::cout << desc << "\n";
    exit(0);
  }

  if (cfg->include.empty()) {
    cfg->include.emplace_back(".");
  }

  if (cfg->mask.empty()) {
    cfg->mask.emplace(".*");
  }

  if (!cfg->is_valid()) {
    std::cout << desc << std::endl;
    throw std::runtime_error("Invalid arguments");
  }
}

/** @brief Main entry point */
int main(int argc, char* argv[]) {
  std::cout << "BAYAN v." << BAYAN_MAJOR << "." << BAYAN_MINOR << "." << BAYAN_PATCH << std::endl;
  std::cout << "  Make by Maxim <john.jasper.doe@gmail.com>" << std::endl;

  try {
    get_param(argc, argv);
  }
  catch (std::runtime_error& e) {
    std::cout << e.what() << std::endl;
  }


  bayan::core::walker w;
  w.run();
  w.print();
}
