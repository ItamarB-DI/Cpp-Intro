
#include <stdexcept> 
#include <cassert>

#include "Ex1.hpp"


//Ctor
FileHandler::FileHandler(const std::string path, std::ios_base::openmode permissions)
: m_path(path), 
  m_permissions(permissions)
{

    m_stream.open(path, permissions);
    if (!m_stream.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }
}

//CCtor, non-const because of tellg/p which are modifying 
FileHandler::FileHandler(FileHandler &other) 
: m_path(other.m_path), 
  m_permissions(other.m_permissions) {

    m_stream.open(m_path, m_permissions);
    if (!m_stream.is_open()) {
        throw std::runtime_error("Failed to open file: " + m_path);
    }   

    auto g = other.m_stream.tellg();
    auto p = other.m_stream.tellp();
    if (g == -1 || p == -1) {
        throw std::runtime_error("Failed to do tellg/p on file: " + other.m_path);
    }
}

//MCtor
FileHandler::FileHandler(FileHandler &&other) noexcept 
: m_stream(std::move(other.m_stream)),
  m_path(std::move(other.m_path)),
  m_permissions(other.m_permissions)
{
    other.m_permissions = std::ios_base::openmode(0);
}

//Copy Assignment, non-const because of tellg/p which are modifying 
FileHandler& FileHandler::operator=(FileHandler &other) {

    assert(this != &other);

    if (m_stream.is_open()) {
        m_stream.close();
    }

    m_stream.open(other.m_path, other.m_permissions);
    if (!m_stream.is_open()) {
        throw std::runtime_error("Failed to open file: " + other.m_path);
    }   

    auto g = other.m_stream.tellg();
    auto p = other.m_stream.tellp();
    if (g == -1 || p == -1) {
        throw std::runtime_error("Failed to do tellg/p on file: " + other.m_path);
    }

    m_stream.seekg(g);
    m_stream.seekp(p);

    m_path = other.m_path;
    m_permissions = other.m_permissions;

    return *this;
}

//Move Assignment
FileHandler& FileHandler::operator=(FileHandler &&other) noexcept {

    assert(this != &other);

    if (m_stream.is_open()) {
        m_stream.close();
    }

    m_stream = std::move(other.m_stream);
    m_path = std::move(other.m_path);
    m_permissions = other.m_permissions;

    other.m_permissions = std::ios_base::openmode(0);

    return *this;
} 

std::fstream &FileHandler::getFile() {

    return m_stream;
}
