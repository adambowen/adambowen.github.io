#ifndef BABY_VECTOR_H_INCLUDED
#define BABY_VECTOR_H_INCLUDED

class baby_vector
{
public:
    baby_vector();
    ~baby_vector();

    int size() const;
    void push_back(int value);
    void pop_back();
    int& operator[](int index);
    const int& operator[](int index) const;

    int *m_data;
    int m_size;
    int m_capacity;
};

#endif
