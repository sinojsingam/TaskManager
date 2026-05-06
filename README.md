Manage your tasks all in one app.

This is a simple CLI-based app for creating and keeping track of your most important tasks.


Compiling and building is done with CMAKE.

```bash
mkdir build
cd build
# - B tells CMake to use the given relative path as the location to generate files
cmake ..
make
```

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

