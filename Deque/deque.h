#include "base.h"
#include <vector>
#include <iterator>
const uint base_capacity = 8;

template <typename T> class Deque;


template <typename ContainerType> class container_iterator :
    public iterator<random_access_iterator_tag, ContainerType>
{
private:
    friend class Deque<ContainerType>;

    ContainerType* ptr;
    uint cur, size;

    container_iterator(ContainerType* n_ptr, uint head, uint capacity) 
        : ptr(n_ptr), cur(head), size(capacity)
    {
    }

public:

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

    ContainerType& operator *()
    {
        return *ptr;
    }

    container_iterator& operator++(int)
    {
        ptr++;
        cur++;
        if (cur >= size)
        {
            ptr -= (cur / size) * size;
            cur -= (cur / size) * size;
        }
        return *this;
    }

    container_iterator& operator++()
    {
        ptr++;
        cur++;
        if (cur >= size)
        {
            ptr -= (cur / size) * size;
            cur -= (cur / size) * size;
        }
        return *this;
    }

    container_iterator& operator--()
    {
        ptr--;
        if (cur == 0)
        {
            ptr += size;
            cur += size;
        }
        cur--;
        return *this;
    }

    container_iterator& operator--(int)
    {
        ptr--;
        if (cur == 0)
        {
            ptr += size;
            cur += size;
        }
        cur --;
        return *this;
    }

    bool operator != (const container_iterator &it)
    {
        return ptr != it.ptr;
    }

    bool operator == (const container_iterator &it) const
    {
        return ptr == it.ptr;
    }
};

template <typename T> class Deque
{
    T* buf;
    uint capacity, tail, head;

    inline uint nextHead()
    {
        return (head - 1 + capacity) % capacity;
    }
    inline uint nextTail()
    {
        return (tail + 1) % capacity;
    }
    inline uint prevHead()
    {
        return (head + 1) % capacity;
    }
    inline uint prevTail()
    {
        return (tail - 1 + capacity) % capacity;
    }

    void extendCapacity()
    {
        uint new_capacity = capacity << 1;
        T *tmp = new T[new_capacity];
        for (uint i = 0; i < capacity; i++)
            tmp[i] = this->operator[](i);
        delete buf;
        buf = tmp;    
        head = 0;
        tail = capacity;
        capacity = new_capacity;
    }
    void compressCapacity()
    {
        uint new_capacity = capacity >> 1;
        T *tmp = new T[new_capacity];
        uint size = getSize();
        for (uint i = 0; i < size; i++)
            tmp[i] = this->operator[](i);
        delete buf;
        buf = tmp;
        head = 0;
        tail = size;
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
        buf = new T[capacity];
        dbg("Deque<T>");
    }

    Deque(uint user_capacity) 
        : head(0), tail(0)
    {
        capacity = base_capacity;
        while (capacity < user_capacity)
            capacity <<= 1;
        buf = new T[capacity];
    }

    Deque(const Deque & obj) 
        : capacity(obj.capacity), head(obj.head), tail(obj.tail)
    {
        delete buf;
        buf = new T[capacity];
        for (uint i = 0; i < capacity; i++)
            buf[i] = obj.buf[i];

        dbg("Deque(const Deque<T> & obj)");
    }

    Deque(Deque && obj) 
    {
        buf = obj.buf;
        capacity = obj.capacity;
        head = obj.head;
        tail = obj.tail;

        dbg("Deque(Deque<T> && obj)");
    }

    Deque& operator = (const Deque & obj)
    {
        capacity = obj.capacity;
        head = obj.head;
        tail = obj.tail;
        delete buf;
        buf = new T[capacity];
        for (uint i = 0; i < capacity; i++)
            buf[i] = obj.buf[i];

        dbg("Deque<T>& operator = (const Deque<T> & obj)");
        return *this;
    }

    bool empty() const
    {
        return (tail == head);
    }

    int getSize() const
    {
        return (tail - head + capacity);
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
        if (getSize() == capacity / 4 && capacity != base_capacity)
            compressCapacity();
    }

    void pop_front()
    {
        head = prevHead();
        if (getSize() == capacity / 4 && capacity != base_capacity)
            compressCapacity();
    }

    const T back()
    {
        return this->operator[](getSize() - 1);
    }

    const T back() const
    {
        return this->operator[](getSize() - 1);
    }

    const T front()
    {
        return this->operator[](0);
    }

    const T front() const
    {
        return this->operator[](0);
    }

    T& operator[] (int index)
    {
        if (index < 0 || index > getSize()) 
            throw new exception();
        return buf[(head + index) % capacity];
    }

    T& operator[] (int index) const 
    {
        if (index < 0 || index > getSize()) 
            throw new exception();
        return buf[(head + index) % capacity];
    }


    iterator begin()
    {
        return iterator(buf + head, head, capacity);
    }
    iterator begin() const
    {
        return iterator(buf + head, head, capacity);
    }
    iterator end()
    {
        return iterator(buf + tail, tail, capacity);
    }
    iterator end() const
    {
        return iterator(buf + tail, tail, capacity);
    }

    const_iterator cbegin()
    {
        return const_iterator(buf + head, head, capacity);
    }
    const_iterator cbegin() const
    {
        return const_iterator(buf + head, head, capacity);
    }
    const_iterator cend()
    {
        return const_iterator(buf + tail, tail, capacity);
    }
    const_iterator cend() const
    {
        return const_iterator(buf + tail, tail, capacity);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(iterator(buf + tail, tail, capacity));
    }
    reverse_iterator rbegin() const
    {
        return reverse_iterator(iterator(buf + tail, tail, capacity));
    }
    reverse_iterator rend()
    {
        return reverse_iterator(iterator(buf + head, head, capacity));
    }
    reverse_iterator rend() const
    {
        return reverse_iterator(iterator(buf + head, head, capacity));
    }

    const_reverse_iterator crbegin()
    {
        return const_reverse_iterator(buf + prevTail(), prevTail(), capacity);
    }
    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(buf + prevTail(), prevTail(), capacity);
    }
    const_reverse_iterator crend()
    {
        return const_reverse_iterator(buf + nextHead(), nextHead(), capacity);
    }
    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(buf + nextHead(), nextHead(), capacity);
    }

};