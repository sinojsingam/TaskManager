#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Task{

public:
	static int id;
	int task_id;
	string task;
	bool status;

	Task( string &aTask){
		task = aTask;
		task_id = id;
		status = false;
		id++;
	}
	
	void toggleStatus(){
		status = !status;
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
		string title;
		string new_task;
		vector<Task> tasks;
		
		void giveOrder(){
			char order;
			cout << "What would you like to do? [a]dd task, [r]emove a task, [t]oggle status, [v]iew all tasks, [q]uit: ";
			cin >> order;
			// clear the line before passing 
			//  in to cin
			cin.ignore();
			if (order == 'a'){
				addTask();
			} else if (order == 't') {
				toggleTask();
			} else if (order == 'r') {
				removeTask();
                updateIndex();
                getItems();
			} else if (order == 'v') {
				getItems();
			} else if (order == 'q') {
				cout << "Gracefully exiting app" << endl;
				exit(1);
			} else {
				cout << "Invalid input, try again!" << endl;
				giveOrder();
			}

		};

	public:
		Todo(string new_title){
			title = new_title;
		};

        void addTask(string &task){
            tasks.push_back(Task(task));
        }

		void getItems(){

			cout << "--- Your tasks are ---" << endl;

			for(int i = 0; i < tasks.size(); i++){
				tasks[i].print();
			}
			cout << "------" << endl;
		}

		void toggleItemStatus(int itemIx){
			
			for (int i = 0; i < tasks.size(); i++){
				if (itemIx == i){
					tasks[i].toggleStatus();
				};
			}
			cout << "Toggling item of index " << itemIx <<"'s status" << endl;
		}
		
		void runTaskApp(){

			giveOrder();
			runTaskApp();
		}
	
		void addTask(){
			string new_task;
			cout << "Add a new task: ";

			getline(cin, new_task);

            addTask(new_task);
			cout << "Added new task, enter 'v' to view your tasks!" << endl;
		}

		void removeTask() {
			int index;
            printf("Enter the index number for the task to be removed: ");
			cin >> index;
            cout << "The task of id: " << index << " shall be removed." << endl;
            tasks.erase(tasks.begin() + index);
		}

		void toggleTask() {
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
            Task::id = lastItem->task_id + 1; // assign last item's index to the static var
    }

};

// initialize static variable
int Task::id = 0;

int main(){
	
	Todo my_list("My Projects");
	while(true){
		my_list.runTaskApp();
	}
	return 0;
};
