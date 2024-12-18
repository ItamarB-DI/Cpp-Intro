
#include <stdexcept> //runtime_error
#include <cassert>

#include "Ex1.hpp"

/************************************* Main Functions *******************************************/

//Ctor
FileHandler::FileHandler(const std::string path, std::ios_base::openmode permissions)
: m_path(path), 
  m_permissions(permissions)
{

    m_file.open(path, permissions);
    if (!m_file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }
}

//CCtor
FileHandler::FileHandler(FileHandler &other) 
: m_path(other.m_path), 
  m_permissions(other.m_permissions) {

    m_file.open(m_path, m_permissions);
    if (!m_file.is_open()) {
        throw std::runtime_error("Failed to open file: " + m_path);
    }   

    auto g = other.m_file.tellg();
    auto p = other.m_file.tellp();
    if (g == -1 || p == -1) {
        throw std::runtime_error("Failed to do tellg/p on file: " + other.m_path);
    }
}

//MCtor
FileHandler::FileHandler(FileHandler &&other) noexcept 
: m_file(std::move(other.m_file)),
  m_path(std::move(other.m_path)),
  m_permissions(other.m_permissions)
{
    other.m_permissions = std::ios_base::openmode(0);
}

//Copy Assignment
FileHandler& FileHandler::operator=(FileHandler &other) {

    assert(this != &other);

    if (m_file.is_open()) {
        m_file.close();
    }

    m_file.open(other.m_path, other.m_permissions);
    if (!m_file.is_open()) {
        throw std::runtime_error("Failed to open file: " + other.m_path);
    }   

    auto g = other.m_file.tellg();
    auto p = other.m_file.tellp();
    if (g == -1 || p == -1) {
        throw std::runtime_error("Failed to do tellg/p on file: " + other.m_path);
    }

    m_file.seekg(g);
    m_file.seekp(p);

    m_path = other.m_path;
    m_permissions = other.m_permissions;

    return *this;
}

//Move Assignment
FileHandler& FileHandler::operator=(FileHandler &&other) noexcept {

    assert(this != &other);

    if (m_file.is_open()) {
        m_file.close();
    }

    m_file = std::move(other.m_file);
    m_path = std::move(other.m_path);
    m_permissions = other.m_permissions;

    other.m_permissions = std::ios_base::openmode(0);

    return *this;
} 

std::fstream &FileHandler::getFile() {

    return m_file;
}

void FileHandler::createFile(const std::string filename) {

    std::ofstream file(filename, std::ios::out);

    if (!file) {
        throw std::runtime_error("Failed to create file: " + filename);
    }

    file.close();
}