#include "fanotify_handler.hpp"

#include <fcntl.h>
#include <sys/fanotify.h>
#include <unistd.h>

FaNotifyHandler::FaNotifyHandler(std::vector<std::filesystem::path>& files)
: m_files(files) {

    m_fanotify = fanotify_init(FAN_NONBLOCK | FAN_CLOEXEC | FAN_CLASS_CONTENT, O_RDONLY);
    if (m_fanotify == -1) {
        throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), "fanotify_init failed");
    }

    for (auto file_path : files) {
        if (fanotify_mark(m_fanotify, FAN_MARK_ADD, FAN_OPEN_PERM, AT_FDCWD, file_path.c_str()) == -1) {
            throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), std::string("fanotify_mark failed to mark")  + file_path.c_str());
        }
    }

}

void FaNotifyHandler::listenForEvents() {

    const size_t BUFFER_SIZE = 4096;
    std::vector<char> buffer(BUFFER_SIZE);

    int intr_fails = 0; 
    while (true) {

        int bytes_read = read(m_fanotify, buffer.data(), BUFFER_SIZE);
        if (bytes_read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            
            // ++ check the reply queue and if there is somthing, reply to it.

        } else if (bytes_read == -1) {

            if ((errno == EINTR) && intr_fails < 5) {
                ++intr_fails;
                continue;
            } else {
                throw std::system_error(std::make_error_code(static_cast<std::errc>(errno)), std::string("read syscall failed"));
            }
        }

        // ++ create a fanotify_event_metadata
        struct fanotify_event_metadata* metadata = reinterpret_cast<struct fanotify_event_metadata*>(buffer.data());
        
        while (FAN_EVENT_OK(metadata, bytes_read)) {

            // ++ EventItem x = private method to handle the event
            // ++ if (x is valid) { push to event queue}
            // ++ else ignore / throw an error. 

            metadata = FAN_EVENT_NEXT(metadata, bytes_read);
        }
    }
}

