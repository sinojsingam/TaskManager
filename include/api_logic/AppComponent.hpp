#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "oatpp/web/server/interceptor/ResponseInterceptor.hpp"
#include "db/MyClient.hpp"
#include "oatpp/orm/DbClient.hpp"
#include "oatpp-1.4.0/oatpp-postgresql/oatpp-postgresql/orm.hpp"
#include "oatpp/web/server/interceptor/AllowCorsGlobal.hpp"
#include "oatpp/json/ObjectMapper.hpp"
#include <memory>
#include <oatpp-postgresql/Connection.hpp>
#include <oatpp-postgresql/ConnectionProvider.hpp>
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "task_logic/task_manager.hpp"

#include "oatpp/macro/component.hpp"
#include "cli_logic/cli_logic.hpp"
#include <iostream>
#include <string>

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */

struct ServerConfig {
  std::string host;
  v_uint16 port;
};

std::string getDBParams() {
    /* Create database-specific ConnectionProvider */
    auto PG_USER = cliMod::getEnvVar("PG_USER");
    auto PG_PASS = cliMod::getEnvVar("PG_PASS");
    auto PG_IP = cliMod::getEnvVar("PG_IP");
    auto PG_PORT = cliMod::getEnvVar("PG_PORT");
    auto PG_DB = cliMod::getEnvVar("PG_DB");
    if (PG_USER != "" &&
        PG_PASS != "" &&
        PG_IP != "" &&
        PG_PORT != "" &&
        PG_DB != ""
    ){
      auto PG_STRING = "postgresql://" + PG_USER + ":" + PG_PASS +"@" + PG_IP + ":" + PG_PORT + "/" + PG_DB;
      std::cout << PG_STRING << std::endl;
      return PG_STRING;
    } else {
      std::cout << "no vars" << std::endl;
      return "";
    };
};

class CustomCorsInterceptor : public oatpp::web::server::interceptor::ResponseInterceptor {
public:
  std::shared_ptr<OutgoingResponse> intercept(
    const std::shared_ptr<IncomingRequest>& request,
    const std::shared_ptr<OutgoingResponse>& response) override {
    
    response->putHeader("Access-Control-Allow-Origin", "*");
    response->putHeader("Access-Control-Allow-Methods", "GET, POST, PUT, PATCH, DELETE, OPTIONS");
    response->putHeader("Access-Control-Allow-Headers", "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range, Authorization");
    response->putHeader("Access-Control-Max-Age", "1728000");
    
    return response;
  }
};

class AppComponent {
public:

  OATPP_CREATE_COMPONENT(std::shared_ptr<ServerConfig>, serverConfig)([] {
    auto cfg = std::make_shared<ServerConfig>();
    cfg->host = "0.0.0.0";
    cfg->port = 8000;
    return cfg;
  }());


  /**
  *  Create ConnectionProvider component which listens on the port
  */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
    OATPP_COMPONENT(std::shared_ptr<ServerConfig>, cfg);

    return oatpp::network::tcp::server::ConnectionProvider::createShared({
        cfg->host, cfg->port, oatpp::network::Address::IP_4});
  }());

  /**
  *  Create Router component
  */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());

  
  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
  connectionHandler->addRequestInterceptor(
  std::make_shared<oatpp::web::server::interceptor::AllowOptionsGlobal>()
  );
  connectionHandler->addResponseInterceptor(
  std::make_shared<CustomCorsInterceptor>()
);

    return connectionHandler;
  }());

  /**
  *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
  */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    return std::make_shared<oatpp::json::ObjectMapper>();
  }());

  /**
   * Create DbClient component.
   * SQLite is used as an example here. For other databases declaration is similar.
   */

  OATPP_CREATE_COMPONENT(std::shared_ptr<db::MyClient>, myDatabaseClient)([] {

    auto connectionProvider = std::make_shared<oatpp::postgresql::ConnectionProvider>(
          getDBParams()
        );

    auto connectionPool = oatpp::postgresql::ConnectionPool::createShared(
      connectionProvider,
      10 /* max connections */,
      std::chrono::seconds(5)
    );

    /* Create database-specific Executor */
    auto executor = std::make_shared<oatpp::postgresql::Executor>(connectionPool);

    /* Create MyClient database client */
    return std::make_shared<db::MyClient>(executor);

  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<taskMod::Todo>, todoList)([] {
    OATPP_COMPONENT(std::shared_ptr<db::MyClient>, client);
    // pass db client to task manager class
    auto todo_list = std::make_shared<taskMod::Todo>("todo list", client);
    return todo_list;
  }());

};

#endif /* AppComponent_hpp */
