#include <vector> 

template <typename T>
class DynamicBuffer {
public:
    explicit DynamicBuffer(size_t size);

    T& operator[](int idx);
    const T&  operator[](int idx) const;

private:

    std::vector<T> m_buffer;
};

/****************************************** Main Functions *****************************************/

template <typename T> //Ctor
DynamicBuffer<T>::DynamicBuffer(size_t size): m_buffer(size) {}

template <typename T>
T& DynamicBuffer<T>::operator[](int idx) {

    return m_buffer.at(idx);
}

template <typename T>
const T& DynamicBuffer<T>::operator[](int idx) const {

    return m_buffer.at(idx);
}
