
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Ex1.hpp"

void writeReadSizeTest();
void simpleCCtorTest();
void simpleMCtorTest();
void reopenTest();


inline std::string vecToStr(const std::vector<char> vec);
void createFile(const std::string &filename);

int main() {
    try {
        writeReadSizeTest();
        simpleCCtorTest();
        simpleMCtorTest();
        reopenTest();
        std::cout << "All tests passed successfully!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Test Failed: " << e.what() << std::endl;
    }

    return 0;
}

void writeReadSizeTest() {
    const std::string filename = "write_read_test.txt";
    createFile(filename);

    auto permissions = std::ios::out | std::ios::app | std::ios::in;

    FileHandler fh(filename, permissions);

    const std::string str1 = "Test Write\n";  
    std::vector<char> test1(str1.begin(), str1.end());
    
    fh.Write(test1);

    if (fh.Size() != str1.size()) {
        throw std::runtime_error("Failed in the Write or Size method: " + filename);
    }

    std::vector<char> first_read_vec = fh.Read(4);
    if (vecToStr(first_read_vec) != "Test") {
        throw std::runtime_error("Read method failed: " + filename);
    }

    if (fh.Size() != str1.size() - first_read_vec.size()) {
        throw std::runtime_error("Size method failed: " + filename);
    }

    std::vector<char> read_rest_vec = fh.Read((str1.size() - 4));
    if (vecToStr(read_rest_vec) != " Write\n") {
        throw std::runtime_error("Read method failed: " + filename);
    }

    std::cout << "Testing Read, Write & Size pass" << std::endl;
}

void simpleCCtorTest() {

    const std::string file_name = "copy_test.txt";
    std::ios_base::openmode prem = std::ios::in | std::ios::out;

    createFile(file_name);

    FileHandler fh(file_name, prem);

    const std::string str1 = "Test Copy";  
    std::vector<char> test1(str1.begin(), str1.end());
    
    fh.Write(test1);
    fh.Read(5);

    FileHandler copied_file(fh);
    
    std::vector<char> copy_vec = copied_file.Read(str1.size() - 5);
    std::vector<char> main_vec = fh.Read(str1.size() - 5);

    if (vecToStr(copy_vec) != "Copy" || vecToStr(main_vec) != "Copy" ) {
        throw std::runtime_error("CCtor method failed: " + file_name);
    }

    std::cout << "simpleCCtorTest passed!" << std::endl;
}

void simpleMCtorTest() {

    const std::string file_name = "move_test.txt";
    std::ios_base::openmode prem = std::ios::in | std::ios::out;

    createFile(file_name);

    FileHandler fh(file_name, prem);

    const std::string str1 = "Test Move";  
    std::vector<char> test1(str1.begin(), str1.end());
    
    fh.Write(test1);
    fh.Read(5);

    FileHandler moved_file(std::move(fh));
    
    std::vector<char> move_vec = moved_file.Read(str1.size() - 5);

    if (vecToStr(move_vec) != "Move") {
        throw std::runtime_error("MCtor method failed: " + file_name);
    }

    try {
        fh.Read(1);

        throw std::runtime_error("Move constructor failed: file1 should not be valid after move.");

    } catch (const std::ios_base::failure &e) {
        std::cout << "simpleMCtorTest passed!" << std::endl;
    }
}

void reopenTest() {

    const std::string file_name = "reopen_test.txt";
    std::ios_base::openmode prem = std::ios::out;
    FileHandler fh(file_name, prem);

    const std::string str1 = "Test Reopen\n"; 
    const std::vector<char> test1(str1.begin(), str1.end());

    const std::string str2 = "Test Reopen2"; 
    const std::vector<char> test2(str2.begin(), str2.end());

    fh.Write(test1);
    fh.Write(test2);

    try {
        fh.Read(2);
        throw std::runtime_error("Permission Failed: Opened only for write ");
    } catch (const std::exception &e) {

    }

    fh.ReOpen(std::ios::in);
    try {
        const std::string str3 = "should not write it"; 
        const std::vector<char> test3(str3.begin(), str3.end());
        fh.Write(test3);

        throw std::runtime_error("reopen Failed: Opened only for read ");
    } catch (const std::exception &e) {
        std::cout << "reOpen passed!" << std::endl;
    }

}

void createFile(const std::string &filename) {
    std::ofstream file(filename, std::ios::out);

    if (!file) {
        throw std::runtime_error("Failed to create file: " + filename);
    }

    file.close();
}

std::string vecToStr(std::vector<char> vec) {
    vec.push_back('\0');

    return vec.data();
}
