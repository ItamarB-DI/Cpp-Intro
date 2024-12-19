#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include "Ex1.hpp"

void simpleCopyTest();
void simpleMoveTest();
void simpleMoveOpTest();

void checkFileLineCount(const std::string &file_name, int expected_lines);
void createFile(const std::string filename);

int main() {

    try {
        simpleCopyTest();
        simpleMoveTest();
        simpleMoveOpTest();

        std::cout << "All tests passed successfully!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
    }

    return 0;
}

void simpleCopyTest() {

    const std::string file_name = "test_file1.txt";
    std::ios_base::openmode prem = std::ios::in | std::ios::out;

    createFile(file_name);

    FileHandler file1(file_name, prem);

    std::fstream &file1_fd = file1.getFile();
    if (!file1_fd.is_open()) {
        throw std::runtime_error("Failed to open file1.");
    }

    file1_fd << "First Test" << std::endl;
    file1_fd.flush();

    FileHandler copied_file(file1);
    std::string line;

    std::fstream &copied_file_fd = copied_file.getFile();
    if (!copied_file_fd.is_open()) {
        throw std::runtime_error("Failed to open copied_file.");
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

    createFile(file_name);

    FileHandler file1(file_name, prem);
    std::fstream &file1_fd = file1.getFile();

    file1_fd << "Move Test" << std::endl;
    file1_fd.flush();

    FileHandler moved_file(std::move(file1));
    std::fstream &moved_file_fd = moved_file.getFile();

    moved_file_fd.seekg(0, std::ios::beg); 
    std::string line;
    if (!std::getline(moved_file_fd, line)) {
        throw std::runtime_error("Move constructor failed: Unable to read from file.");
    }

    if (line != "Move Test") {
        throw std::runtime_error("Move constructor failed: Content mismatch.");
    }

    try {
        std::fstream &invalid_fd = file1.getFile();

        invalid_fd.exceptions(std::ios::failbit | std::ios::badbit);
        invalid_fd << "This should fail" << std::endl;

        throw std::runtime_error("Move constructor failed: file1 should not be valid after move.");

    } catch (const std::ios_base::failure &e) {

    }

    std::cout << "simpleMoveTest passed!" << std::endl;
}


void simpleMoveOpTest() {
    const std::string file_name = "test_file3.txt";
    std::ios_base::openmode prem = std::ios::in | std::ios::out | std::ios::app;

    createFile(file_name);
    
    FileHandler file1(file_name, prem);
    std::fstream &file1_fd = file1.getFile();

    file1_fd << "Move operator Test" << std::endl;
    file1_fd.flush();

    /**********************************************/
    const std::string file_name2 = "test_file4.txt";   
    createFile(file_name2);

    FileHandler file2(file_name2, prem);
    std::fstream &file2_fd = file2.getFile();

    file2_fd << "Should Have Only One Line" << std::endl;
    file2_fd.flush();

    file2 = std::move(file1);

    file2.getFile() << "Second Line" << std::endl;
    file2.getFile().flush();

    checkFileLineCount(file_name, 2);
    checkFileLineCount(file_name2, 1);

    std::cout << "simpleMoveOpTest passed!" << std::endl;
}


/*********************** Helper Functions ******************************/

void checkFileLineCount(const std::string &file_name, int expected_lines) {
    std::ifstream file(file_name, std::ios::in);

    int line_count = 0;
    std::string line;
    while (std::getline(file, line)) {
        ++line_count;
    }

    if (line_count != expected_lines) {
        throw std::runtime_error("Count Line Check Failed.");
    }   
}

void createFile(const std::string filename) {

    std::ofstream file(filename, std::ios::out);

    if (!file) {
        throw std::runtime_error("Failed to create file: " + filename);
    }

    file.close();
}