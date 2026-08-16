#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "task_logic/task_manager.hpp"
#include "db/MyClient.hpp"


namespace taskMod {

    Task::Task(
        int aTaskId,
        std::string &aTask,
        bool aStatus
    ){
        m_task = aTask;
        m_task_id = aTaskId;
        m_status = aStatus;
    };

    void Task::updateIndex(int new_index){
      m_task_id = new_index;
    }

    const std::string& Task::getTask() const {
      return m_task;
    };

    bool Task::getTaskStatus() const{
      return m_status;
    };

    int Task::getId() const{
      return m_task_id;
    }

    void Task::editTaskText(std::string &new_task_string){
        m_task = new_task_string;
    };

    void Task::editTaskStatus(bool status){
      m_status = status;
    }

    void Task::toggleStatus(){
        m_status = !m_status;
    };

    std::string Task::toCSVFormat(){
        return std::to_string(m_task_id) + ',' + m_task + ',' + std::to_string(m_status) + "\n";
    }

    std::string Task::print(){
        std::string status_text;
        if (m_status){
            status_text = "✅";
        } else {
            status_text = "⏳";
        };
        return std::to_string(m_task_id) + ": " + m_task + ": " + status_text + '\n';
    }

    Todo::Todo(
        std::string new_title,
        std::shared_ptr<db::MyClient> db
        ):  m_title(std::move(new_title)),
            m_db(std::move(db)),
            m_isFileLoadedOnce(false)
        {
        // constructor
        std::string clean_title = sanitizeText(m_title, '_');
        m_dataFileName = clean_title + ".csv";
        if (m_db &&
            is_file_exist(m_dataFileName) &&
            !m_isFileLoadedOnce){
            // only load once for the lifecycle of the app
            std::cout << "Loading data from "<< m_dataFileName << std::endl;
            loadFile();
            m_isFileLoadedOnce = true; // don't load again
            updateIndex();
            std::cout << "Loaded data..." << std::endl;
        }
    }

    Todo::~Todo(){
        std::cout << "writing to file "<< m_dataFileName << std::endl;
        saveFile();

    }

    std::string Todo::sanitizeText(
            std::string &original_text,
            const char replacement_char,
            std::set<char> extra_checks)
    {
        /**
         * sanitize an input string.
         *
         * The input reference is copied and the copy is returned.
         *
         * Args:
         * the string reference to sanitize
         * [optional] the replacement character
         * [optional] any additional nono letters to add to the check set
         * */

        // default set of no-no letters
        std::set<char> check {'%', ' ', '#', '-'};
        // append any additional checks to def ones
        if (!extra_checks.empty()){
            check.insert(extra_checks.begin(), extra_checks.end());
        }

        for (char t : check){
            std::cout << t << std::endl;
        }


        // create a copy of the input ref
        std::string copy_text = original_text;


        // iterate over each letter
        for(int ix=0; ix < copy_text.length(); ix++){
            // pull the letter ref
            char& letter = copy_text[ix];
            // lower case the letter
            letter = tolower(letter);

            // if in nono list, replace with replacement character
            if (check.find(letter) != check.end()){
                letter = replacement_char;
            }
        }

        return copy_text;
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
        std::ofstream DataFile(m_dataFileName);
        for (int i=0; i < m_tasks.size(); i++ ){
            DataFile << m_tasks[i].toCSVFormat();
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

        std::ifstream LoadedDataFile(m_dataFileName);
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
        m_tasks.push_back(Task(id, task, status));
        updateIndex();
    }

    void Todo::editTask(int task_ix){
        Task& task = getTaskByIndex(task_ix);
        std::cout << "Current Task: " << task.print() << std::endl;
        std::cout << "Enter new task (or press Enter to keep current): ";
        std::string new_task;
        getline(std::cin >> std::ws, new_task);

        if (!new_task.empty()) {
            task.editTaskText(new_task);
        }

        std::cout << "Updated Task: " << task.print() << std::endl;
    }

    void Todo::getItems(){

        std::cout << "--- Your tasks are ---" << std::endl;

        for(int i = 0; i < m_tasks.size(); i++){
            std::cout << m_tasks[i].print() << std::endl;
        }
        std::cout << "------" << std::endl;
    }

    void Todo::toggleItemStatus(int itemIx){
        Task& task = getTaskByIndex(itemIx);
        task.toggleStatus();
        std::cout << "Toggling item of index " << itemIx <<"'s status" << std::endl;
    }

    Task& Todo::getTaskByIndex(int index){
        Task task = m_tasks[0];
        if (index < 0 || index >= m_tasks.size()) {
            throw std::out_of_range("Index out of bounds");
        }
        return m_tasks[index];
    }

    int Todo::sizeOfList(){
      return m_tasks.size();
    }

    void Todo::runTaskApp(){
        if (is_file_exist(m_dataFileName) && !m_isFileLoadedOnce){
            // only load once for the lifecycle of the app
            std::cout << "Loading data..." << std::endl;
            loadFile();
            m_isFileLoadedOnce = true; // don't load again
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

        addTask(m_autoID, new_task, false);
        m_autoID++; // increment auto id
        std::cout << "Added new task, enter 'v' to view your tasks!" << std::endl;
    }

    void Todo::editTaskApi(
        int task_index,
        std::string &new_task,
        bool status
        ){
        if (task_index >= 0)  {
            Task& task = getTaskByIndex(task_index);
            if (!new_task.empty()) {
                    task.editTaskText(new_task);
            }
            task.editTaskStatus(status);
        }
    };

    void Todo::addTaskApi(std::string &task, bool status){
        addTask(m_autoID, task, status);
        m_autoID++; // increment auto id
        updateIndex();
    };

    void Todo::editTaskRequest() {
        int index;
        printf("Enter the index number for the task to be edited: ");
        std::cin >> index;
        std::cout << "The task of id: " << index << " shall be edited." << std::endl;

        if (index)  {
            editTask(index);
        }
    }

    void Todo::removeTaskApi(int task_index){
        m_tasks.erase(m_tasks.begin() + task_index);
        updateIndex();
    };

    void Todo::toggleTaskApi(int task_index){
        toggleItemStatus(task_index);
    };

    std::vector<Task> Todo::viewTasks(int task_index){
        std::vector<Task> single_task;
        if (task_index >= 0){
            Task& task = getTaskByIndex(task_index);
            std::string taskString = task.getTask();
            single_task.push_back(Task(task.getId(), taskString, task.getTaskStatus()));
            return single_task;
        }
        return m_tasks;
    };


    void Todo::removeTaskRequest() {
        int index;
        printf("Enter the index number for the task to be removed: ");
        std::cin >> index;
        std::cout << "The task of id: " << index << " shall be removed." << std::endl;
        m_tasks.erase(m_tasks.begin() + index);
    }

    void Todo::toggleTaskRequest() {
        int index;
        std::cout << "Specify the index of the item you want to toggle:  ";
        std::cin >> index;
        toggleItemStatus(index);
        getItems();
    }

    void Todo::updateIndex(){
        // if task list is empty do nothing
        if (m_tasks.size() == 0){
          return;
        };

        for (int i=0; i < m_tasks.size(); i++){
            // update the indices of the vector items
            m_tasks[i].updateIndex(i);
        }

        // update the static variable 'id' instance
        // get last item of vector
        std::vector<Task>::iterator lastItem = m_tasks.end() - 1; 
        // assign last item's index to the static var
        m_autoID = lastItem->getId() + 1;
    }

    int Todo::m_autoID = 0;

}
