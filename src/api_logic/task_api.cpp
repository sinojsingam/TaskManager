#include "api_logic/AppComponent.hpp"
#include "controller/MyController.hpp"
#include "oatpp/network/Server.hpp"

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include <csignal>
#include <oatpp/macro/component.hpp>

#include "oatpp/Environment.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include <iostream>
#include <memory>
#include <oatpp/Types.hpp>
#include <oatpp/base/Log.hpp>
#include <oatpp/json/Deserializer.hpp>
#include <oatpp/json/ObjectMapper.hpp>
#include <oatpp/json/Serializer.hpp>
#include <oatpp/json/Utils.hpp>
#include <oatpp/utils/parser/ParsingError.hpp>
#include <oatpp/web/server/HttpProcessor.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include "api_logic/task_api.hpp"
#include "task_logic/task_manager.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/json/ObjectMapper.hpp"
#include "oatpp/Types.hpp"



std::shared_ptr<oatpp::network::Server> g_server;

void shutdownHandler(int signal) {
    OATPP_LOGi("Server", "Shutting down...");
    if (g_server) {
        g_server->stop();
    }
}


namespace apiMod {


    void run() {
        {
            // scope
            /* Register Components in scope of run() method */
            AppComponent components;


            auto objectMapper = std::make_shared<oatpp::json::ObjectMapper>();


            /* Create Router for HTTP requests routing */
            std::cout << "Oat++ Version: " << OATPP_VERSION << std::endl;

            // Get router component
            OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

            // Create MyController and add all of its endpoints to router
            auto myController = std::make_shared<MyController>();
            router->addController(myController);

            /* Get HTTP connection handler with router */
            OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

            /* Get TCP connection provider */
            OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

            /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
            g_server = std::make_shared<oatpp::network::Server>(connectionProvider, connectionHandler);

            /* Print info about server port */
            OATPP_LOGi("MyApp", "Server running on port " + connectionProvider->getProperty("port").toString());

            std::signal(SIGINT, shutdownHandler);

            /* Run server */
            g_server->run(); // blocks until Ctrl+C

        }

    g_server = nullptr; // clear the global var

    }
}

