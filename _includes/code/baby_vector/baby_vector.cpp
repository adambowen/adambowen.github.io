#include "baby_vector.h"

baby_vector::baby_vector() :
    m_data(),
    m_size(),
    m_capacity()
{
}

baby_vector::~baby_vector()
{
    delete[] m_data;
    m_data = nullptr;
    m_size = m_capacity = 0;
}

int baby_vector::size() const
{
    return m_size;
}

void baby_vector::push_back(int value)
{
    if(m_size+1 > m_capacity)
    {
        m_capacity = m_size+1;

        int *tmp = new int[m_capacity];
        for(int i=0; i<m_size; ++i)
            tmp[i] = m_data[i];
        delete[] m_data;
        m_data = tmp;
    }

    m_data[m_size] = value;
    ++m_size;
}

void baby_vector::pop_back()
{
    if(m_size>0)
        --m_size;
}

int& baby_vector::operator[](int index)
{
    return m_data[index];
}

const int& baby_vector::operator[](int index) const
{
    return m_data[index];
}
