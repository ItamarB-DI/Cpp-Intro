#include <iostream>
#include <filesystem>
#include <future>
#include <string>
#include <vector>
#include <fstream> 


void clientJob(std::promise<std::filesystem::path> file_name, std::future<std::vector<char>> what_the_server_read);
void serverJob(std::future<std::filesystem::path> client_file_name, std::promise<std::vector<char>> promise_file_data);

void readFileName(std::promise<std::filesystem::path>& file_name);
void readFileContent(std::promise<std::vector<char>>& promise_file_data, std::filesystem::path& file_name);

int main() {
    
    // create packed_task for the file name (the clientJob would get the promise & serverJob would get the future)
    // create packed_task for the file content (the clientJob would get the future & serverJob would get the promise) 

    std::promise<std::filesystem::path> promise_file_name;
    std::future<std::filesystem::path> future_file_name = promise_file_name.get_future();


    std::promise<std::vector<char>> promise_file_content;
    std::future<std::vector<char>> future_file_content = promise_file_content.get_future();

    // create thread with client job
    // create thread with server job

    std::thread client_thread(clientJob, std::move(promise_file_name), std::move(future_file_content));
    std::thread server_thread(serverJob, std::move(future_file_name), std::move(promise_file_content));


    // join client
    // join server

    client_thread.join();
    server_thread.join();

    return 0;
}


void clientJob(std::promise<std::filesystem::path> file_name, std::future<std::vector<char>> what_the_server_read) {

    readFileName(file_name);

    std::vector<char> file_content = what_the_server_read.get();
    std::cout << std::endl;
    for (long unsigned int i = 0; i < file_content.size(); ++i) {
        std::cout << file_content[i];
    }
    std::cout << std::endl;
}

void serverJob(std::future<std::filesystem::path> client_file_name, std::promise<std::vector<char>> promise_file_data) {

    auto file_name = client_file_name.get();

    readFileContent(promise_file_data, file_name);
}

void readFileName(std::promise<std::filesystem::path>& file_name) {

    std::filesystem::path path;

    try {
        std::cout << "Please Enter File Name: " << std::endl;
        std::cin >> path;
        
        file_name.set_value(path);
    } catch (std::ios_base::failure &e) {
        auto eptr = std::current_exception();
        file_name.set_exception(eptr);
    }

}

void readFileContent(std::promise<std::vector<char>>& promise_file_data, std::filesystem::path& file_name) {

    // ++handle exceptions 

    try {
        std::fstream file(file_name, std::ios::in);
        std::istreambuf_iterator<char> it(file), eof;
        std::vector<char> data;

        while (it != eof) {
            data.push_back(*it);
            ++it;
        }

        promise_file_data.set_value(data);
    } catch (std::exception& e) {
        auto eptr = std::current_exception();
        promise_file_data.set_exception(eptr);
    }

}


