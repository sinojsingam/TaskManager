#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Task{

public:
    int task_id;
    string task;
    bool status;

    Task(int aTaskId, string &aTask, bool aStatus){
        task = aTask;
        task_id = aTaskId;
        status = aStatus;
    }

    void editTaskText(string &new_task_string){
	    task = new_task_string;
    }

    void toggleStatus(){
        status = !status;
    }

    string toCSVFormat(){
        return to_string(task_id) + ',' + task + ',' + to_string(status) + "\n";
    }

    void print(){
        string status_text;
        if (status){
            status_text = "✅";
        } else {
            status_text = "⏳";
        };
        cout << task_id << ": " << task << ": " << status_text << endl;
    }
};

class Todo {
private:
    static int autoID;
    string title;
    string new_task;
    static string dataFileName;
    vector<Task> tasks;
    const char *longString = R""""(

  ░██████                     ░██████████                      ░██          ░███     ░███
 ░██   ░██                        ░██                          ░██          ░████   ░████
░██          ░██     ░██          ░██     ░██████    ░███████  ░██    ░██   ░██░██ ░██░██  ░██████   ░████████   ░██████    ░████████  ░███████  ░██░████
░██        ░██████ ░██████        ░██          ░██  ░██        ░██   ░██    ░██ ░████ ░██       ░██  ░██    ░██       ░██  ░██    ░██ ░██    ░██ ░███
░██          ░██     ░██          ░██     ░███████   ░███████  ░███████     ░██  ░██  ░██  ░███████  ░██    ░██  ░███████  ░██    ░██ ░█████████ ░██
 ░██   ░██                        ░██    ░██   ░██         ░██ ░██   ░██    ░██       ░██ ░██   ░██  ░██    ░██ ░██   ░██  ░██   ░███ ░██        ░██
  ░██████                         ░██     ░█████░██  ░███████  ░██    ░██   ░██       ░██  ░█████░██ ░██    ░██  ░█████░██  ░█████░██  ░███████  ░██
                                                                                                                                  ░██
                                                                                                                            ░███████
        )"""";

    void giveOrder(){
        char order;
        cout << "\nWhat would you like to do?\n[a]dd task, [e]dit task, [r]emove a task, [t]oggle status, [v]iew all tasks, [q]uit: ";
        cin >> order;
        // clear the line before passing 
        //  in to cin
        cin.ignore();
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
            cout << "Saving tasks and exiting..." << endl;
            saveFile();
            exit(1);
        } else {
            cout << "Invalid input, try again!" << endl;
            giveOrder();
        }

    };

    void saveFile(){
        ofstream DataFile(Todo::dataFileName);
        for (int i=0; i < tasks.size(); i++ ){
            DataFile << tasks[i].toCSVFormat();
        }
        DataFile.close();
        cout << "Saved successfully!" << endl;
    }

    void loadFile(){
        string taskDataLine;

        string taskDataId, taskString, taskStatus;

        char delimiter = ',';

        string splitTextArray[3]; // array to store split text
        int i =0; // index to store in array

        ifstream LoadedDataFile(Todo::dataFileName);
        string splitString; // temporarary split string
        bool tempBool;

        while (getline(LoadedDataFile, taskDataLine)){
            stringstream ss(taskDataLine);
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

    bool is_file_exist(string &fileName){
        ifstream infile(fileName);
        return infile.good();
    }

public:
    bool isFileLoadedOnce;

    Todo(string new_title){
        title = new_title;
        // initialize
        isFileLoadedOnce = false;
    };

    void addTask( int id,string &task, bool status ){
        tasks.push_back(Task(id, task, status));
    }

    void editTask(int task_ix){
	Task& task = getTaskByIndex(task_ix);
	cout << "Current Task: " << task.task << endl;
	cout << "Enter new task (or press Enter to keep current): ";
	string new_task;
	getline(cin >> ws, new_task);
	cout << "TEST " << new_task << endl;
	if (!new_task.empty()) {
	    task.editTaskText(new_task);
	}
	cout << "Updated Task: " << task.task << endl;
    }

    void getItems(){

        cout << "--- Your tasks are ---" << endl;

        for(int i = 0; i < tasks.size(); i++){
            tasks[i].print();
        }
        cout << "------" << endl;
    }

    void toggleItemStatus(int itemIx){
	Task& task = getTaskByIndex(itemIx);
	task.toggleStatus();
        cout << "Toggling item of index " << itemIx <<"'s status" << endl;
    }

    Task& getTaskByIndex(int index){
	Task task = tasks[0];
	if (index < 0 || index >= tasks.size()) {
        throw out_of_range("Index out of bounds");
    	}
	return tasks[index];
    }

    void runTaskApp(){
        if (is_file_exist(Todo::dataFileName) && !isFileLoadedOnce){
            // only load once for the lifecycle of the app
            cout << "Loading data..." << endl;
            loadFile();
            isFileLoadedOnce = true; // don't load again
            updateIndex();
            cout << longString << endl;
            getItems();
        }
        giveOrder();
        runTaskApp();
    }

    void addTaskRequest(){
        string new_task;
        cout << "Add a new task: ";

        getline(cin, new_task);

        addTask(Todo::autoID, new_task, false);
        Todo::autoID++; // increment auto id
        cout << "Added new task, enter 'v' to view your tasks!" << endl;
    }

    void editTaskRequest() {
        int index;
        printf("Enter the index number for the task to be edited: ");
        cin >> index;
        cout << "The task of id: " << index << " shall be edited." << endl;
	if (index)  {
	 editTask(index);
    }
    }

    void removeTaskRequest() {
        int index;
        printf("Enter the index number for the task to be removed: ");
        cin >> index;
        cout << "The task of id: " << index << " shall be removed." << endl;
        tasks.erase(tasks.begin() + index);
    }

    void toggleTaskRequest() {
        int index;
        cout << "Specify the index of the item you want to toggle:  ";
        cin >> index;
        toggleItemStatus(index);
        getItems();
    }

    void updateIndex(){
        for (int i=0; i < tasks.size(); i++){
            // update the indices of the vector items
            tasks[i].task_id = i;
        }
        // update the static variable 'id' instance
        vector<Task>::iterator lastItem = tasks.end() - 1; // get last item of vector
        Todo::autoID = lastItem->task_id + 1; // assign last item's index to the static var
}

};

// initialize static variable
int Todo::autoID = 0;
string Todo::dataFileName = "tasksdata.csv";

int main(){

    Todo my_list("My Projects");
    while(true){
        my_list.runTaskApp();
    }
    return 0;
};
