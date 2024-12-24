#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <future>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


std::vector<char> readFileContent(std::filesystem::path file_name);

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "please provide in the command line the file name" << std::endl;

        return 1;
    }
    std::filesystem::path file_name = argv[1];

    std::packaged_task<std::vector<char>(std::filesystem::path)> read_file_content_task(readFileContent);
    std::future<std::vector<char>> file_content = read_file_content_task.get_future();
    
    try {
        std::thread server_thread(std::move(read_file_content_task), file_name);
        if (server_thread.joinable()) {
            server_thread.join();
        }
    } catch (std::system_error& e) {
        throw std::runtime_error(std::string("failed creating a thread") + e.what());
    }
    
    try {
        std::vector<char> data = file_content.get();

        for (unsigned long int i = 0; i < data.size(); ++i) {
            std::cout << data[i];
        }
        std::cout << std::endl;
        
    } catch (std::system_error& e) {
        std::cerr << "syscall in thread failed: " << e.what() << std::endl;
    } catch (std::bad_alloc& e) {
        std::cerr << "alloc in thread failed: " << e.what() << std::endl;
    }

    return 0;
}

std::vector<char> readFileContent(std::filesystem::path file_name) {

    const size_t BUFFER_SIZE = 1024;
    std::vector<char> buffer(BUFFER_SIZE);
    std::vector<char> data_read;

    int fd = open(file_name.c_str(), O_RDONLY);
    if (fd == -1) {
        throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), "Open syscall failed");
    }

    int times_tried = 0;
    try {

        while (times_tried < 3) {
            int bytes_read = read(fd, buffer.data(), BUFFER_SIZE);

            if (bytes_read == 0) {
                break;
            } else if (bytes_read == -1) {
                if (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK) {
                    ++times_tried;
                } else {
                    close(fd);
                    throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), "Read syscall failed");
                }
            } else {
                data_read.insert(data_read.end(), buffer.begin(), buffer.begin() + bytes_read);
            }         
        } 
    } catch (std::bad_alloc& e) {
        close(fd);
        throw std::runtime_error(std::string("vector push_back method failed") + e.what());
    }

    close(fd);
    return data_read;
}
