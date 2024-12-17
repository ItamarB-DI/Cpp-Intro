#include <memory> //unique_ptr

template <typename T>
class DynamicBuffer {
public:
    explicit DynamicBuffer(size_t size);
    DynamicBuffer(const DynamicBuffer<T>& other);  
    DynamicBuffer(DynamicBuffer<T> &&other) noexcept;

    DynamicBuffer<T>& operator=(const DynamicBuffer<T>& other); 
    DynamicBuffer<T>& operator=(DynamicBuffer<T>&& other) noexcept; 

    T& operator[](int idx);
    T operator[](int idx) const;

    ~DynamicBuffer() noexcept;

private:
    void copyDynamicBuffer(const DynamicBuffer<T>& src);
    void deAllocate() noexcept;

    size_t m_size;
    T *m_buffer;
};

/****************************************** Main Functions *****************************************/

template <typename T> //Ctor
DynamicBuffer<T>::DynamicBuffer(size_t size): m_buffer(new T[size]), m_size(size) {}

template <typename T> //CCtor
DynamicBuffer<T>::DynamicBuffer(const DynamicBuffer<T>& other): m_buffer(new T[other.m_size]), m_size(other.m_size) {

    copyDynamicBuffer(other);
}

template <typename T> //Move Ctor
DynamicBuffer<T>::DynamicBuffer(DynamicBuffer<T>&& other) noexcept : m_buffer(new T[other.m_size]), m_size(other.m_size) {

    m_buffer = other.m_buffer;

    other.m_buffer = nullptr;
    other.m_size = 0;
}


template <typename T> //Copy Assignment
DynamicBuffer<T>& DynamicBuffer<T>::operator=(const DynamicBuffer<T>& other) {

    if (this != &other) {
        deAllocate();

        m_buffer = new T[other.m_size];
        m_size = other.m_size;

        copyDynamicBuffer(other);
    }
    
    return *this;
}

template <typename T> //Move Assignment
DynamicBuffer<T>& DynamicBuffer<T>::operator=(DynamicBuffer<T>&& other) noexcept {

    if (this != &other) {

        m_buffer = other.m_buffer;
        m_size = other.m_size;

        other.m_buffer = nullptr;
        other.m_size = 0;
    }

    return *this;
}

template <typename T> //Dtor
DynamicBuffer<T>::~DynamicBuffer() noexcept {

    deAllocate();
}


template <typename T>
T& DynamicBuffer<T>::operator[](int idx) {

    if (idx < 0) {
        throw "invalid Index";
    }

    return m_buffer[idx];
}

template <typename T>
T DynamicBuffer<T>::operator[](int idx) const {

    if (idx < 0) {
        throw "invalid Index";
    }

    return m_buffer[idx];
}


/****************************************** Helper Functions *****************************************/

template <typename T>
void DynamicBuffer<T>::copyDynamicBuffer(const DynamicBuffer<T>& src) {
    
    m_size = src.m_size;
    for (size_t i = 0; i < m_size; ++i) {
        m_buffer[i] = src.m_buffer[i];
    }
}


template <typename T>
void DynamicBuffer<T>::deAllocate() noexcept {

    delete m_buffer;
    m_buffer = nullptr;
    m_size = 0;
}