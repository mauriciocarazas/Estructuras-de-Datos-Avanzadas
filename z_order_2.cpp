#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>

using namespace std;
//uint32_t para garantizar una representacion de 32 bits
uint32_t calcZOrder(uint16_t xPos, uint16_t yPos)
{
    static const uint32_t MASKS[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF}; // MASKS 
    static const uint32_t SHIFTS[] = {1, 2, 4, 8}; //shifts are 1,2,4,8

    uint32_t x = xPos;  // Intercala los 16 bits inferiores de x e y de modo que los bits 
    uint32_t y = yPos;  // de x estén en las posiciones pares y los bits de y en las impares;

    x = (x | (x << SHIFTS[3])) & MASKS[3]; //  0x55555555 = 01010101010101010101010101010101
    x = (x | (x << SHIFTS[2])) & MASKS[2]; // 0x33333333 = 00110011001100110011001100110011
    x = (x | (x << SHIFTS[1])) & MASKS[1]; // 0x0F0F0F0F = 00001111000011110000111100001111
    x = (x | (x << SHIFTS[0])) & MASKS[0]; // 0x00FF00FF = 00000000111111110000000011111111

    y = (y | (y << SHIFTS[3])) & MASKS[3]; //  0x55555555 = 01010101010101010101010101010101 
    y = (y | (y << SHIFTS[2])) & MASKS[2]; // 0x33333333 = 00110011001100110011001100110011 
    y = (y | (y << SHIFTS[1])) & MASKS[1]; // 0x0F0F0F0F = 00001111000011110000111100001111 
    y = (y | (y << SHIFTS[0])) & MASKS[0]; // 0x00FF00FF = 00000000111111110000000011111111

    const uint32_t result = x | (y << 1); // intercala los bits de x e y de nuevo
    return result; // devuelve el resultado
}

int main(){

    cout << "Z order to 0 , 0 is: "<< calcZOrder(0, 0) << endl; // 0
    cout << "Z order to 2 , 3 is: "<< calcZOrder(2, 3) << endl; // 14
    cout << "Z order to 3 , 2 is: "<< calcZOrder(3, 2) << endl; // 21
    cout << "Z order to 1 , 1 is: "<< calcZOrder(1, 1) << endl; // 5
    cout << "Z order to 1 , 2 is: "<< calcZOrder(1, 2) << endl; // 9
    cout << "Z order to 6 , 5 is: "<< calcZOrder(6,5) << endl; // 9
    return 0;
}