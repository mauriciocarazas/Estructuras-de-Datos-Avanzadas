#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

template <typename Type> class CVector{
    public:
        Type*m_pVect; // Pointer to the buffer
        int m_nCount, // Control how many elements are actually used
        m_nMax, // Control the number of allocated elements
        m_nDelta; // To control the growing
        void Init(int delta); // Init our private variables, etc
        void Resize(); // Resize the vector when occurs an overflow
        public:
        CVector(int delta = 10); // Constructor
        void Insert(Type elem); // Insert a new element
};

// Class CVector implementation
template <typename Type> CVector<Type>::CVector(int delta){
    Init(delta);
}

template <typename Type> void CVector<Type>::Insert(Type elem){
    if( m_nCount == m_nMax ) // Verify the overflow
        Resize(); // Resize the vector before inserting elem
    m_pVect[m_nCount++] = elem; // Insert the element at the end
}

template <typename Type> void CVector<Type>::Resize(){
    m_pVect = (Type*)realloc(m_pVect, sizeof(Type) * (m_nMax + m_nDelta));
    m_nMax += m_nDelta; // The Max has to be increased by delta
}

template <typename Type> void CVector<Type>::Init(int delta){
    m_nDelta = delta; // Save the delta
    m_nCount = 0; // No elements are used
    m_nMax = 0; // No elements are allocated
    m_pVect = NULL; // No buffer is allocated
}

int main(){
    int nElem;
    CVector<int> vect; // Create a vector
    while( cin >> nElem ) // Read the elements
        vect.Insert(nElem); // Insert the element
    for( int i = 0; i < vect.m_nCount; i++ ){ // Print the elements
        cout << vect.m_pVect[i] << endl;
    }
    delete [ ] vect.m_pVect; // Free the memory
    

    return 0;
}