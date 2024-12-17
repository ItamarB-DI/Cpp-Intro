#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "Ex1.hpp"

void simpleCopyTest();
void simpleMoveTest();

int main() {

    try {
        simpleCopyTest();
        simpleMoveTest();

        std::cout << "All tests passed successfully!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
    }

    return 0;
}

void simpleCopyTest() {

    const std::string file_name = "test_file1.txt";
    std::ios_base::openmode prem = std::ios::in | std::ios::out;

    FileHandler::createFile(file_name);

    FileHandler file1(file_name, prem);

    std::fstream &file1_fd = file1.getFD();
    if (!file1_fd.is_open()) {
        throw std::runtime_error("Failed to open file1 for reading.");
    }

    file1_fd << "First Test" << std::endl;
    file1_fd.flush();

    FileHandler copied_file(file1);
    std::string line;

    std::fstream &copied_file_fd = copied_file.getFD();
    if (!copied_file_fd.is_open()) {
        throw std::runtime_error("Failed to open copied_file for reading.");
    }
    
    copied_file_fd.seekg(0, std::ios_base::beg); 

    if (!std::getline(copied_file_fd, line)) {
        throw std::runtime_error("Copy failed: unable to read from file.");
    }

    if (line != "First Test") {
        throw std::runtime_error("Copy failed: content mismatch.");
    }

    std::cout << "simpleCopyTest passed!" << std::endl;
}

void simpleMoveTest()
{
    const std::string file_name = "test_file2.txt";
    std::ios_base::openmode prem = std::ios::in | std::ios::out;

    FileHandler::createFile(file_name);

    FileHandler file1(file_name, prem);
    std::fstream &file1_fd = file1.getFD();

    file1_fd << "Move Test" << std::endl;
    file1_fd.flush();

    FileHandler moved_file(std::move(file1));
    std::fstream &moved_file_fd = moved_file.getFD();

    moved_file_fd.seekg(0, std::ios::beg); 
    std::string line;
    if (!std::getline(moved_file_fd, line)) {
        throw std::runtime_error("Move constructor failed: Unable to read from file.");
    }

    if (line != "Move Test") {
        throw std::runtime_error("Move constructor failed: Content mismatch.");
    }

    try {
        std::fstream &invalid_fd = file1.getFD();

        invalid_fd.exceptions(std::ios::failbit | std::ios::badbit);
        invalid_fd << "This should fail" << std::endl;

        throw std::runtime_error("Move constructor failed: file1 should not be valid after move.");

    } catch (const std::ios_base::failure &e) {

    }

    std::cout << "simpleMoveTest passed!" << std::endl;
}
