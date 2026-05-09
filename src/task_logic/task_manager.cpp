#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "task_logic/task_manager.hpp"


namespace taskMod {

    Task::Task(int aTaskId, std::string &aTask, bool aStatus){
        task = aTask;
        task_id = aTaskId;
        status = aStatus;

    };

    void Task::editTaskText(std::string &new_task_string){
        task = new_task_string;
    };

    void Task::toggleStatus(){
        status = !status;
    };

    std::string Task::toCSVFormat(){
        return std::to_string(task_id) + ',' + task + ',' + std::to_string(status) + "\n";
    }

    void Task::print(){
        std::string status_text;
        if (status){
            status_text = "✅";
        } else {
            status_text = "⏳";
        };
        std::cout << task_id << ": " << task << ": " << status_text << std::endl;
    }

    Todo::Todo(std::string new_title){
        // constructor
        title = new_title;
        isFileLoadedOnce = false;
    }

    void Todo::giveOrder(){
        char order;
        std::cout << "\nWhat would you like to do?\n[a]dd task, [e]dit task, [r]emove a task, [t]oggle status, [v]iew all tasks, [q]uit: ";
        std::cin >> order;
        // clear the line before passing 
        //  in to cin
        std::cin.ignore();
        if (order == 'a'){
            addTaskRequest();
            getItems();
        } else if (order == 't') {
            toggleTaskRequest();
            getItems();
        } else if (order == 'e') {
            editTaskRequest();
            getItems();
        } else if (order == 'r') {
            removeTaskRequest();
            updateIndex();
            getItems();
        } else if (order == 'v') {
            getItems();
        } else if (order == 'q') {
            std::cout << "Saving tasks and exiting..." << std::endl;
            saveFile();
            exit(1);
        } else {
            std::cout << "Invalid input, try again!" << std::endl;
            giveOrder();
        }
    };

    void Todo::saveFile(){
        std::ofstream DataFile(Todo::dataFileName);
        for (int i=0; i < tasks.size(); i++ ){
            DataFile << tasks[i].toCSVFormat();
        }
        DataFile.close();
        std::cout << "Saved successfully!" << std::endl;
    }
    void Todo::loadFile(){
        std::string taskDataLine;

        std::string taskDataId, taskString, taskStatus;

        char delimiter = ',';

        std::string splitTextArray[3]; // array to store split text
        int i =0; // index to store in array

        std::ifstream LoadedDataFile(Todo::dataFileName);
        std::string splitString; // temporarary split string
        bool tempBool;

        while (getline(LoadedDataFile, taskDataLine)){
            std::stringstream ss(taskDataLine);
            while(getline(ss, splitString, delimiter)){
                splitTextArray[i] = splitString;
                i++;
            }
            // recast to boolean
            if (splitTextArray[2] == "0"){
                tempBool = false;
            } else {
                tempBool = true;
            }
            // cast to int and add new task per line
            addTask(stoi(splitTextArray[0]), splitTextArray[1], tempBool);
            i = 0; // once the line is done, reset the temp array index
        }

        LoadedDataFile.close();
    }

    bool Todo::is_file_exist(std::string &fileName){
        std::ifstream infile(fileName);
        return infile.good();
    }

    void Todo::addTask( int id, std::string &task, bool status ){
        tasks.push_back(Task(id, task, status));
    }

    void Todo::editTask(int task_ix){
        Task& task = getTaskByIndex(task_ix);
        std::cout << "Current Task: " << task.task << std::endl;
        std::cout << "Enter new task (or press Enter to keep current): ";
        std::string new_task;
        getline(std::cin >> std::ws, new_task);
        std::cout << "TEST " << new_task << std::endl;

        if (!new_task.empty()) {
            task.editTaskText(new_task);
        }

        std::cout << "Updated Task: " << task.task << std::endl;
    }

    void Todo::getItems(){

        std::cout << "--- Your tasks are ---" << std::endl;

        for(int i = 0; i < tasks.size(); i++){
            tasks[i].print();
        }
        std::cout << "------" << std::endl;
    }

    void Todo::toggleItemStatus(int itemIx){
        Task& task = getTaskByIndex(itemIx);
        task.toggleStatus();
        std::cout << "Toggling item of index " << itemIx <<"'s status" << std::endl;
    }

    Task& Todo::getTaskByIndex(int index){
        Task task = tasks[0];
        if (index < 0 || index >= tasks.size()) {
            throw std::out_of_range("Index out of bounds");
        }
        return tasks[index];
    }
    void Todo::runTaskApp(){
        if (is_file_exist(Todo::dataFileName) && !isFileLoadedOnce){
            // only load once for the lifecycle of the app
            std::cout << "Loading data..." << std::endl;
            loadFile();
            isFileLoadedOnce = true; // don't load again
            updateIndex();
            // std::cout << longString << std::endl;
            getItems();
        }
        giveOrder();
        runTaskApp();
    }

    void Todo::addTaskRequest(){
        std::string new_task;
        std::cout << "Add a new task: ";

        getline(std::cin, new_task);

        addTask(Todo::autoID, new_task, false);
        Todo::autoID++; // increment auto id
        std::cout << "Added new task, enter 'v' to view your tasks!" << std::endl;
    }

    void Todo::editTaskRequest() {
        int index;
        printf("Enter the index number for the task to be edited: ");
        std::cin >> index;
        std::cout << "The task of id: " << index << " shall be edited." << std::endl;

        if (index)  {
            editTask(index);
        }
    }

    void Todo::removeTaskRequest() {
        int index;
        printf("Enter the index number for the task to be removed: ");
        std::cin >> index;
        std::cout << "The task of id: " << index << " shall be removed." << std::endl;
        tasks.erase(tasks.begin() + index);
    }

    void Todo::toggleTaskRequest() {
        int index;
        std::cout << "Specify the index of the item you want to toggle:  ";
        std::cin >> index;
        toggleItemStatus(index);
        getItems();
    }

    void Todo::updateIndex(){
        for (int i=0; i < tasks.size(); i++){
            // update the indices of the vector items
            tasks[i].task_id = i;
        }

        // update the static variable 'id' instance
        // get last item of vector
        std::vector<Task>::iterator lastItem = tasks.end() - 1; 
        // assign last item's index to the static var
        Todo::autoID = lastItem->task_id + 1;
    }

    int Todo::autoID = 0;
    std::string Todo::dataFileName = "tasksdata.csv";

}
