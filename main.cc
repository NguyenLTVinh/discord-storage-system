#include "filesplitter.h"
#include <iostream>
#include <string>
#include <sstream>

int main() {
    FileSplitter fileSplitter;
    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "quit") {
            break;
        } else if (command.substr(0, 5) == "split") {
            std::istringstream iss(command);
            std::string splitCommand;
            std::string filePath;
            iss >> splitCommand >> filePath;

            if (!filePath.empty()) {
                fileSplitter.splitFile(filePath);
            } else {
                std::cout << "Invalid command. Usage: split [file path]" << std::endl;
            }
        }
    }
}