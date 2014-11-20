#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "math_util.h"
#include <math.h>

int main()
{
    //constellation points & other constants
    double x_ab = 10.563;
    double x_bd =11.655;
    double y_cb = 14.5*2;
    double y_ca = y_cb/2 + 2.583;
    double y_cd = y_cb/2 + 8.741;
    double stars_origin[2] = {x_ab, y_cb/2};
    Point *camera_origin = Point_create(1024.0/2,768.0/2);

    //stars
    Point *s0 = Point_create(x_ab, y_cb);
    Point *s1 = Point_create(x_ab, 0);
    Point *s2 = Point_create(x_ab+x_bd, y_cd);
    Point *s3 = Point_create(0, y_ca);
    Point stars[4] ={*s0,*s1,*s2,*s3};
    
    //load the sample data - this is just for testing
    char buffer[1024] ;
    char *record,*line;
    int k=0,l=0;
    
    const int NUM_LINES_TO_PROCESS = 1376;
    int sample_points[NUM_LINES_TO_PROCESS][8];
    FILE *fstream = fopen("/Users/adri.vazquez/Dropbox/MEAM410 Mechatronics/Robockey/robopuff/Matrix/Matrix/StarsA.csv","r");
    if(fstream == NULL)
    {
        printf("\n file opening failed ");
        return -1 ;
    }
    
    line=fgets(buffer,sizeof(buffer),fstream);
    record = strtok(line,",");
    for(k=0;k<NUM_LINES_TO_PROCESS;k++){
        for(l=0;l<8;l++){
            if(record!=NULL){
                sample_points[k][l] = atoi(record);
                }
            record = strtok(NULL,",");
            }
        line=fgets(buffer,sizeof(buffer),fstream);
        record = strtok(line,",");
        }
    
    fclose(fstream);
    printf("\n No of lines read %0.5d", k);
    
    //robo localization
    //create arrays with past coordinates and angles - this is just for testing
    double robo_coordinates [k][2];
    float robo_thetas [k];
    int rows_ignored = 0;
    
    //process data
    for(int a=0;a<k;a++){
    
        //read the sampled points x0,x1,x2,x3,y0,y1,y2,y3
        Point *sp0 = Point_create(sample_points[a][0], sample_points[a][4]);
        Point *sp1 = Point_create(sample_points[a][1], sample_points[a][5]);
        Point *sp2 = Point_create(sample_points[a][2], sample_points[a][6]);
        Point *sp3 = Point_create(sample_points[a][3], sample_points[a][7]);
        
        int numBadStars = 0;// use if we do the 3 star scenario
        //ignore if we have a bad star
        
        if(Util_is_bad_star(sp0)||Util_is_bad_star(sp1)||Util_is_bad_star(sp2)||Util_is_bad_star(sp3)){
            rows_ignored++;
            continue;
        }
        
    Point sample_points[4]={*sp0,*sp1,*sp2,*sp3};
    Matrix *distance_matrix = Util_get_distance_matrix(sample_points);
    // find indexes of min and max r-row, c-col
    int r_c_min[2];
    int r_c_max[2];
    Matrix_find_min(r_c_min, distance_matrix);
    Matrix_find_max(r_c_max, distance_matrix);
    
    //find which star is pointing in the positive y direction
        
    int north_star = Util_find_intersection(r_c_min,r_c_max);
        if (isnan(north_star)){
            rows_ignored++;
            continue;
        }

    int south_star = Util_find_other_star(north_star, r_c_max);
    Point * sample_point_origin = Point_get_midpoint(&sample_points[north_star], &sample_points[south_star]);//check!!!
    Point * origin_offset = Point_sub(sample_point_origin, camera_origin);
    Point * vect_south_to_origin = Point_sub(&sample_points[south_star], sample_point_origin);
    float theta = atan2f(-Point_get_x(vect_south_to_origin), Point_get_y(vect_south_to_origin));
    double p0x = Point_get_x(origin_offset);
    double p0y = Point_get_y(origin_offset);
        
    //H_stars_robot
    Matrix * H_stars_robot = Matrix_create(3, 3);
    float sin_theta = sinf(theta);
    float cos_theta = cosf(theta);
    Matrix_set(H_stars_robot, 0, 0, cos_theta);  Matrix_set(H_stars_robot, 0, 1, -sin_theta); Matrix_set(H_stars_robot, 0, 2, p0x);
    Matrix_set(H_stars_robot, 1, 0, sin_theta);  Matrix_set(H_stars_robot, 1, 1, cos_theta);  Matrix_set(H_stars_robot, 1, 2, p0y);
    Matrix_set(H_stars_robot, 2, 0, 0);          Matrix_set(H_stars_robot, 2, 1, 0);          Matrix_set(H_stars_robot, 2, 2, 1);
    
    //    printf("\n -------------- H_stars_robot -------------- \n");
    Matrix_dump(H_stars_robot);
    Matrix * H_robot_stars = Matrix_inverse(H_stars_robot);
    //    printf("\n -------------- H_robot_stars -------------- \n");
    Matrix_dump(H_robot_stars);

    float robo_theta = acosf(Matrix_get(H_robot_stars, 0, 0));
    double robo_x = Matrix_get(H_robot_stars, 0, 2);
    double robo_y = Matrix_get(H_robot_stars, 1, 2);
        
    robo_coordinates[a][0]=robo_x;
    robo_coordinates[a][1]=robo_y;
    robo_thetas[a]=robo_theta;
    }
    
    FILE *fstream2 = fopen("/Users/adri.vazquez/Dropbox/MEAM410 Mechatronics/Robockey/robopuff/Matrix/Matrix/Results.csv","w");
    if(fstream2 == NULL)
    {
        printf("\n file opening failed ");
        return -1 ;
    }
  
    for(k=0;k<NUM_LINES_TO_PROCESS-rows_ignored;k++){
        for(l=0;l<2;l++){
            fprintf(fstream2,"%f,",robo_coordinates[k][l]);
        }
        
        fprintf(fstream2,"%f,",robo_thetas[k]);
        fprintf(fstream2,"\n");
    }

    fclose(fstream2);
    return 0;
}