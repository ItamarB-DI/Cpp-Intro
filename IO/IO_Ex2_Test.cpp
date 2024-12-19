
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Ex1.hpp"

void firstTest();
void simpleCCtorTest();

inline std::string vecToStr(const std::vector<char> vec);
void createFile(const std::string &filename);

int main() {
    try {
        firstTest();
        simpleCCtorTest();
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
