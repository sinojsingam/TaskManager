#pragma once
#include <string>
#include <vector>

// class mod1c1{
// public:
//    void foo();
// };

namespace taskMod {
    // each individual task blueprint
    class Task{
        public:
            int m_task_id;
            std::string m_task;
            bool m_status;
            Task(int aTaskId, std::string &aTask, bool aStatus);
            void editTaskText(std::string &new_task_string);
            void toggleStatus();
            std::string toCSVFormat();
            void print();
    };

    // the whole list of tasks blueprint
    class Todo{
        private:
            static int m_autoID;
            std::vector<Task> m_tasks;
            void giveOrder();
            void saveFile();
            void loadFile();
            bool is_file_exist(std::string &fileName);
        public:
            bool m_isFileLoadedOnce;
            std::string m_dataFileName;
            std::string m_title;
            Todo(std::string new_title);
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
            void addTaskApi(std::string &new_task);
            void editTaskApi(int task_index,  std::string &new_task);
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
