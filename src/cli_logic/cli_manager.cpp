#include <getopt.h>
#include <memory>
#include <iostream>
#include "api_logic/task_api.hpp"
#include "task_logic/task_manager.hpp"
#include "cli_logic/cli_logic.hpp"
#include "oatpp/Environment.hpp"

namespace cliMod {

  void PrintHelp(){
    std::cout << "-a, --api :: \tStart Task Manager as a REST API service" << std::endl;
    std::cout << "-c, --cli :: \tStart Task Manager as a CLI service" << std::endl;
    std::cout << "-h, --help :: \tShow help" << std::endl;
    return;
  }

  void ProcessArgs(int argc, char** argv) {
    const char* const short_opts = "hac";
    const option long_opts[] = {
        {"api", no_argument, nullptr, 'a'},
        {"cli", no_argument, nullptr, 'c'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, no_argument, nullptr, 0}
      };

    while (true) {
      const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

      if (opt == -1) break;

      switch (opt) {
        case 'a':
          std::cout << "starting api" << std::endl;
          /* Init oatpp Environment */
          oatpp::Environment::init();
          /* Run App */
          apiMod::run();
          /* Destroy oatpp Environment */
          oatpp::Environment::destroy();
          break;
        case 'c':
          // creating scope for taskApp
          // so it is not available for other case jumps
          {
            std::cout << "starting cli" << std::endl;
            std::unique_ptr<taskMod::Todo> taskApp = std::make_unique<taskMod::Todo>("test");
            taskApp->runTaskApp();
          }
          break;
        case 'h':
        case '?':
        default:
          PrintHelp();
          break;
        }
      }
  }

}

