To run:
* Make sure all dependencies are installed (look in dependencies.md)
* Change the `const std::string DATA_FOLDER_PATH` to the directory where the flightradar24 data is stored
* Optional: Change the number of threads in main.cpp, 1 thread should be ok for the csv part, but if there is RAM to spare, feel free to increase the zip threads
* Make sure a role with `LOGIN` and username `wireflies` with password `wireflies` has been created in postgresql
* Make sure a database `wireflies` has been created in postgresql
* `make && ./a.out`
* Wait for a few hours for the program to insert all data