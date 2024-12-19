#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "Ex1.hpp"

void firstTest();
void createFile(const std::string &filename);

int main() {
    try {
        firstTest();
        std::cout << "All tests passed successfully!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Test Failed: " << e.what() << std::endl;
    }

    return 0;
}

void firstTest() {
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
    std::string first_read_str = first_read_vec.data();
    if (first_read_str != "Test") {
        throw std::runtime_error("Read method failed: " + filename);
    }

    if (fh.Size() != str1.size() - first_read_vec.size()) {
        throw std::runtime_error("Size method failed: " + filename);
    }

    std::vector<char> read_rest_vec = fh.Read(str1.size() - 4);
    std::string read_rest_str = read_rest_vec.data();
    if (read_rest_str != " Write\n") {
        throw std::runtime_error("Read method failed: " + filename);
    }

    std::cout << "Testing Read, Write & Size pass" << std::endl;
}

void createFile(const std::string &filename) {
    std::ofstream file(filename, std::ios::out);

    if (!file) {
        throw std::runtime_error("Failed to create file: " + filename);
    }

    file.close();
}
