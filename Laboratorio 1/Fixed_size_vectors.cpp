#include <iostream>
using namespace std;

int gVect[100]; // Buffer to save the elements
int gnCount; // Counter to know the number of elements use


void Insert(int elem){
    if( gnCount < 100 ) // we can only insert if there is space
    gVect[gnCount++] = elem; // Insert the element at the end
}



int main(){
    int nElem;
    gnCount = 0; // Initialize the counter
    while( cin >> nElem ) // Read the elements
    Insert(nElem); // Insert the element
    for( int i = 0; i < gnCount; i++ ){ // Print the elements
        cout << gVect[i] << endl;
    }
    
    
    return 0;
}