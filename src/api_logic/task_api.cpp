#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include <csignal>

#include "oatpp/Environment.hpp"
#include "oatpp/network/Server.hpp"
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
#include <vector>
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
    /* Begin DTO code-generation */
    #include OATPP_CODEGEN_BEGIN(DTO)

    /**
     * Message Data-Transfer-Object
     */
    class MessageDto : public oatpp::DTO {

        DTO_INIT(MessageDto, DTO /* Extends */)

        DTO_FIELD(Int32, statusCode);   // Status code field
        DTO_FIELD(String, message);     // Message field
        DTO_FIELD(Vector<String>, tasks);


    };

    /* End DTO code-generation */
    #include OATPP_CODEGEN_END(DTO)


    /** 
     * Custom Request Handler
     */
    std::shared_ptr<Handler::OutgoingResponse> Handler::handle(
        const std::shared_ptr<IncomingRequest>& request
    ) {
        std::vector<taskMod::Task> t = m_todoList->viewTasks(-1);
        std::vector<std::string> h;
        auto message = MessageDto::createShared();
        message->statusCode = 1024;
        message->message = "tasks";
        message->tasks = oatpp::Vector<oatpp::String>::createShared();

        for (const auto& task : t) {
            message->tasks->push_back(oatpp::String(task.m_task));
        }

        return ResponseFactory::createResponse(Status::CODE_200, message, m_objectMapper);

    }

    Handler::Handler(
                    const std::shared_ptr<taskMod::Todo>& todoList,
                    const std::shared_ptr<oatpp::json::ObjectMapper>& objectMapper)
                   : m_todoList(todoList), m_objectMapper(objectMapper) {};



    void run() {
        {
            // scope
            auto todoList = std::make_shared<taskMod::Todo>("newlist");

            todoList->getItems();

            std::string API_HOST = "0.0.0.0";
            v_uint16 API_PORT = 8000;
            auto objectMapper = std::make_shared<oatpp::json::ObjectMapper>();


            /* Create Router for HTTP requests routing */
            std::cout << "Oat++ Version: " << OATPP_VERSION << std::endl;
            auto router = oatpp::web::server::HttpRouter::createShared();

            router->route("GET", "/tasks", std::make_shared<Handler>(todoList, objectMapper));

            /* Create HTTP connection handler with router */
            auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

            /* Create TCP connection provider */
            auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared({API_HOST, API_PORT, oatpp::network::Address::IP_4});

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

