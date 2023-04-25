#include <fftw3.h>
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

    for(int i=0;!stop_signal_called;++i){
        //t = clock();
        auto data=bufq.fetch();
        //std::cout << "fetch data took: "<< (float)(clock() -t)/CLOCKS_PER_SEC <<std::endl;
        //clock_t t0 = clock();
        //t = clock();
        handler_t(*data);
        //std::cout << "handle t data: "<< (float)(clock() -t)/CLOCKS_PER_SEC <<std::endl;
        if (data->count>prev_cnt+1){
            std::cerr<<"Dropping packet "<< data->count - prev_cnt -1<< std::endl;
        }
        
        prev_cnt=data->count;
        //t = clock();
        fftwf_execute_dft(plan, (fftwf_complex*)data->payload.data(), (fftwf_complex*)data->payload.data());
        //std::cout << "fft took: "<< (float)(clock() -t)/CLOCKS_PER_SEC <<std::endl;
        //t = clock();
        fft_shift(data->payload, nch, batch);
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
