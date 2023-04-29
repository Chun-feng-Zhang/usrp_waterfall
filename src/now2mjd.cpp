#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


//extern "C" {
//double slaCldj(int iy, int im, int id, int *j);
////double sla_CLDJ(int iy, int im, int id, int *j);
//}

extern "C" {
int sla_cldj__(long int *iy, long int *im, long int *id, 
	double *djm, long int *j);
}


//double slacldj(int iy, int im, int id, int *j);

/* Does not include jump discontinuities from leap seconds.  */
/* I don't really know if it should !                        */

#ifdef USEDMALLOC
#include "dmalloc.h"
#endif

int main(int argc, char *argv[])
{
    long int year, month = 1, day = 1, hour = 0, min = 0, err;
    double MJD, fracday, sec = 0.0;

    time_t rawtime;
    struct tm * ptm;

    time ( &rawtime );

    ptm = gmtime ( &rawtime );

    year =   (long int ) ptm->tm_year + 1900;
    month =  (long int ) ptm->tm_mon + 1;
    day =    (long int ) ptm->tm_mday;
    hour =   (long int ) ptm->tm_hour;
    min =    (long int ) ptm->tm_min;
    sec =    (double )   ptm->tm_sec;

    //printf("now is year %ld month %ld day %ld hour %ld min %ld sec %f",
            //year,month,day,hour,min,sec);

    fracday = (hour + (min + (sec / 60.0)) / 60.0) / 24.0;
    //MJD = slaCldj(year, month, day, &err);
    //MJD = sla_CLDJ(year, month, day, &err);
    sla_cldj__(&year, &month, &day, &MJD, &err);
    MJD += fracday;
    if (err == 1) {
        printf("\nTry again.  Bad year.\n\n");
        exit(1);
    }
    
    if (err == 2) {
        printf("\nTry again.  Bad month.\n\n");
        exit(1);
    }
    if (err == 3) {
        printf("\nTry again.  Bad day.\n\n");
        exit(1);
    }
    printf("\nMJD is %17.11f\n\n", MJD);
    exit(0);
}
