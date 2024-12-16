#include <memory> //unique_ptr

template <typename T>
class DynamicBuffer {
public:
    explicit DynamicBuffer(size_t size);
    DynamicBuffer(const DynamicBuffer<T>& other);  
    DynamicBuffer(DynamicBuffer<T> &&other) noexcept;

    DynamicBuffer<T>& operator=(const DynamicBuffer<T>& other); 
    DynamicBuffer<T>& operator=(DynamicBuffer<T>&& other) noexcept; 

    ~DynamicBuffer() noexcept = default;

private:
    void CopyDynamicBuffer(const DynamicBuffer<T>& src);

    size_t m_size;
    std::unique_ptr<T[]> m_buffer;
};

/****************************************** Main Functions *****************************************/

template <typename T> //Ctor
DynamicBuffer<T>::DynamicBuffer(size_t size): m_buffer(std::make_unique<T[]>(size)), m_size(size) {}

template <typename T> //CCtor
DynamicBuffer<T>::DynamicBuffer(const DynamicBuffer<T>& other): m_buffer(std::make_unique<T[]>(other.m_size)), m_size(other.m_size) {

    CopyDynamicBuffer(other);
}

template <typename T> //Move Ctor
DynamicBuffer<T>::DynamicBuffer(DynamicBuffer<T>&& other) noexcept : m_buffer(std::move(other.m_buffer)), m_size(other.m_size) {

    other.m_size = 0;
}


template <typename T> //Copy Assignment
DynamicBuffer<T>& DynamicBuffer<T>::operator=(const DynamicBuffer<T>& other) {

    if (this != &other) {
        m_buffer = std::make_unique<T[]>(other.m_size);
        m_size = other.m_size;

        CopyDynamicBuffer(other);
    }
    
    return *this;
}

template <typename T> //Move Assignment
DynamicBuffer<T>& DynamicBuffer<T>::operator=(DynamicBuffer<T>&& other) noexcept {

    if (this != &other) {
        m_buffer = std::move(other.m_buffer);
        m_size = other.m_size;

        other.m_size = 0;
    }

    return *this;
}

/****************************************** Helper Functions *****************************************/

template <typename T>
void DynamicBuffer<T>::CopyDynamicBuffer(const DynamicBuffer<T>& src) {
    
    m_size = src.m_size;
    for (size_t i = 0; i < m_size; ++i) {
        m_buffer[i] = src.m_buffer[i];
    }
}
