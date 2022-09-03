#include <vector> // without .h
#include <list>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
using namespace std;

template <typename Container> void Write(Container &ds, ostream &os){
    typename Container::iterator iter = ds.begin();
    for( ; iter != ds.end() ; iter++ )
    os << *iter << "\n";
}

class CMyComplexDataStructure{
    vector<float> m_container;
    public:
    void Insert(float fVal) { m_container.push_back(fVal); }    
    template <typename objclass> void sumone(objclass funobj){
        vector<float>::iterator iter = m_container.begin();
        for (; iter != m_container.end() ; iter++)
        funobj(*iter);
    }
};

template <typename objclass>class funcobjclass{
    public:
        void operator ()(objclass& objinstance) { objinstance += 1; }
};

int main(){
    CMyComplexDataStructure ds;
    ds.Insert(3.5);
    ds.Insert(4.5);
    ds.Insert(6.5);
    ds.Insert(3.9);
    funcobjclass<float> x;
    //print
    
    ds.sumone(x);
    return 0;
}
