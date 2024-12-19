
#include <stdexcept> 
#include <cassert>

#include "Ex1.hpp"


//Ctor
FileHandler::FileHandler(const std::string path, std::ios_base::openmode permissions)
: m_path(path), 
  m_permissions(permissions),
  m_read_pos(std::streampos(0)) {

    open();
}

//CCtor, non-const because of tellg/p which are modifying 
FileHandler::FileHandler(FileHandler &other) 
: m_path(other.m_path), 
  m_permissions(other.m_permissions),
  m_read_pos(other.m_read_pos) {

    open(); 

    auto g = other.m_stream.tellg();
    auto p = other.m_stream.tellp();
    if (g == -1 || p == -1) {
        throw std::runtime_error("CCtor Failed to do tellg/p on file: " + m_path);
    }

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

    this->close();

    m_path = other.m_path;
    m_permissions = other.m_permissions;
    m_read_pos = other.m_read_pos;

    this->open();

    auto g = other.m_stream.tellg();
    auto p = other.m_stream.tellp();
    if (g == -1 || p == -1) {
        throw std::runtime_error("Failed to do tellg/p on file: " + other.m_path);
    }

    m_stream.seekg(g);
    m_stream.seekp(p);

    return *this;
}

//Move Assignment
FileHandler& FileHandler::operator=(FileHandler &&other) noexcept {

    assert(this != &other);

   this->close();

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

    close();

    m_permissions = permissions;

    open();
}

void FileHandler::write(const std::vector<char> data) {

    m_stream.write(data.data(), data.size());
}

std::vector<char> FileHandler::read(size_t size_to_read) {
    
    std::vector<char> read_buff(size_to_read, 0);

    m_stream.seekg(m_read_pos);

    if (!m_stream.read(read_buff.data(), size_to_read)) {
        m_stream.clear();

        throw std::runtime_error("Read Failed");
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
    if (new_pos == std::streampos(-1)) {
        throw std::runtime_error("Failed to get the current position after seeking");
    }

    m_read_pos = new_pos;
}

void FileHandler::seekg(std::streamoff off, std::ios_base::seekdir way) {
    if (!m_stream.is_open()) {
        throw std::runtime_error("File is not open");
    }

    m_stream.seekg(off, way);

    auto new_pos = m_stream.tellg();
    if (new_pos == std::streampos(-1)) {
        throw std::runtime_error("Failed to get the current position after seeking");
    }

    m_read_pos = new_pos;
}

std::uintmax_t FileHandler::sizeUntillEOF() {

    if (!m_stream.is_open()) { throw std::runtime_error("File is closed"); }

    m_stream.seekg(0, std::ios::end);             

    auto eof_idx = m_stream.tellg();
    if (eof_idx == -1) {
        throw std::runtime_error("Tell Failed");
    } 

    m_stream.seekg(m_read_pos);     

    return eof_idx - m_read_pos;
}

std::uintmax_t FileHandler::size() {

    if (!m_stream.is_open()) { throw std::runtime_error("File is closed"); }

    m_stream.seekg(0, std::ios::beg);             

    auto beg_idx = m_stream.tellg();
    if (beg_idx == -1) {
        throw std::runtime_error("Tell Failed");
    } 

    m_stream.seekg(0, std::ios::end);             

    auto eof_idx = m_stream.tellg();
    if (eof_idx == -1) {
        throw std::runtime_error("Tell Failed");
    } 

    m_stream.seekg(m_read_pos); 

    return eof_idx - beg_idx;
}


FileHandler::~FileHandler() noexcept {

   close();
}


void FileHandler::open() {

    m_stream.open(m_path, m_permissions);
    if (!m_stream.is_open()) {
        throw std::runtime_error("Failed to open file: " + m_path);
    }

    m_stream.exceptions(std::ios::failbit | std::ios::badbit);
}

void FileHandler::close() {

    if (m_stream.is_open())
    {
        m_stream.flush();
        m_stream.close();
    }
}