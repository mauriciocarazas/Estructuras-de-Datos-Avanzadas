#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

int *gpVect = NULL; // Dynamic buffer to save the elements int
int gnCount = 0; // Counter to know the number of used elements int
int gnMax = 0; // Variable containing the size of the allocated memory

// Class CVector definition
class CVector{
    public:
    int *m_pVect, // Pointer to the buffer
    m_nCount, // Control how many elements are actually used
    m_nMax, // Control how many are allocated as maximum
    m_nDelta; // To control the growing
    void Init(int delta); // Init our private variables, etc
    void Resize(); // Resize the vector when occurs an overflow
    public:
    CVector(int delta = 10); // Constructor
    void Insert(int elem); // Insert a new element
    // More methods go here
};

void CVector::Insert(int elem){
    if( m_nCount == m_nMax ) // Verify the overflow
    Resize(); // Resize the vector before inserting elem
    m_pVect[m_nCount++] = elem; // Insert the element at the end
}

void CVector::Resize(){
    m_pVect = (int*)realloc(m_pVect, sizeof(int) * (m_nMax + m_nDelta));
    m_nMax += m_nDelta; // The Max has to be increased by delta
}

void CVector::Init(int delta){
    m_nDelta = delta; // Save the delta
    m_nCount = 0; // No elements are used
    m_nMax = 0; // No elements are allocated
    m_pVect = NULL; // No buffer is allocated
}

CVector::CVector(int delta){
    Init(delta); // Init our private variables, etc
}

int main(){
    int nElem;
    CVector vect; // Create a vector
    while( cin >> nElem ) // Read the elements
    vect.Insert(nElem); // Insert the element
    for( int i = 0; i < vect.m_nCount; i++ ){ // Print the elements
        cout << vect.m_pVect[i] << endl;
    }
    delete [ ] vect.m_pVect; // Free the memory

    return 0;
}