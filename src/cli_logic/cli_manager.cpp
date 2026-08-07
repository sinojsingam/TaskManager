#include <getopt.h>
#include <memory>
#include <iostream>
#include "api_logic/task_api.hpp"
#include "task_logic/task_manager.hpp"
#include "cli_logic/cli_logic.hpp"
#include "oatpp/Environment.hpp"
#include <iostream>
#include <string>


namespace cliMod {

  void PrintHelp(){
    std::cout << "ENV variables to declare \n\tPG_USER: The database username\n\tPG_PASS: Database password\n\tPG_IP: IP of the DB server\n\tPG_PORT: port of the DB\n\tPG_DB name of the database to connect to\n\n" << std::endl;
    std::cout << "-a, --api :: \tStart Task Manager as a REST API service" << std::endl;
    std::cout << "-c, --cli :: \tStart Task Manager as a CLI service" << std::endl;
    std::cout << "-h, --help :: \tShow help" << std::endl;
    return;
  }

  std::string getEnvVar( std::string const & key )
    {
        std::cout << "trying to get value for " << key << std::endl;
        char * val = getenv( key.c_str() );
        return val == NULL ? std::string("") : std::string(val);
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

      if (opt == -1){
        // if none was passed show help and close
        PrintHelp();
        break;
      }

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
            std::unique_ptr<taskMod::Todo> taskApp = std::make_unique<taskMod::Todo>("todo list");
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

