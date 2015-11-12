#include "base.h"
#include "deque.h"

#include <vld.h>

Deque<int> something()
{
    Deque<int> d;
    fori(i, 20)
    {
        d.push_back(2 * i + 1);
    }

    fori(i, 10)
        d.push_front(2 * (-i) + 1);
    
    return d;
}

int main()
{
    
    Deque<int> d;
    
    d = something();
    fori (i, 30)
    {
        cout << d[i] << ' ';
    }
    cout << endl << endl;
    
    //auto q = d.cbegin();
    

    cout << "for (auto it = d.begin(); it != d.end(); it++)" << endl;
    for (auto it = d.begin(); it != d.end(); it++)
        cout << *it << ' ';
    cout << endl;

    cout << "for (auto it = d.cbegin(); it != d.cend(); it++)" << endl;
    for (auto it = d.cbegin(); it != d.cend(); it++)
        cout << *it << ' ';
    cout << endl;
    

    cout << "for (auto it = d.rbegin(); it != d.rend(); it++)" << endl;
    for (auto it = d.rbegin(); it != d.rend(); it++)
        cout << *it << ' ';
    cout << endl;

    cout << "for (auto it = d.crbegin(); it != d.crend(); it++)" << endl;
    for (auto it = d.crbegin(); it != d.crend(); it++)
        cout << *it << ' ';
    cout << endl;
    
    
    
    cout << endl << endl;
    
    fori (i, 30)
    {
        cout << d.back() << ' ';
        d.pop_back();
    }
    
    system("pause");   
    
    return 0;
}