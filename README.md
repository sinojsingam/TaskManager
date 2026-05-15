Manage your tasks all in one app.

A REST API that manipulates your personal todo list!

## Routes:

### GET:

`/tasks`

Description: shows all your list

Returns list of json objects and metadata.

`/tasks{id}`

Description: retrieve a specific task


Returns a json:

```json
{
    "statusCode": 200,
    "taskId": 1,
    "taskString": "run a marathon",
    "taskStatus": false
}
```

### PUT

`/tasks{id}`

Description: Edits an already existing task's text and/or a task's status.
The JSON body must include at least one the below key-value pairs showed below.

Expects a JSON in the request body:

```json

{
    "taskString": "your new string"
    "taskStatus": false or true
}
```

### PATCH

`/tasks{id}`

Description: Toggles the status of a particular task to the opposite of whatever it was before.

### DELETE

`/tasks{id}`

Description: Removes the entry from the todo list.


## Prerequisites

- Git
- C++ compiler supporting C++ version >= 11
- Make
- CMake version >= 3.1
- The Oatpp library

    ```bash
    git clone https://github.com/oatpp/oatpp.git
    cd oatpp/

    mkdir build && cd build

    cmake ..
    make install

    ```

## Run with Docker

```bash
docker build -t TaskManager .
docker run -p 8000:8000 -d TaskManager
```

## Compile
Then you can compile and run the Taskmanager. Running the program opens a port at 8000.

```bash
mkdir build && cd build

cmake ..
make

# run the app
./TaskManager
```

## Neovim stuff

LSPs like clangd don't automatically read CMakeLists.txt. They look for a "map" of your project called a compile_commands.json file. Without it, clangd has no idea where your include folder is or what libraries you are linking.

Update the LSP (if using neovim) in the build folder

```bash
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
```
Clangd expects the compile_commands.json to be in your project root. If you are building in a /build directory (which you should!), you need to symlink it:

From root of the project
```bash
ln -s build/compile_commands.json .
```
