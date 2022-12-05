#include <bits/stdc++.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <string>

using namespace std;

#define v vector<int>
#define vv vector<vector<int>>

int bits = 4; //Cantidad de bits a usar
int N = pow(2,bits); //Tamaño de la matriz

int int_to_bin(int n) {
  long long B_Number = 0;
  int cnt = 0;
  while (n != 0) {
    int rem = n % 2;
    long long c = pow(10, cnt);
    B_Number += rem * c;
    n /= 2;
    cnt++;
  }
  return B_Number;
}
int bin_to_int(int bin) {
  int exp = 0;
  int num = 0;
  while (bin > 0) {
    int aux = bin % 10;
    if (aux == 1)
      num += pow(2, exp);
    bin /= 10;
    exp++;
  }
  return num;
}

int calcular_valor2d(int x, int y) //Valor de cada celda
{
  string _x = to_string(int_to_bin(x));
  string _y = to_string(int_to_bin(y));
  //cout<<"x: "<<_x<<"  y: "<<_y<<endl;

  if(_x.size()<bits)
    _x = string(bits-_x.size(),'0') + _x;
  if(_y.size()<bits)
    _y = string(bits-_y.size(),'0') + _y;
  
  //cout<<"x: "<<_x<<"  y: "<<_y<<endl;
  string aux(bits*2,' ');
  int j = 0;
  for(int i=0; i<bits*2; i+=2)
  {
    aux[i]=_x[j];
    aux[i+1]=_y[j];
    j++;
  }    
  //cout<<aux<<endl;
  return bin_to_int(stoi(aux));
}
void llenar_mat2d(vv & mat)
{
  for(int i=0; i<mat.size(); i++)
  {
    for(int j=0; j<mat.size(); j++)
      mat[i][j] = calcular_valor2d(i,j);    
  }
}
void print_range (int x_1, int x_2, int y_1, int y_2, vv  mat) //Impresion de la matriz en un rango determinado
{
  for(int i=y_1; i!=y_2; i++)
  {
    for(int j=x_1; j!=x_2; j++)
      cout<<mat[i][j]<<" "; 
    cout<<endl;
  }
}
void print(vv mat)//Impresion de la totalidad de la matriz
{
  for(int i=0; i<mat.size(); i++)
  {
    for(int j=0; j<mat.size(); j++)
      cout<<mat[i][j]<<"\t";
    cout<<endl;
  }
  cout<<endl;
}

int main() {
  vv mat(N,v(N,0)); //Inicialización
  cout<<"Bits (N): "<<bits<<endl;
  //print(mat);
  llenar_mat2d(mat);  
  print(mat);

  print_range(0,4,0,4,mat);
}
