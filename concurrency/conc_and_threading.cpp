#include <iostream>
#include <filesystem>
#include <future>
#include <string>
#include <vector>
#include <fstream> 


void clientJob(std::exception_ptr& ex_ptr, std::promise<std::filesystem::path> file_name, std::future<std::vector<char>> what_the_server_read);
void serverJob(std::exception_ptr& ex_ptr, std::future<std::filesystem::path> client_file_name, std::promise<std::vector<char>> promise_file_data);

void readFileName(std::promise<std::filesystem::path>& file_name);
void readFileContent(std::promise<std::vector<char>>& promise_file_data, std::filesystem::path& file_name);

int main() {
    
    std::promise<std::filesystem::path> promise_file_name;
    std::future<std::filesystem::path> future_file_name = promise_file_name.get_future();


    std::promise<std::vector<char>> promise_file_content;
    std::future<std::vector<char>> future_file_content = promise_file_content.get_future();

    std::exception_ptr client_thread_ex_ptr;
    std::exception_ptr server_thread_ex_ptr;

    try {
        std::thread client_thread(clientJob, std::ref(client_thread_ex_ptr), std::move(promise_file_name), std::move(future_file_content));
        std::thread server_thread(serverJob, std::ref(server_thread_ex_ptr), std::move(future_file_name), std::move(promise_file_content));

        if (client_thread.joinable()) {
        client_thread.join();
        }
        if (server_thread.joinable()) {
            server_thread.join();
        }
    } catch (std::system_error& e) {
        std::cerr << "failed to create a thread: " << e.what() << std::endl;
    }

    if (client_thread_ex_ptr != nullptr) {
        try {
            std::rethrow_exception(client_thread_ex_ptr);
        } catch (std::exception& e) {
            std::cerr << "client thread throw an exception: " << e.what() << std::endl;
        }
    }

    if (server_thread_ex_ptr != nullptr) {
        try {
            std::rethrow_exception(server_thread_ex_ptr);
        } catch (std::exception& e) {
            std::cerr << "server thread throw an exception: " << e.what() << std::endl;
        }
    }

    return 0;
}


void clientJob(std::exception_ptr& ex_ptr, std::promise<std::filesystem::path> file_name, std::future<std::vector<char>> what_the_server_read) {

    readFileName(file_name);

    try {
        std::vector<char> file_content = what_the_server_read.get();
        std::cout << std::endl;
        for (long unsigned int i = 0; i < file_content.size(); ++i) {
            std::cout << file_content[i];
        }
        std::cout << std::endl;
    } catch (...) {

        ex_ptr = std::current_exception();
    }
    
}

void serverJob(std::exception_ptr& ex_ptr, std::future<std::filesystem::path> client_file_name, std::promise<std::vector<char>> promise_file_data) {

    std::filesystem::path file_name;

    try {

        file_name = client_file_name.get();
        readFileContent(promise_file_data, file_name);
    } catch (...) {
        ex_ptr = std::current_exception();
    }

    
}

void readFileName(std::promise<std::filesystem::path>& file_name) {

    std::filesystem::path path;

    try {
        std::cout << "Please Enter File Name: " << std::endl;
        std::cin >> path;
        
        //throw std::runtime_error("Test, client puts an error in the server promise, the server thread should throw"); // for testing

        file_name.set_value(path);
    } catch (...) {
        
        try {
            file_name.set_exception(std::current_exception());
        } catch (std::future_error&) {
            throw std::runtime_error("Failed to set exception in promise");
        }
    }

}

void readFileContent(std::promise<std::vector<char>>& promise_file_data, std::filesystem::path& file_name) {

    try {
        std::fstream file(file_name, std::ios::in);
        std::istreambuf_iterator<char> it(file), eof;
        std::vector<char> data;

        while (it != eof) {
            data.push_back(*it);
            ++it;
        }

        //throw std::runtime_error("Test, server puts an error in the client promise, the client thread should throw"); // for testing 

        promise_file_data.set_value(data);
    }  catch (...) {
        
        try {
            promise_file_data.set_exception(std::current_exception());
        } catch (std::future_error&) {
            throw std::runtime_error("Failed to set exception in promise");
        }
    }

}


