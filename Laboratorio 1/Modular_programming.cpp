#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

int *gpVect = NULL; // Dynamic buffer to save the elements int
int gnCount = 0; // Counter to know the number of used elements int
int gnMax = 0; // Variable containing the size of the allocated memory

void Resize(int *& rpVect, int& rnMax){
    const int delta = 10; // Used to increase the vector size
    // You can also use the following code:
    int *pTemp, i;
    pTemp = new int[rnMax + delta]; // Alloc a new vector
    for(i = 0 ; i < rnMax ; i++) // Transfer the elements
    pTemp[i] = rpVect[i]; // we can also use the function memcpy
    delete [ ] rpVect; // delete the old vector
    rpVect = pTemp; // Update the pointer
    rnMax += delta; // The Max has to be increased by delta
}

void Insert(int *& rpVect, int& rnCount, int& rnMax, int elem){
    if( rnCount == rnMax ) // Verify the overflow
    Resize(rpVect, rnMax); // Resize the vector before inserting elem
    rpVect[rnCount++] = elem; // Insert the element at the end of the sequence
}

struct Vector{
    int*m_pVect, // Pointer to the buffer
    m_nCount, // Control how many elements are actually used
    m_nMax, // Control how many are allocated as maximum
    m_nDelta; // To control the growing
};

int main(){
    int nElem;
    while( cin >> nElem ) // Read the elements
    Insert(gpVect, gnCount, gnMax, nElem); // Insert the element
    for( int i = 0; i < gnCount; i++ ){ // Print the elements
        cout << gpVect[i] << endl;
    }
    delete [ ] gpVect; // Free the memory

    

    return 0;
}