#pragma once
#include <iostream>
#include <string>


namespace cliMod {

  void PrintHelp();
  void ProcessArgs(int argc, char** argv);
  std::string getEnvVar( std::string const & key );

}
