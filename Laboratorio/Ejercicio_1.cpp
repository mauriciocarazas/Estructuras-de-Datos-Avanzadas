//calculate distance between 2 point(x,y,z)
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

float distance(int x1, int y1, int z1, int x2, int y2,int z2){
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z1-z2,2) * 1.0);
}


int main(){
  int x,y,z;
  bool xsign, ysign, zsign;
  int arrX[5];
	int arrY[5];
  int arrZ[5];
  
  for(int i=0; i<5; i++){
		x = rand()%100;
		y = rand()%100;
    z = rand()%100;
		xsign = rand()%2;
		ysign = rand()%2;
    zsign = rand()%2;
		
		if(xsign) x *= -1;
		if(ysign) y *= -1;
    if(zsign) z *= -1;
		
		arrX[i] = x;
		arrY[i] = y;
    arrZ[i] = z;
	}  
  for (int i=0;i<5; i++) 	
    cout << arrX[i] << ", " << arrY[i] << endl;

}
