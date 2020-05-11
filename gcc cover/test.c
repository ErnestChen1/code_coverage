#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int putOut_max(int a, int b)
{
    return(a > b ? a : b);
}

int main(void)
{
    int value1 = 10;
    int value2 = 25;
    int max = 0;
    
    printf("Address of main function %p\n\n", &main);
    
    max = putOut_max(value1, value2);
    
    printf("max data is: %d \n", max);
    
    return 0;
}
