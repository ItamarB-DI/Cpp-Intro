
#include <stdexcept> //runtime_error
#include <cassert>

#include "Ex1.hpp"

/************************************* Main Functions *******************************************/

//Ctor
FileHandler::FileHandler(const std::string path, std::ios_base::openmode permissions)
: m_permissions(permissions),
  m_path(path)
{

    m_fd.open(path, permissions);
    if (!m_fd.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }
}

//CCtor
FileHandler::FileHandler(const FileHandler &other) {

    m_fd.open(other.m_path, other.m_permissions);
    if (!m_fd.is_open()) {
        throw std::runtime_error("Failed to open file: " + other.m_path);
    }   

    m_path = other.m_path;
    m_permissions = other.m_permissions;
}

//MCtor
FileHandler::FileHandler(FileHandler &&other) noexcept 
: m_fd(std::move(other.m_fd)),
  m_path(std::move(other.m_path)),
  m_permissions(other.m_permissions)
{
    other.m_permissions = std::ios_base::openmode(0);
}

//Copy Assignment
FileHandler& FileHandler::operator=(const FileHandler &other) {

    assert(this == &other);

    if (m_fd.is_open()) {
        m_fd.close();
    }

    m_fd.open(other.m_path, other.m_permissions);
    if (!m_fd.is_open()) {
        throw std::runtime_error("Failed to open file: " + other.m_path);
    }   

    m_path = other.m_path;
    m_permissions = other.m_permissions;

    return *this;
}

//Move Assignment
FileHandler& FileHandler::operator=(FileHandler &&other) noexcept {

    assert(this == &other);

    if (m_fd.is_open()) {
        m_fd.close();
    }

    m_fd = std::move(other.m_fd);
    m_path = std::move(other.m_path);
    m_permissions = other.m_permissions;

    other.m_permissions = std::ios_base::openmode(0);

    return *this;
} 