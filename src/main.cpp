#include "task_logic/task_manager.hpp"
#include "api_logic/task_api.hpp"
#include "oatpp/Environment.hpp"

int main() {

  /* Init oatpp Environment */
  oatpp::Environment::init();

  /* Run App */
  apiMod::run();

  /* Destroy oatpp Environment */
  oatpp::Environment::destroy();

  return 0;

}

