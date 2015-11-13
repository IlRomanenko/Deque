#pragma once
#include "base.h"
#include <vector>
#include <iterator>
#include <algorithm>

const uint base_capacity = 8;
template <typename T> class Deque;

template <typename IteratorType> class container_iterator :
    public iterator<random_access_iterator_tag, IteratorType>
{
private:

    IteratorType* ptr;
    int cur, size;

    void move_this(int tsize)
    {
        tsize %= size;
        ptr += tsize;
        cur += tsize;
        if (cur >= size)
        {
            cur -= size;
            ptr -= size;
        }
        if (cur < 0)
        {
            cur += size;
            ptr += size;
        }
    }

public:

    container_iterator(IteratorType* n_ptr, uint head, uint capacity)
        : ptr(n_ptr), cur(head), size(capacity)
    {
    }

    container_iterator(const container_iterator &it)
    {
        ptr = it.ptr;
        cur = it.cur;
        size = it.size;
    }

    container_iterator(const container_iterator && it)
    {
        ptr = it.ptr;
        cur = it.cur;
        size = it.size;
    }

    IteratorType& operator *()
    {
        return *ptr;
    }

    container_iterator& operator++(int)
    {
        move_this(1);
        return *this;
    }

    container_iterator& operator++()
    {
        move_this(1);
        return *this;
    }

    container_iterator& operator -- ()
    {
        move_this(-1);
        return *this;
    }

    container_iterator& operator -- (int)
    {
        move_this(-1);
        return *this;
    }

    container_iterator& operator + (int f)
    {
        move_this(f);
        return *this;
    }

    container_iterator& operator - (int f)
    {
        move_this(-f);
        return *this;
    }

    int operator - (const container_iterator& it)
    {
        return ptr - it.ptr;
    }

    container_iterator& operator += (int f)
    {
        move_this(f);
        return *this;
    }

    container_iterator& operator -= (int f)
    {
        move_this(-f);
        return *this;
    }

    IteratorType& operator [] (int f)
    {
        container_iterator new_q(*this);
        new_q += f;
        return *new_q;
    }

    bool operator != (const container_iterator &it) const
    {
        return ptr != it.ptr;
    }

    bool operator == (const container_iterator &it) const
    {
        return ptr == it.ptr;
    }

    bool operator < (const container_iterator &it) const
    {
        return cur < it.cur;
    }

    bool operator > (const container_iterator &it) const
    {
        return cur > it.cur;
    }

    bool operator >= (const container_iterator &it) const
    {
        return (*this > it || *this == it);
    }

    bool operator <= (const container_iterator &it) const
    {
        return (*this < it || *this == it);
    }

    ~container_iterator()
    {

    }
};

