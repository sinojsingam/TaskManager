#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/Environment.hpp"
#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include <iostream>
#include "api_logic/task_api.hpp"
#include "task_logic/task_manager.hpp"




namespace apiMod {
/** 
 * Custom Request Handler
 */
    std::shared_ptr<Handler::OutgoingResponse> Handler::handle(
        const std::shared_ptr<IncomingRequest>& request
    ) {
        return ResponseFactory::createResponse(Status::CODE_200, "Hello World!");
    }

    int bye(){
        std::cout<< "goodbye world";
        return 1;
    }

    void run() {

      /* Create Router for HTTP requests routing */
      std::cout << "Oat++ Version: " << OATPP_VERSION << std::endl;
      auto router = oatpp::web::server::HttpRouter::createShared();

      router->route("GET", "/hello", std::make_shared<Handler>());

      /* Create HTTP connection handler with router */
      auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);


      /* Create TCP connection provider */
      auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared({"localhost", 8000, oatpp::network::Address::IP_4});

      /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
      oatpp::network::Server server(connectionProvider, connectionHandler);

      /* Print info about server port */
      OATPP_LOGi("MyApp", "Server running on port " + connectionProvider->getProperty("port").toString());

      /* Run server */
      server.run();
    }
}

