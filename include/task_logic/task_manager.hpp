#pragma once
#include <string>
#include <vector>
#include <set>
#include "db/MyClient.hpp"

namespace db {
    class MyClient;
}

namespace taskMod {
  // each individual task blueprint
  class Task{
    private:
      int m_task_id;
      std::string m_task;
      bool m_status;
    public:
      Task(
          int aTaskId,
          std::string &aTask,
          bool aStatus
      );
      void updateIndex(int new_index);
      int getId() const;
      const std::string& getTask() const;
      bool getTaskStatus() const;
      void editTaskText(std::string &new_task_string);
      void editTaskStatus(bool status);
      void toggleStatus();
      std::string toCSVFormat();
      std::string print();
  };

  // the whole list of tasks blueprint
  class Todo{
    private:
      std::shared_ptr<db::MyClient> m_db;
      static int m_autoID;
      std::vector<Task> m_tasks;
      void giveOrder();
      void saveFile();
      void loadFile();
      bool is_file_exist(std::string &fileName);
      std::string sanitizeText(
              std::string &original_text,
              const char replacement_char='_',
              std::set<char> extra_checks={});


    public:
      bool m_isFileLoadedOnce;
      std::string m_dataFileName;
      std::string m_title;
    // Default m_db to nullptr for CLI testing mode
      explicit Todo(
        std::string new_title,
        std::shared_ptr<db::MyClient> db= nullptr
      );
      ~Todo();
      // Task class control
      void addTask( int id, std::string &task, bool status );
      void editTask(int task_ix);
      void getItems();
      int sizeOfList();
      void toggleItemStatus(int itemIx);
      Task& getTaskByIndex(int index);
      // CLI mode: What the user wants todo?
      void addTaskRequest();
      void editTaskRequest();
      void removeTaskRequest();
      void toggleTaskRequest();
      // API mode
      void addTaskApi(std::string &new_task, bool status=false);
      void editTaskApi(int task_index,  std::string &new_task, bool status);
      void removeTaskApi(int task_index);
      void toggleTaskApi(int task_index);
      std::vector<Task> viewTasks(int task_index);

      void updateIndex();
      void runTaskApp();
  };
}


//     const char *longString = R""""(
//
//   ░██████                     ░██████████                      ░██          ░███     ░███
//  ░██   ░██                        ░██                          ░██          ░████   ░████
// ░██          ░██     ░██          ░██     ░██████    ░███████  ░██    ░██   ░██░██ ░██░██  ░██████   ░████████   ░██████    ░████████  ░███████  ░██░████
// ░██        ░██████ ░██████        ░██          ░██  ░██        ░██   ░██    ░██ ░████ ░██       ░██  ░██    ░██       ░██  ░██    ░██ ░██    ░██ ░███
// ░██          ░██     ░██          ░██     ░███████   ░███████  ░███████     ░██  ░██  ░██  ░███████  ░██    ░██  ░███████  ░██    ░██ ░█████████ ░██
//  ░██   ░██                        ░██    ░██   ░██         ░██ ░██   ░██    ░██       ░██ ░██   ░██  ░██    ░██ ░██   ░██  ░██   ░███ ░██        ░██
//   ░██████                         ░██     ░█████░██  ░███████  ░██    ░██   ░██       ░██  ░█████░██ ░██    ░██  ░█████░██  ░█████░██  ░███████  ░██
//                                                                                                                                   ░██
//                                                                                                                             ░███████
//         )"""";
//
