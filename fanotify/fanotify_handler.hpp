#pragma once
#include <filesystem>
#include <vector>


class FaNotifyHandler {
public:
    FaNotifyHandler(std::vector<std::filesystem::path>& files);
    FaNotifyHandler(const FaNotifyHandler& other) = delete;
    FaNotifyHandler &operator=(const FaNotifyHandler& other) = delete;
    void listenForEvents();
    // ++ EventItem getTopEvent()
    // ++ void addNewReply(Reply) 

private:


    std::vector<std::filesystem::path>& m_files;
    int m_fanotify;
    // ++ std::queue<EventItem> m_events
    // ++ std::queue<Replay> m_replies

};

