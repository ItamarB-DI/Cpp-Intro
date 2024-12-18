
#include <fstream> 
#include <string>

class FileHandler {
public:
    explicit FileHandler(const std::string path, std::ios_base::openmode permissions); //Ctor
    FileHandler(FileHandler &other); //CCtor non-const because that tellg/p are modifying 
    FileHandler(FileHandler &&other) noexcept; //MCtor

    FileHandler& operator=(FileHandler &other); //Copy Assignment non-const because that tellg/p are modifying
    FileHandler& operator=(FileHandler &&other) noexcept; //Move Assignment

    std::fstream &getFile();

    ~FileHandler() noexcept = default; //Dtor

private:
    std::fstream m_stream;
    std::string m_path;
    std::ios_base::openmode m_permissions;

};