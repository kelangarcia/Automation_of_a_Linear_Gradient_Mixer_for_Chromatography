#include <stdio.h>
#include <math.h>
#define ITERATIONS 900

int main(){

    int percentageA, percentageB, time1, time2,n;
    float times[ITERATIONS];
    double q;

    printf("Insert Time 1:\n");
    scanf("%d", &time1);
    printf("Insert Percentage A:\n");
    scanf("%d", &percentageA);
    printf("Insert Time 2:\n");
    scanf("%d", &time2);
    printf("Insert Percentage B:\n");
    scanf("%d", &percentageB);
    
    n = time1 * 10;
    printf("n iterations: %d\n", n);
    for (int x = 0; x < n; x++){
        q = percentageA * 60;
        printf("\n%lf\n",  q);
        printf("%lf\n",  6000 - q);
    }
    
    int Bandwidth = time2 - time1;
    n = Bandwidth * 10;
    double delta = (double)(percentageB - percentageA) / n;
    double intantpercentageA;
    printf("N: %d\n", n);
    printf("Delta %lf\n", delta);
    for (int x = 0; x <= n; x++){
        intantpercentageA = percentageA + delta * x;
        printf("aumentando: %lf\n", intantpercentageA);
        q = (double) intantpercentageA * 60;
        printf("%lf\n",  floor(q));
    }
    return 0;
}