template <typename T> class Deque
{
    unique_ptr<T[]> buf;
    uint capacity, tail, head;

    inline uint nextHead() const
    {
        return (head - 1 + capacity) % capacity;
    }
    inline uint nextTail() const
    {
        return (tail + 1) % capacity;
    }
    inline uint prevHead() const
    {
        return (head + 1) % capacity;
    }
    inline uint prevTail() const
    {
        return (tail - 1 + capacity) % capacity;
    }

    T& getAt(int index)
    {
        return buf[(head + index) % capacity];
    }

    void extendCapacity()
    {
        uint new_capacity = capacity << 1;
        unique_ptr<T[]> tmp = unique_ptr<T[]>(new T[new_capacity]);
        for (uint i = 0; i < capacity; i++)
            tmp[i] = getAt(i);
        buf.swap(tmp);
        head = 0;
        tail = capacity;
        capacity = new_capacity;
    }
    void compressCapacity()
    {
        uint new_capacity = capacity >> 1;
        unique_ptr<T[]> tmp = unique_ptr<T[]>(new T[new_capacity]);
        uint cur_size = size();
        
        for (uint i = 0; i < cur_size; i++)
            tmp[i] = getAt(i);
        buf.swap(tmp);
        head = 0;
        tail = cur_size;
        capacity = new_capacity;
    }

public:

    typedef container_iterator<T>       iterator;
    typedef container_iterator<const T> const_iterator;

    typedef reverse_iterator<const_iterator>  const_reverse_iterator;
    typedef reverse_iterator<iterator>        reverse_iterator;

    Deque()
        : capacity(base_capacity), head(0), tail(0)
    {
        buf.reset();
        buf = unique_ptr<T[]>(new T[capacity]);
    }

    Deque(uint user_capacity)
        : head(0), tail(0)
    {
        buf.reset();
        capacity = base_capacity;
        while (capacity < user_capacity)
            capacity <<= 1;
        buf = unique_ptr<T[]>(new T[capacity]);
    }

    Deque(const Deque & obj)
        : capacity(obj.capacity), head(obj.head), tail(obj.tail)
    {
        buf.reset();
        buf = new T[capacity];
        for (uint i = 0; i < capacity; i++)
            buf[i] = obj.buf[i];
    }

    Deque(Deque && obj)
    {
        buf.reset();
        buf.swap(obj.buf);
        capacity = obj.capacity;
        head = obj.head;
        tail = obj.tail;
    }

    Deque& operator = (const Deque & obj)
    {
        capacity = obj.capacity;
        head = obj.head;
        tail = obj.tail;
        buf.reset();
        buf = unique_ptr<T[]>(new T[capacity]);
        for (uint i = 0; i < capacity; i++)
            buf[i] = obj.buf[i];
        return *this;
    }

    bool empty() const
    {
        return (tail == head);
    }

    int size() const
    {
        if (tail >= head)
            return tail - head;
        return (tail - head + capacity);
    }

    void clear()
    {
        unique_ptr<T[]> tmp = unique_ptr<T[]>(new T[base_capacity]);
        buf.swap(tmp);
        head = tail = 0;
        capacity = base_capacity;
    }

    void push_back(T obj)
    {
        buf[tail] = obj;
        tail = nextTail();
        if (tail == head)
            extendCapacity();
    }

    void push_front(T obj)
    {
        head = nextHead();
        buf[head] = obj;
        if (head == tail)
            extendCapacity();
    }

    void pop_back()
    {
        tail = prevTail();
        if (size() == capacity / 4 && capacity != base_capacity)
            compressCapacity();
    }

    void pop_front()
    {
        head = prevHead();
        if (size() == capacity / 4 && capacity != base_capacity)
            compressCapacity();
    }

    const T back()
    {
        return operator[](size() - 1);
    }

    const T back() const
    {
        return operator[](size() - 1);
    }

    const T front()
    {
        return operator[](0);
    }

    const T front() const
    {
        return operator[](0);
    }

    T& operator[] (int index)
    {
        if (index < 0 || index > size())
            throw new exception();
        return getAt(index);
    }

    T& operator[] (int index) const
    {
        if (index < 0 || index > size())
            throw new exception();
        return getAt(index);
    }

    iterator begin()
    {
        return iterator(buf.get() + head, head, capacity);
    }
    iterator begin() const
    {
        return iterator(buf.get() + head, head, capacity);
    }
    iterator end()
    {
        return iterator(buf.get() + tail, tail, capacity);
    }
    iterator end() const
    {
        return iterator(buf.get() + tail, tail, capacity);
    }

    const_iterator cbegin()
    {
        return const_iterator(buf.get() + head, head, capacity);
    }
    const_iterator cbegin() const
    {
        return const_iterator(buf.get() + head, head, capacity);
    }
    const_iterator cend()
    {
        return const_iterator(buf.get() + tail, tail, capacity);
    }
    const_iterator cend() const
    {
        return const_iterator(buf.get() + tail, tail, capacity);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(iterator(buf.get() + tail, tail, capacity));
    }
    reverse_iterator rbegin() const
    {
        return reverse_iterator(iterator(buf.get() + tail, tail, capacity));
    }
    reverse_iterator rend()
    {
        return reverse_iterator(iterator(buf.get() + head, head, capacity));
    }
    reverse_iterator rend() const
    {
        return reverse_iterator(iterator(buf.get() + head, head, capacity));
    }

    const_reverse_iterator crbegin()
    {
        return const_reverse_iterator(const_iterator(buf.get() + tail, tail, capacity));
    }
    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(const_iterator(buf.get() + tail, tail, capacity));
    }
    const_reverse_iterator crend()
    {
        return const_reverse_iterator(const_iterator(buf.get() + head, head, capacity));
    }
    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(const_iterator(buf.get() + head, head, capacity));
    }

    ~Deque()
    {
    }
};