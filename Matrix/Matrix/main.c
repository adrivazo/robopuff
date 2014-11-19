#include <stdio.h>
#include <stdlib.h>
#include "math_util.h"

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
    
    
    Point *p0 = Point_create(1, 10);
    Point *p1 = Point_create(25, 88);
    Point *p2 = Point_create(9, 153);
    Point *p3 = Point_create(45, 76);
    
    Point *sum = Point_add(p1, p2);
    printf("_________Sum____\n");
    Point_dump(sum);
    Point *midpoint = Point_get_midpoint(p1, p2);
    printf("_________Midpoint____\n");
    Point_dump(midpoint);

    printf("________Distance____\n");
    double distance = Point_get_distance_between(p1, p2);
    printf("%0.5f ",distance);
    
    
    Point points[4];
    points[0]=*p0;
    points[1]=*p1;
    points[2]=*p2;
    points[3]=*p3;
    
    
    Matrix *DM = Util_get_distance_matrix(points);
    
    printf("________Distance Matrix____\n");
    Matrix_dump(DM);
    return 0;
}
