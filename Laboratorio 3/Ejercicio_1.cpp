//calculate distance between 2 point(x,y,z)
#include <bits/stdc++.h>
#include <ctime>
using namespace std;

float distance(int x1, int y1, int z1, int x2, int y2,int z2){
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z1-z2,2) * 1.0);
}


int main(){
  clock_t start;
  clock_t end;
  double timetaken_1000;
  
  double timetaken_10000;
  
  double timetaken_50000;
  
  int x,y,z;
  bool xsign, ysign, zsign;
  int n_1=1000;
  int n_2=10000;
  int n_3=50000;

  
  int arrX_1[n_1];
	int arrY_1[n_1];
  int arrZ_1[n_1];
  
  for(int i=0; i<n_1; i++){
		x = rand()%100;
		y = rand()%100;
    z = rand()%100;
		xsign = rand()%2;
		ysign = rand()%2;
    zsign = rand()%2;
		
		if(xsign) x *= -1;
		if(ysign) y *= -1;
    if(zsign) z *= -1;
		
		arrX_1[i] = x;
		arrY_1[i] = y;
    arrZ_1[i] = z;
	}  
  int arrX_2[n_2];
	int arrY_2[n_2];
  int arrZ_2[n_2];
  
  for(int i=0; i<n_2; i++){
		x = rand()%100;
		y = rand()%100;
    z = rand()%100;
		xsign = rand()%2;
		ysign = rand()%2;
    zsign = rand()%2;
		
		if(xsign) x *= -1;
		if(ysign) y *= -1;
    if(zsign) z *= -1;
		
		arrX_2[i] = x;
		arrY_2[i] = y;
    arrZ_2[i] = z;
	}  
  
  int arrX_3[n_3];
	int arrY_3[n_3];
  int arrZ_3[n_3];
  
  for(int i=0; i<n_3; i++){
		x = rand()%100;
		y = rand()%100;
    z = rand()%100;
		xsign = rand()%2;
		ysign = rand()%2;
    zsign = rand()%2;
		
		if(xsign) x *= -1;
		if(ysign) y *= -1;
    if(zsign) z *= -1;
		
		arrX_3[i] = x;
		arrY_3[i] = y;
    arrZ_3[i] = z;
	}

  
  start = clock();
  for (int i=1;i<=n_1; i++) {
    distance(arrX_1[0], arrY_1[0], arrZ_1[0], arrX_1[i], arrY_1[i], arrZ_1[i]);
  }
  end = clock();
  timetaken_1000 = (end - start) / (double)CLOCKS_PER_SEC;
  cout << "Time taken by 1000: " << fixed << timetaken_1000 << "s" << endl;


  
  start = clock();
  for (int i=1;i<=n_2; i++) {
    distance(arrX_2[0], arrY_2[0], arrZ_2[0], arrX_2[i], arrY_2[i], arrZ_2[i]);
  }
  end = clock();
  timetaken_10000 = (end - start) / (double)CLOCKS_PER_SEC;
  cout << "Time taken by 10000: " << fixed << timetaken_10000 << "s" << endl;



  
  start = clock();
  for (int i=1;i<=n_3; i++) {
    distance(arrX_3[0], arrY_3[0], arrZ_3[0], arrX_3[i], arrY_3[i], arrZ_3[i]);
  }
  end = clock();
  timetaken_50000 = (end - start) / (double)CLOCKS_PER_SEC;
  cout << "Time taken by 50000: " << fixed << timetaken_50000 << "s" << endl;
  
}
