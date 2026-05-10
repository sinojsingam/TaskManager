#ifndef MyController_hpp
#define MyController_hpp

#include "dto/DTOs.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"
#include "task_logic/task_manager.hpp"
#include <iostream>
#include <string>

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

  ENDPOINT("GET", "/debug", root) {
    auto dto = MessageDto::createShared();
    dto->statusCode = 200;
    dto->message = "Hello World!";
    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("GET", "/tasks", tasks) {
    auto dto = TasksDTO::createShared();
    dto->statusCode = 200;
    dto->message = "Your tasks!";
    dto->tasks = oatpp::Vector<oatpp::Object<TaskDTO>>::createShared();

    for (const auto& task : todo->viewTasks(-1)) {
      auto single = TaskDTO::createShared();
      single->statusCode = 200;
      single->taskId = task.m_task_id;
      single->taskString = task.m_task;
      single->taskStatus = task.m_status;

      dto->tasks->push_back(single);
    }

    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("DELETE", "/tasks/{id}", deleteTaskById, PATH(Int32, id)) {
    auto dto = MessageDto::createShared();
    dto->statusCode = 200;
    dto->message = "Deleted task successfully";
    if (id < todo->sizeOfList()) {
        todo->removeTaskApi(id);
    }
    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("PATCH", "/tasks/{id}", toggleTaskById,
      PATH(Int32, id),
      BODY_DTO(Object<TaskDTO>, taskDto)) {

    auto dto = MessageDto::createShared();

    bool toggle = *taskDto->taskStatus;

    if (toggle){
      std::cout << "toggle task to true" << std::endl;
    } else{
      std::cout << "toggle task to false" << std::endl;
    }

    dto->statusCode = 200;
    dto->message = "Toggled task successfully";

    if (id < todo->sizeOfList()) {
        todo->toggleTaskApi(id);
    }

    return createDtoResponse(Status::CODE_200, dto);
  }

  ENDPOINT("PUT", "/tasks/{id}", editTaskById,
      PATH(Int32, id),
      BODY_DTO(Object<TaskDTO>, taskDto)) {

    auto dto = MessageDto::createShared();
    std::cout << *taskDto->taskString << std::endl;
    std::string edit_task = *taskDto->taskString;
    dto->statusCode = 200;
    dto->message = "Edited task successfully";

    if (id < todo->sizeOfList()) {
        todo->editTaskApi(id, edit_task);
    }

    return createDtoResponse(Status::CODE_200, dto);

  }

  ENDPOINT("GET", "/tasks/{id}", getTaskById, PATH(Int32, id)) {
    auto dto = TaskDTO::createShared();

    std::string outOfBoundsString = "Out of bounds";
    bool outOfBoundsStatus = false;
    int outOfBoundsCode = 400;
    Status outOfBoundsReturnCode = Status::CODE_400;

    std::string &taskString = outOfBoundsString;
    bool &status = outOfBoundsStatus;
    int &statusCode = outOfBoundsCode;
    Status &returnCode = outOfBoundsReturnCode;

    if (id < todo->sizeOfList()) {
        std::vector<taskMod::Task> taskVector = todo->viewTasks(id);
        taskMod::Task task = taskVector[0];
        taskString = task.m_task;
        status = task.m_status;
        statusCode = 200;
        returnCode = Status::CODE_200;
    }

    dto->statusCode = statusCode;
    dto->taskId = id;
    dto->taskString = taskString;
    dto->taskStatus = status;



    return createDtoResponse(returnCode, dto);
  }

 // BODY_DTO deserializes incoming JSON
  ENDPOINT("POST", "/tasks", createTask, BODY_DTO(Object<TaskDTO>, taskDto)){
    std::cout << "incoming data" << std::endl;
    std::cout << *taskDto->taskString << std::endl;

    std::string new_task = *taskDto->taskString;

    todo->addTaskApi(new_task);

    auto dto = MessageDto::createShared();
    dto->statusCode = 200;
    dto->message = "POST request received!";

    return createDtoResponse(Status::CODE_201, dto);
  }


  // TODO Insert Your endpoints here !!!
  
};

#include OATPP_CODEGEN_END(ApiController) ///< End Codegen

#endif /* MyController_hpp */
