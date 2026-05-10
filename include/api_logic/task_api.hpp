#pragma once

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/json/ObjectMapper.hpp"
#include "task_logic/task_manager.hpp"
#include <set>

namespace apiMod {
    /** 
     * Custom Request Handler
     */
    std::string sanitizeText(
            std::string &original_text,
            const char replacement_char='_',
            std::set<char> extra_checks={});

    class Handler : public oatpp::web::server::HttpRequestHandler {

        private:
            std::shared_ptr<taskMod::Todo> m_todoList;
            std::shared_ptr<oatpp::data::mapping::ObjectMapper> m_objectMapper;

        public:
            /**
            * Handle incoming request and return outgoing response.
            */
            std::shared_ptr<OutgoingResponse> handle(
                    const std::shared_ptr<IncomingRequest>& request
                    ) override;

            Handler(
                    const std::shared_ptr<taskMod::Todo>& todoList,
                    const std::shared_ptr<oatpp::json::ObjectMapper>& objectMapper);

    };


    void run();
}
