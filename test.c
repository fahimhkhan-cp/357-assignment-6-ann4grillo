#include <stdio.h>

void main(){
    char *test = "1.3";
    float f;
    sscanf(test, "%f", &f);
    printf("%f\n", f);

}