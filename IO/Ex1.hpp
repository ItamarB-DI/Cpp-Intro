
#pragma once

#include <fstream> 
#include <string>
#include <vector>


class FileHandler {
public:
    explicit FileHandler(const std::string path, std::ios_base::openmode permissions); // Ctor
    FileHandler(FileHandler &other); // CCtor non-const because that tellg/p are modifying 
    FileHandler(FileHandler &&other) noexcept; // MCtor

    FileHandler& operator=(FileHandler &other); // Copy Assignment non-const because that tellg/p are modifying
    FileHandler& operator=(FileHandler &&other) noexcept; // Move Assignment

    void reOpen(std::ios_base::openmode permissions); // Closes and reopen the file with new permissions
    
    void write(const std::vector<char> data);
    std::vector<char> read(size_t size_to_read);
    
    void seekg(std::streampos pos);
    void seekg(std::streamoff off, std::ios_base::seekdir way);
    
    std::uintmax_t size();
    std::uintmax_t sizeUntilEOF();

    ~FileHandler() noexcept; // Dtor

private:
    void closeFile();
    void openFile();

    std::fstream m_stream;
    std::string m_path;
    std::ios_base::openmode m_permissions;
    std::streampos m_read_pos;

};