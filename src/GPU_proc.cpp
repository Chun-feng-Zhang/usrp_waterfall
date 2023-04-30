#include <cufftw.h>
#include <atomic>
#include <utils.hpp>
#include <data_proc.hpp>
#include <time.h>


void waterfall(BufQ<DataFrame>& bufq, size_t nch, size_t batch, std::atomic_bool& stop_signal_called, std::function<void(const DataFrame&)> handler_f, std::function<void(const DataFrame&)> handler_t){
    stop_signal_called=false;
    std::cerr<<stop_signal_called<<std::endl;
    int n[]={(int)nch};
    int howmany=(int)batch;
    int idist=(int)nch;
    int odist=(int)nch;
    int istride=1;
    int ostride=1;
    int* inembed=n;
    int* onembed=n;
    int rank=1;
    fftwf_plan plan=fftwf_plan_many_dft(rank, n, howmany,
                            nullptr, inembed,
                            istride, idist,
                            nullptr, onembed,
                            ostride, odist,
                            FFTW_FORWARD, FFTW_ESTIMATE);

    size_t prev_cnt=0;
    clock_t t;
        t = clock();


    for(int i=0;!stop_signal_called;++i){
        //std::cout << "full iter time: "<< i << " : " << (float)(clock() -t)/CLOCKS_PER_SEC <<std::endl;
        t = clock();
        auto data=bufq.fetch();
        //std::cout << "fetch data took: "<< (float)(clock() -t)/CLOCKS_PER_SEC <<std::endl;
        clock_t t0 = clock();
        //t = clock();
        handler_t(*data);
        //std::cout << "handle t data: "<< (float)(clock() -t)/CLOCKS_PER_SEC <<std::endl;
        //std::cerr<<"iter: "<< i<<" count: "<<data->count<< " prev: " << prev_cnt << std::endl;
        if (data->count>prev_cnt+1){
            std::cerr<<"Dropping packet: \n"<< data->count - prev_cnt -1<< std::endl;
        }
        
        prev_cnt=data->count;
        //t = clock();
        fftwf_execute_dft(plan, (fftwf_complex*)data->payload.data(), (fftwf_complex*)data->payload.data());
        //std::cout << "fft took: "<< (float)(clock() -t)/CLOCKS_PER_SEC <<std::endl;
        //t = clock();
        fft_shift(data->payload, nch, batch);
        //std::cout << "fft shift took: "<< (float)(clock() -t)/CLOCKS_PER_SEC <<std::endl;
        
        if(i%100==0){
            std::cerr<<i<<" "<<data->count<<std::endl;
        }
        
        //t = clock();
        handler_f(*data);
        //std::cout << "handle f data: "<< (float)(clock() -t)/CLOCKS_PER_SEC <<std::endl;
        //std::cout << "full cycle took : "<< (float)(clock() -t0)/CLOCKS_PER_SEC <<std::endl;
    }
    fftwf_destroy_plan(plan);
}


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


