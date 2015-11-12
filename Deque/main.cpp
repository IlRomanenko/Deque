#include "base.h"
#include "deque.h"

template <typename T> class Base
{
public:
    T *q;
    Base()
    {
        q = new T [5];
        fori(i, 5)
            q[i] = rand();
        dbg("Base()");
    }
    Base(const Base& obj)
    {
        q = obj.q;
        dbg("Base (const Base& obj)");
    }

    Base(Base && obj)
    {
        q = obj.q;
        dbg("Base(Base && obj)");
    }
};

Base<int> something()
{
    Base<int> object;
    object.q[0] = -1;
    object.q[1] = 1;
    return object;
}

int main()
{
    Deque<int> d;
    
    fori(i, 20)
    {
        d.push_back(2 * i + 1);
    }

    fori(i, 10)
        d.push_front(2 * (-i) + 1);

    fori (i, 30)
    {
        cout << d[i] << ' ';
    }
    cout << endl << endl;
    
    //auto q = d.cbegin();
    for (auto it = d.cbegin(); it != d.cend(); it++)
        cout << *it << ' ';
    
    cout << endl << endl;
    
    fori (i, 30)
    {
        cout << d.back() << ' ';
        d.pop_back();
    }
    


    system("pause");

    return 0;
}