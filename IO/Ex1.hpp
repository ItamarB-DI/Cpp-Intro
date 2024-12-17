
#include <fstream> //ios_base::openmode
#include <string>

class FileHandler {
public:
    explicit FileHandler(const std::string path, std::ios_base::openmode permissions); //Ctor
    FileHandler(const FileHandler &other); //CCtor
    FileHandler(FileHandler &&other) noexcept; //MCtor

    FileHandler& operator=(const FileHandler &other); //Copy Assignment
    FileHandler& operator=(FileHandler &&other) noexcept; //Move Assignment

    std::fstream &getFD();
    static void createFile(const std::string filename);

    ~FileHandler() noexcept = default; //Dtor

private:
    std::fstream m_fd;
    std::string m_path;
    std::ios_base::openmode m_permissions;

};