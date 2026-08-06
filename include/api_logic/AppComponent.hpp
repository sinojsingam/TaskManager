#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "oatpp/json/ObjectMapper.hpp"
#include <memory>
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "task_logic/task_manager.hpp"

#include "oatpp/macro/component.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */

struct ServerConfig {
  std::string host;
  v_uint16 port;
};

class AppComponent {
public:

  OATPP_CREATE_COMPONENT(std::shared_ptr<taskMod::Todo>, todoList)([] {
    auto todo_list = std::make_shared<taskMod::Todo>("todo list");
    return todo_list;
  }());

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
    return oatpp::web::server::HttpConnectionHandler::createShared(router);
  }());

  /**
  *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
  */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    return std::make_shared<oatpp::json::ObjectMapper>();
  }());

};

#endif /* AppComponent_hpp */
