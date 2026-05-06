#pragma once


// class mod1c1{
// public:
//    void foo();
// };
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/Environment.hpp"
#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

namespace apiMod {
/** 
 * Custom Request Handler
 */

class Handler : public oatpp::web::server::HttpRequestHandler {

public:
  /**
   * Handle incoming request and return outgoing response.
   */
  std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override;
};

int bye();

void run();
}
