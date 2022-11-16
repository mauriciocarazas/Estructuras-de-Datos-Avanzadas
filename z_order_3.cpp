#include <stdio.h>
static const unsigned int B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF};
static const unsigned int S[] = {1, 2, 4, 8};
unsigned int zorder2D(unsigned x, unsigned y){
    
    x = (x | (x << S[3])) & B[3];
    x = (x | (x << S[2])) & B[2];
    x = (x | (x << S[1])) & B[1];
    x = (x | (x << S[0])) & B[0];

    y = (y | (y << S[3])) & B[3];
    y = (y | (y << S[2])) & B[2];
    y = (y | (y << S[1])) & B[1];
    y = (y | (y << S[0])) & B[0];
    return x | (y << 1);
}

int main()
{    
    const unsigned nx=8,ny=8;
    unsigned res[ny][nx];

    for(unsigned y=0; y<ny; y++){
        for(unsigned x=0; x<nx; x++){
            res[y][x] = zorder2D(x,y);
            printf("yx=%d %d z=%d\n",y,x,res[y][x]);    
        }
    }
    for(unsigned y=0; y<ny; y++){
        for(unsigned x=0; x<nx; x++){
            printf("%-4d",res[y][x]);
        }
        printf("\n");
    }
    return 0;
}