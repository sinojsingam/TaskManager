#pragma once
#include <string>
#include <vector>

// class mod1c1{
// public:
//    void foo();
// };

namespace taskMod {
    int hello();
    // each individual task blueprint
    class Task{
        public:
            int task_id;
            std::string task;
            bool status;
            Task(int aTaskId, std::string &aTask, bool aStatus);
            void editTaskText(std::string &new_task_string);
            void toggleStatus();
            std::string toCSVFormat();
            void print();
    };

    // the whole list of tasks blueprint
    class Todo{
        private:
            static int autoID;
            std::string title;
            std::string new_task;
            static std::string dataFileName;
            std::vector<Task> tasks;
            void giveOrder();
            void saveFile();
            void loadFile();
            bool is_file_exist(std::string &fileName);
        public:
            bool isFileLoadedOnce;
            Todo(std::string new_title);
            // Task class control
            void addTask( int id, std::string &task, bool status );
            void editTask(int task_ix);
            void getItems();
            void toggleItemStatus(int itemIx);
            Task& getTaskByIndex(int index);
            // What the user wants todo?
            void addTaskRequest();
            void editTaskRequest();
            void removeTaskRequest();
            void toggleTaskRequest();
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
