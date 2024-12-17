#include <vector> 

template <typename T>
class DynamicBuffer {
public:
    explicit DynamicBuffer(size_t size);
    DynamicBuffer(const DynamicBuffer<T>& other);  
    DynamicBuffer(DynamicBuffer<T> &&other) noexcept;

    DynamicBuffer<T>& operator=(const DynamicBuffer<T>& other); 
    DynamicBuffer<T>& operator=(DynamicBuffer<T>&& other) noexcept; 

    T& operator[](int idx);
    const T&  operator[](int idx) const;

    ~DynamicBuffer() noexcept = default;

private:

    std::vector<T> m_buffer;
};

/****************************************** Main Functions *****************************************/

template <typename T> //Ctor
DynamicBuffer<T>::DynamicBuffer(size_t size): m_buffer(size) {}

template <typename T> //CCtor
DynamicBuffer<T>::DynamicBuffer(const DynamicBuffer<T>& other): m_buffer(other.m_buffer) {}

template <typename T> //Move Ctor
DynamicBuffer<T>::DynamicBuffer(DynamicBuffer<T>&& other) noexcept : m_buffer(std::move(other.m_buffer)) {}


template <typename T> //Copy Assignment
DynamicBuffer<T>& DynamicBuffer<T>::operator=(const DynamicBuffer<T>& other) {

    m_buffer = other.m_buffer;
    
    return *this;
}

template <typename T> //Move Assignment
DynamicBuffer<T>& DynamicBuffer<T>::operator=(DynamicBuffer<T>&& other) noexcept {

    if (this != &other) {

        m_buffer = std::move(other.m_buffer);
    }

    return *this;
}

template <typename T>
T& DynamicBuffer<T>::operator[](int idx) {

    return m_buffer[idx];
}

template <typename T>
const T& DynamicBuffer<T>::operator[](int idx) const {

    return m_buffer[idx];
}
