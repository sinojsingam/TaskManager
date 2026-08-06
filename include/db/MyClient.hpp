#pragma once

#include "oatpp/orm/SchemaMigration.hpp"
#include "oatpp/orm/DbClient.hpp"
#include "oatpp/macro/codegen.hpp"


namespace db {

#include OATPP_CODEGEN_BEGIN(DbClient) ///< Begin code-gen section

class MyClient : public oatpp::orm::DbClient {
public:

  MyClient(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor)
  {}

    QUERY(createUser,
        "INSERT INTO users (username, email, role) VALUES (:username, :email, :role);",
        PARAM(oatpp::String, username), 
        PARAM(oatpp::String, email), 
        PARAM(oatpp::String, role))

  QUERY(getUserByName, 
        "SELECT * FROM users WHERE username=:username;", 
        PARAM(oatpp::String, username)) 
};

#include OATPP_CODEGEN_END(DbClient) ///< End code-gen section
};
