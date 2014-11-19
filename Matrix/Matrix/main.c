#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "point.h"

int main()
{
    int n=5;
    int m=5;
    Matrix *m1=Matrix_create(n,m);
    Matrix *m2=Matrix_create_identity(n);
    
    int i,j;
    double val=1;
    for (i=0;i<n;i++){
        for(j=0;j<m;j++){
            val=((double)random()/RAND_MAX)*1;
            if(((double)random()/RAND_MAX)<0.5) val*=-1;
            Matrix_set(m1,i,j,val);
        }
    }
    
    
    Matrix *top=Matrix_product(m1,m2);
    Matrix_dump(m1);
    printf("_________Inverting____\n");
    m2=Matrix_inverse(m1);
    printf("OK\n");
    Matrix_dump(Matrix_inverse(m1));
    
    
    Point *p1 = Point_create(0.1, 0.56);
    Point *p2 = Point_create(5, 25.4);
    
    Point *sum = Point_add(p1, p2);
    printf("_________Sum____\n");
    Point_dump(sum);
    Point *midpoint = Point_get_midpoint(p1, p2);
    printf("_________Midpoint____\n");
    Point_dump(midpoint);

    printf("________Distance____\n");
    double distance = Point_get_distance_between(p1, p2);
    printf("%0.5f ",distance);
    
    return 0;
}
