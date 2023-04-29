#ifndef UTILS_HPP
#define UTILS_HPP
#include <cassert>
#include <string>
#include <complex>
#include <vector>
#include <tuple>
#include "config.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>



template <typename A>
void fft_shift(A& data, size_t nch, size_t batch){
    for(int i=0;i<batch;++i){
        //for(int j=0;j<nch/2;++j){
            //std::swap(data[i*nch+j], data[i*nch+j+nch/2]);
        //}

        for( int j =0; j<nch/4; ++j){
            std::swap(data[i*nch+j], data[i*nch+nch/4-j]);
        }
        for( int j=nch/2; j<3*nch/4; ++j){
            std::swap(data[i*nch+j], data[i*nch-j+nch/2]);
        }


    }
}


std::tuple<float,float, std::vector<SAMP_TYPE>> minmax(const std::vector<std::complex<SAMP_TYPE>>& data);

#endif


extern "C" {
int sla_cldj__(long int *iy, long int *im, long int *id, 
	double *djm, long int *j);
}


//double slacldj(int iy, int im, int id, int *j);

/* Does not include jump discontinuities from leap seconds.  */
/* I don't really know if it should ! -PRESTO 
 * No you shouldnt  -WW                      */


double now2mjd()
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
        printf("\nTry again.  Bad year: %ld \n\n", year);
        return 60000.;
    }
    if (err == 2) {
        printf("\nTry again.  Bad month: %ld \n\n", month);
        return 60000.;
    }
    if (err == 3) {
        printf("\nTry again.  Bad day: %ld \n\n", day);
        return 60000.;
    }
    return MJD;
    //printf("\nMJD is %17.11f\n\n", MJD);
    //exit(0);
}

