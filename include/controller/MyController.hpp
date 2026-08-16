#ifndef MyController_hpp
#define MyController_hpp

#include "api_logic/task_api.hpp"
#include "dto/DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"
#include "task_logic/task_manager.hpp"
#include "oatpp/json/ObjectMapper.hpp"
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include "db/MyClient.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin Codegen

/**
 * Sample Api Controller.
 */
class MyController : public oatpp::web::server::api::ApiController {
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}

  OATPP_COMPONENT(std::shared_ptr<taskMod::Todo>, todo);

public:

  OATPP_COMPONENT(std::shared_ptr<db::MyClient>, client);


  auto errorDto (std::string message="") {
    auto dto = MessageDto::createShared();
    dto->statusCode = 400;
    dto->message = "Error: " + message;
    return createDtoResponse(Status::CODE_400, dto);
  }

  ENDPOINT("GET", "/debug", root) {
    auto dto = MessageDto::createShared();
    dto->statusCode = 200;
    dto->message = "Hello World!";
    // auto result = client->getTasks();
    // auto dataset = result->fetch<oatpp::Vector<oatpp::Fields<oatpp::Any>>>();

    /* And we can easily serialize result as a json string using json object mapper */
    // auto jsonObjectMapper = oatpp::json::ObjectMapper();
    // auto json = jsonObjectMapper.writeToString(dataset);
    //
    // /* Print the resultant json */
    // std::cout << json->c_str() << std::endl;
    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("GET", "/tasks", tasks) {
    auto dto = TasksDTO::createShared();
    dto->statusCode = 200;
    dto->message = "Your tasks lists";
    dto->tasks = oatpp::Vector<oatpp::Object<TaskDTO>>::createShared();

    for (const auto& task : todo->viewTasks(-1)) {
      auto single = TaskDTO::createShared();
      single->statusCode = 200;
      single->taskId = task.getId();
      single->taskString = task.getTask();
      single->taskStatus = task.getTaskStatus();

      dto->tasks->push_back(single);
    }

    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("DELETE", "/tasks/{id}", deleteTaskById, PATH(Int32, id)) {
    auto dto = MessageDto::createShared();
    dto->statusCode = 200;
    dto->message = "Deleted task successfully";
    if (todo->hasId(id)) {
        todo->removeTaskApi(id);
      return createDtoResponse(Status::CODE_200, dto);
    }
    return errorDto("Task was not deleted!");
  }

  ENDPOINT("PATCH", "/tasks/{id}", toggleTaskById,
      PATH(Int32, id)
      ) {

    auto dto = MessageDto::createShared();
    dto->statusCode = 200;
    dto->message = "Toggled task successfully";

    if (todo->hasId(id)) {
        todo->toggleTaskApi(id);
        return createDtoResponse(Status::CODE_200, dto);
    }

    return errorDto("Out of bounds");
  }

  ENDPOINT("PUT", "/tasks/{id}", editTaskById,
      PATH(Int32, id),
      BODY_DTO(Object<IncomingTaskDto>, taskDto)) {

    if (!taskDto->taskString && taskDto->taskStatus == nullptr){
      return errorDto("taskString or taskStatus must be provided");
    }

    if (!todo->hasId(id)) {
        return errorDto("Id Out of bounds");
    }

    auto dto = MessageDto::createShared();
    dto->statusCode = 200;
    dto->message = "Edited task successfully";


    // get existing task
    std::vector<taskMod::Task> taskVector = todo->viewTasks(id);
    taskMod::Task task = taskVector[0];
    // save the status and text from before
    bool incomingStatus = task.getTaskStatus();
    std::string incomingString = task.getTask();

    // overwrite from incoming if any
    if (taskDto->taskStatus != nullptr){
        incomingStatus = *taskDto->taskStatus;
    }

    if (taskDto->taskString){
        incomingString = *taskDto->taskString;
    }

    // update instance
    todo->editTaskApi(id, incomingString, incomingStatus);
    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("GET", "/tasks/{id}", getTaskById, PATH(Int32, id)) {
    auto dto = TaskDTO::createShared();

    if (todo->hasId(id)) {
        std::vector<taskMod::Task> taskVector = todo->viewTasks(id);
        taskMod::Task task = taskVector[0];
        dto->statusCode = 200;
        dto->taskId = id;
        dto->taskString = task.getTask();
        dto->taskStatus = task.getTaskStatus();

      return createDtoResponse(Status::CODE_200, dto);
    }

    return errorDto("Out of bounds");
  }

 // BODY_DTO deserializes incoming JSON
  ENDPOINT("POST", "/tasks",
      createTask,
      BODY_DTO(Object<IncomingTaskDto>,taskDto)
      ){

    if (!taskDto->taskString){
      return errorDto("taskString must be provided");
    }

    bool incomingStatus = false;

    if (taskDto->taskStatus != nullptr){
        incomingStatus = *taskDto->taskStatus;
    }

    std::string new_task = *taskDto->taskString;

    todo->addTaskApi(new_task, incomingStatus);

    auto dto = MessageDto::createShared();
    dto->statusCode = 200;
    dto->message = "POST request received!";

    return createDtoResponse(Status::CODE_201, dto);
  }

};

#include OATPP_CODEGEN_END(ApiController) ///< End Codegen

#endif /* MyController_hpp */
