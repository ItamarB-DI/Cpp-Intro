
#pragma once

#include <fstream> 
#include <string>
#include <vector>


class FileHandler {
public:
    explicit FileHandler(const std::string path, std::ios_base::openmode permissions); //Ctor
    FileHandler(FileHandler &other); //CCtor non-const because that tellg/p are modifying 
    FileHandler(FileHandler &&other) noexcept; //MCtor

    FileHandler& operator=(FileHandler &other); //Copy Assignment non-const because that tellg/p are modifying
    FileHandler& operator=(FileHandler &&other) noexcept; //Move Assignment

    void Open(std::ios_base::openmode permissions = std::ios_base::openmode(0));
    void Close();
    void Write(const std::vector<char> data);
    std::vector<char>  Read(size_t size_to_read);
    std::uintmax_t Size();

    ~FileHandler() noexcept; //Dtor

private:
    std::fstream m_stream;
    std::string m_path;
    std::ios_base::openmode m_permissions;
    std::streampos m_read_pos;

};