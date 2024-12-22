#include <iostream>
#include <filesystem>
#include <future>
#include <string>


void clientJob(std::promise<std::string> file_name, std::future<std::string> what_the_server_read);
void serverJob(std::future<std::string> client_file_name, std::promise<std::string> promise_file_data);

void readFileName(std::promise<std::string> file_name);
void readFromFile(std::promise<std::string> promise_file_data);

int main() {
    
    // create packed_task for the file name (the clientJob would get the promise & serverJob would get the future)
    // create packed_task for the file content (the clientJob would get the future & serverJob would get the promise) 

    std::promise<std::string> promise_file_name;
    std::future<std::string> future_file_name = promise_file_name.get_future();


    std::promise<std::string> promise_file_content;
    std::future<std::string> future_file_content = promise_file_content.get_future();

    // create thread with client job
    // create thread with server job

    std::thread client_thread(clientJob, std::move(promise_file_name), std::move(future_file_content));
    std::thread server_thread(serverJob, std::move(future_file_name), std::move(promise_file_content));


    // join client
    // join server

    client_thread.join();


    return 0;
}


void clientJob(std::promise<std::string> file_name, std::future<std::string> what_the_server_read) {
    (void)file_name;
    (void)what_the_server_read;

    // read file name from the cli and fill the promise 

    // read the buffer from the promise 
    // print the buffer content 

}

void serverJob(std::future<std::string> client_file_name, std::promise<std::string> promise_file_data) {
    (void)client_file_name;
    (void)promise_file_data;
    // read the file name from the promise 

    // read the content of the file and put it in the future (inside a buffer)

}


