
#include <stdexcept> 
#include <cassert>
#include <cstring>
#include <algorithm>
#include <filesystem>
#include <unistd.h>

#include "Ex1.hpp"


//Ctor
FileHandler::FileHandler(const std::string path, std::ios_base::openmode permissions)
: m_path(path), 
  m_permissions(permissions),
  m_read_pos(std::streampos(0)) {

    openFile();
}

//CCtor, non-const because of tellg/p which are modifying 
FileHandler::FileHandler(FileHandler &other) 
: m_path(other.m_path), 
  m_permissions(other.m_permissions),
  m_read_pos(other.m_read_pos) {

    openFile(); 

    auto g = other.m_stream.tellg();
    auto p = other.m_stream.tellp();

    m_stream.seekg(g);
    m_stream.seekp(p);
}

//MCtor
FileHandler::FileHandler(FileHandler &&other) noexcept 
: m_stream(std::move(other.m_stream)),
  m_path(std::move(other.m_path)),
  m_permissions(other.m_permissions),
  m_read_pos(std::move(other.m_read_pos))
{
    other.m_path = "";
    other.m_permissions = std::ios_base::openmode(0);
    other.m_read_pos = std::streampos(0);
}

//Copy Assignment, non-const because of tellg/p which are modifying 
FileHandler& FileHandler::operator=(FileHandler &other) {

    assert(this != &other);

    this->closeFile();

    m_path = other.m_path;
    m_permissions = other.m_permissions;
    m_read_pos = other.m_read_pos;

    this->openFile();

    auto g = other.m_stream.tellg();
    auto p = other.m_stream.tellp();

    m_stream.seekg(g);
    m_stream.seekp(p);

    return *this;
}

//Move Assignment
FileHandler& FileHandler::operator=(FileHandler &&other) noexcept {

    assert(this != &other);

   this->closeFile();

    m_stream = std::move(other.m_stream);
    m_path = std::move(other.m_path);
    m_permissions = other.m_permissions;
    m_read_pos = other.m_read_pos;

    other.m_path = "";
    other.m_permissions = std::ios_base::openmode(0);
    other.m_read_pos = std::streampos(0);

    return *this;
} 

void FileHandler::reOpen(std::ios_base::openmode permissions) {

    closeFile();

    m_permissions = permissions;

    openFile();
}

void FileHandler::write(const std::vector<char> data) {

    if (!m_stream.is_open()) {
        throw std::runtime_error("File is not open");
    }

    if (!(m_permissions & std::ios::out)) {
        throw std::runtime_error("File doesn't have permission to write");
    }

    try {
        m_stream.write(data.data(), data.size());
    } catch (const std::exception &e) {
        m_stream.clear();
        throw std::runtime_error(e.what());
    }

}

std::vector<char> FileHandler::read(size_t size_to_read) {

    if (!m_stream.is_open()) {
        throw std::runtime_error("File is not open");
    }

    if (!(m_permissions & std::ios::in)) {
        throw std::runtime_error("File doesn't have permission to read");
    }

    size_to_read = std::min(size_to_read, static_cast<size_t>(sizeUntilEOF()));

    std::vector<char> read_buff(size_to_read, 0);
    m_stream.seekg(m_read_pos);

    std::istreambuf_iterator<char> it(m_stream);

    try {
        std::copy_n(it, size_to_read, read_buff.begin());
    }
    catch(const std::exception &e) {
        throw std::runtime_error(e.what());
    }

    m_read_pos = static_cast<std::streamoff>(m_read_pos) + size_to_read;

    return read_buff;
}

void FileHandler::seekg(std::streampos pos) {

    if (!m_stream.is_open()) {
        throw std::runtime_error("File is not open");
    }

    m_stream.seekg(pos);

    auto new_pos = m_stream.tellg();

    m_read_pos = new_pos;
}

void FileHandler::seekg(std::streamoff off, std::ios_base::seekdir way) {

    if (!m_stream.is_open()) {
        throw std::runtime_error("File is not open");
    }

    m_stream.seekg(off, way);

    auto new_pos = m_stream.tellg();

    m_read_pos = new_pos;
}

std::uintmax_t FileHandler::sizeUntilEOF() {

    if (!m_stream.is_open()) { 
        throw std::runtime_error("File is not open"); 
    }

    m_stream.seekg(0, std::ios::end);             

    auto eof_idx = m_stream.tellg();

    m_stream.seekg(m_read_pos);     

    return eof_idx - m_read_pos;
}

std::uintmax_t FileHandler::size() {

    if (!m_stream.is_open()) { 
        throw std::runtime_error("File is not open"); 
    }

    m_stream.seekg(0, std::ios::end);             

    auto eof_idx = m_stream.tellg();

    m_stream.seekg(m_read_pos); 

    return eof_idx - std::streampos(0);
}


FileHandler::~FileHandler() noexcept {

   closeFile();
}

void FileHandler::openFile() {

    if (!std::filesystem::exists(m_path)) {
        throw std::runtime_error("File does not exist: " + m_path);
    }

    //auto file_prems = std::filesystem::status(m_path.c_str()).permissions();
    int amode = 0;
    if (m_permissions & std::ios::in) {
        amode |= R_OK;
    }
    if (m_permissions & std::ios::out) {
        amode |= W_OK;
    }
    if (access(m_path.c_str(), amode) == -1) {
        throw std::runtime_error("File doesn't have the right permissions");
    }

    
    m_stream.exceptions(std::ios::failbit | std::ios::badbit);
    
    m_stream.open(m_path, m_permissions);
    if (!m_stream.is_open()) {
        throw std::runtime_error("Failed to open file: " + m_path);
    }
}

void FileHandler::closeFile() {

    if (m_stream.is_open())
    {
        m_stream.flush();
        m_stream.close();
    }
}