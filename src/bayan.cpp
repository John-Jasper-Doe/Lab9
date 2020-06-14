#include <iostream>
#include <boost/program_options.hpp>
#include "version.hpp"

void get_param(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
}

int main(int argc, char* argv[]) {
  std::cout << "BAYAN v." << BAYAN_MAJOR << "." << BAYAN_MINOR << "." << BAYAN_PATCH << std::endl;
  std::cout << "  Make by Maxim <john.jasper.doe@gmail.com>" << std::endl;

  get_param(argc, argv);

}
