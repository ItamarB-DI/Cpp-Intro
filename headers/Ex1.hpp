template <typename T>
class DynamicBuffer {
public:
    explicit DynamicBuffer(size_t size);
    DynamicBuffer(const DynamicBuffer<T>& other);  
    DynamicBuffer<T>& operator=(const DynamicBuffer<T>& other); 
    ~DynamicBuffer() noexcept;

private:
    T* Allocate(size_t size);
    void DeAllocate() noexcept;
    void CopyDynamicBuffer(DynamicBuffer<T>& src);

    size_t m_size;
    T* m_buffer;
};

/****************************************** Main Functions *****************************************/

template <typename T>
DynamicBuffer<T>::DynamicBuffer(size_t size): m_buffer(Allocate(size)), m_size(size) {}

template <typename T>
DynamicBuffer<T>::DynamicBuffer(const DynamicBuffer<T>& other): m_buffer(Allocate(other.m_size)), m_size(other.m_size) {

    CopyDynamicBuffer(other);
}

template <typename T>
DynamicBuffer<T>& DynamicBuffer<T>::operator=(const DynamicBuffer<T>& other) {

    if (this != &other) {
        DeAllocate();
        CopyDynamicBuffer(other);
    }
    return *this;
}

template <typename T>
DynamicBuffer<T>::~DynamicBuffer() noexcept {

    DeAllocate();
}

/****************************************** Helper Functions *****************************************/

template <typename T>
T* DynamicBuffer<T>::Allocate(size_t size) {

    return new T[size];
}

template <typename T>
void DynamicBuffer<T>::DeAllocate() noexcept {

    m_size = 0;
    delete[] m_buffer;
    m_buffer = nullptr;
}

template <typename T>
void DynamicBuffer<T>::CopyDynamicBuffer(DynamicBuffer<T>& src) {
    
    m_size = src.m_size;
    for (size_t i = 0; i < m_size; ++i) {
        m_buffer[i] = src.m_buffer[i];
    }
}
