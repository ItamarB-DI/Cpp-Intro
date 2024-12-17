
#include <fstream>
#include <string>

class FileHandler {
public:
    explicit FileHandler(const std::string path); //Ctor
    FileHandler(const FileHandler &other); //CCtor
    FileHandler(FileHandler &&other); //MCtor

    FileHandler& operator=(const FileHandler &other); //Copy Assignment
    FileHandler& operator=(FileHandler &&other) noexcept; //Move Assignment

    ~FileHandler() noexcept = default; //Dtor

private:
    std::fstream m_fd;

};