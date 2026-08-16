#pragma once

#include "oatpp/orm/DbClient.hpp"
#include "oatpp/macro/codegen.hpp"


namespace db {

#include OATPP_CODEGEN_BEGIN(DbClient) ///< Begin code-gen section

class MyClient : public oatpp::orm::DbClient {
public:

  MyClient(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor)
  {}
    const char *todoTableInit = R"(
    CREATE TABLE IF NOT EXISTS tasks (
        id INT PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
        taskString TEXT,
        taskStatus BOOLEAN DEFAULT FALSE
    );
    )";

    QUERY(init, todoTableInit);

    QUERY(createTask,
          "INSERT INTO tasks (taskstring, taskstatus) "
          "VALUES (:task, :status) "
          "RETURNING id, taskstring, taskstatus;",
          PREPARE(false),
          PARAM(oatpp::String, task),
          PARAM(oatpp::Boolean, status))

    QUERY(deleteTask,
          "DELETE FROM tasks "
          "WHERE id=:id;",
          PREPARE(false),
          PARAM(oatpp::Int16, id))

    QUERY(editTaskStringById, 
          "UPDATE tasks "
          "SET taskstring=:task "
          "WHERE id=:id;",
          PARAM(oatpp::Int64, id),
          PARAM(oatpp::String, task))

    QUERY(editTaskStatusById, 
          "UPDATE tasks "
          "SET taskstatus=:status "
          "WHERE id=:id;",
          PARAM(oatpp::Int64, id),
          PARAM(oatpp::Boolean, status))

    QUERY(getTaskById, 
          "SELECT * FROM tasks WHERE id=:id;",
          PARAM(oatpp::Int64, id)) 

    QUERY(getTasks,
          "SELECT * FROM tasks;")

};

#include OATPP_CODEGEN_END(DbClient) ///< End code-gen section
};
