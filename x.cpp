#include <stdio.h>

int grid[3][10] = {
    {1,2,3,4,5,6,7,8,9}, 
    {11,12,13,14,15,16,17,18,19}, 
    {22,23,24,25,26,27,28,29}
};

int main() {
    for(int y = 0; y < 3; ++y) {
        printf("\n");
        int temp = y;
        for(int x = 0; temp > -1; ++x) {
            printf("%d,", grid[temp][x]);
            temp--;
        }
    }


    printf("\n");
}