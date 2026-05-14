#ifndef DTOs_hpp

#define DTOs_hpp

#include "oatpp/Types.hpp"
#include "oatpp/macro/codegen.hpp"

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Message Data-Transfer-Object
 */
class MessageDto : public oatpp::DTO {

    DTO_INIT(MessageDto, DTO /* Extends */)

    DTO_FIELD(Int32, statusCode);   // Status code field
    DTO_FIELD(String, message);     // Message field

};

/** A Single Task **/
class TaskDTO : public oatpp::DTO {

    DTO_INIT(TaskDTO, DTO /* Extends */)

    DTO_FIELD(Int32, statusCode);   // Status code field
    DTO_FIELD(Int32, taskId);     // Task id
    DTO_FIELD(String, taskString);     // the task string
    DTO_FIELD(Boolean, taskStatus); // if the task is complete
};

/** All Tasks **/
class TasksDTO : public oatpp::DTO {

    DTO_INIT(TasksDTO, DTO /* Extends */)

    DTO_FIELD(Int32, statusCode);   // Status code field
    DTO_FIELD(String, message);     // Message field
    DTO_FIELD(Vector<Object<TaskDTO>>, tasks);
};

/** Using only for POST or PUT **/
class IncomingTaskDto : public oatpp::DTO {

    DTO_INIT(IncomingTaskDto, DTO /* Extends */)

    DTO_FIELD(String, taskString);     // Message field
    DTO_FIELD(Boolean, taskStatus); // if the task is complete
};

// here to add more DTOs

/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */

