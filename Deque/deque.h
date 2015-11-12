#include "base.h"
#include <vector>
#include <iterator>
const uint base_capacity = 8;

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

    template <typename ContainerType> class forward_container_iterator : public iterator<random_access_iterator_tag, ContainerType>
    {
    private:
        friend class Deque<T>;
       
        ContainerType* ptr;
        uint cur, size;

        forward_container_iterator(ContainerType* n_ptr) : ptr(n_ptr), cur(head), size(capacity) { }

    public:

        forward_container_iterator(const forward_container_iterator &it)
        {
            ptr = it.ptr;
            cur = it.head;
            size = it.deque_size;
        }

        ContainerType operator *()
        {
            return *ptr;
        }

        forward_container_iterator& operator++(int f)
        {
            ptr += (f + 1);
            cur += (f + 1);
            if (cur >= size)
            {
                ptr -= (cur / size) * size;
                cur -= (cur / size) * size;
            }
            return *this;
        }

        forward_container_iterator& operator++()
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

        bool operator != (forward_container_iterator &it)
        {
            return ptr != it.ptr;
        }
    };
    
    /*template <typename ContainerType> class reverse_container_iterator : public iterator<forward_iterator_tag, ContainerType>
    {
    private:
        friend class Deque<T>;
       
        ContainerType* ptr;
        uint cur, size;

        reverse_container_iterator(ContainerType* n_ptr) : ptr(n_ptr), cur(0), size(0) { }

    public:

        reverse_container_iterator(const reverse_container_iterator &it, uint head, uint deque_size)
        {
            ptr = it.ptr;
            cur = head;
            size = deque_size;
        }

        ContainerType operator *()
        {
            return *ptr;
        }

        reverse_container_iterator& operator++(int f)
        {
            ptr -= (f + 1);
            if (cur == 0)
            {
                ptr += size;
                cur = size;
            }
            cur--;
            return *this;
        }

        reverse_container_iterator& operator++()
        {
            ptr--;
            if (cur == 0)
            {
                ptr += size;
                cur = size;
            }
            cur--;
            return *this;
        }

        bool operator != (reverse_container_iterator &it)
        {
            return ptr != it.ptr;
        }
    };*/
    

    typedef forward_container_iterator<T>       deque_iterator;
    typedef forward_container_iterator<const T> const_deque_iterator;

    typedef reverse_iterator<forward_container_iterator<const T> >  reverse_deque_iterator;
    typedef reverse_iterator<forward_container_iterator<const T> >  const_reverse_deque_iterator;


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


    deque_iterator begin()
    {
        return deque_iterator(buf + head, head, capacity);
    }
    deque_iterator begin() const
    {
        return deque_iterator(buf + head, head, capacity);
    }
    deque_iterator end()
    {
        return deque_iterator(buf + tail, tail, capacity);
    }
    deque_iterator end() const
    {
        return deque_iterator(buf + tail, tail, capacity);
    }

    const_deque_iterator cbegin()
    {
        return const_deque_iterator(buf + head, head, capacity);
    }
    const_deque_iterator cbegin() const
    {
        return const_deque_iterator(buf + head, head, capacity);
    }
    const_deque_iterator cend()
    {
        return const_deque_iterator(buf + tail, tail, capacity);
    }
    const_deque_iterator cend() const
    {
        return const_deque_iterator(buf + tail, tail, capacity);
    }

    reverse_deque_iterator rbegin()
    {
        return reverse_deque_iterator(buf + prevTail(), prevTail(), capacity);
    }
    reverse_deque_iterator rbegin() const
    {
        return reverse_deque_iterator(buf + prevTail(), prevTail(), capacity);
    }
    reverse_deque_iterator rend()
    {
        return reverse_deque_iterator(buf + nextHead(), nextHead(), capacity);
    }
    reverse_deque_iterator rend() const
    {
        return reverse_deque_iterator(buf + nextHead(), nextHead(), capacity);
    }

    const_reverse_deque_iterator crbegin()
    {
        return const_reverse_deque_iterator(buf + prevTail(), prevTail(), capacity);
    }
    const_reverse_deque_iterator crbegin() const
    {
        return const_reverse_deque_iterator(buf + prevTail(), prevTail(), capacity);
    }
    const_reverse_deque_iterator crend()
    {
        return const_reverse_deque_iterator(buf + nextHead(), nextHead(), capacity);
    }
    const_reverse_deque_iterator crend() const
    {
        return const_reverse_deque_iterator(buf + nextHead(), nextHead(), capacity);
    }

};