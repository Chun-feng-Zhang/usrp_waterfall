TARGETS=build/rt_waterfall build/monitor build/rt_wfft build/rt_spec  build/cal2mjd build/rt_cpubackend #build/rt_gpu  #build/rt_backend  #build/now2mjd 
all: $(TARGETS)
FC = gfortran
FFLAGS = -g -fPIC #-ffree-form
CFLAGS = -g -Wall -W -fPIC -O3 -ffast-math -Wno-unused-result
LIBCMD = -shared
USRP=`pwd`

#LIBS=-luhd -lboost_program_options -lboost_date_time -lboost_filesystem -lboost_serialization -lboost_thread -lboost_unit_test_framework -lboost_system  -lboost_atomic `pkg-config --libs libusb-1.0` -lboost_chrono -pthread -ldl -lfftw3f `pkg-config --libs sdl2`
#INC=-I ./include `pkg-config --cflags sdl2`

CPULIBS=-luhd -lboost_program_options -lboost_date_time -lboost_filesystem -lboost_serialization -lboost_thread -lboost_unit_test_framework -lboost_system  -lboost_atomic `pkg-config --libs libusb-1.0` -lboost_chrono -lpthread -ldl `pkg-config --libs libudev` -latomic -lrt -lfftw3f `pkg-config --libs sdl2` 

GPULIBS=-luhd -lboost_program_options -lboost_date_time -lboost_filesystem -lboost_serialization -lboost_thread -lboost_unit_test_framework -lboost_system  -lboost_atomic `pkg-config --libs libusb-1.0` -lboost_chrono -lpthread -ldl `pkg-config --libs libudev` -latomic -lrt -lcufft -lcufftw `pkg-config --libs sdl2` -L/usr/local/cuda/lib64/

CPUINC=-I ./include -I /usr/local/include/uhd -L /usr/local/lib/ `pkg-config --cflags sdl2`

GPUINC=-I ./include -I /usr/local/cuda/include/ `pkg-config --cflags sdl2`

obj/data_proc.o: src/data_proc.cpp include/data_proc.hpp |obj
	g++ -c -o $@ $< -O3 -g $(CPUINC)

obj/GPU_proc.o: src/GPU_proc.cpp include/data_proc.hpp |obj
	g++ -c -o $@ $< -O3 -g $(GPUINC)

obj/daq_queue.o: src/daq_queue.cpp include/daq_queue.hpp |obj
	g++ -c -o $@ $< -O3 -g $(CPUINC)

obj/utils.o: src/utils.cpp include/utils.hpp |obj
	g++ -c -o $@ $< -O3 -g $(CPUINC)

obj/monitor.o: src/monitor.cpp |build
	g++ -c -o $@ $< -O3 -g $(CPUINC)

obj/swap_bytes.o:src/swap_bytes.c
	gcc  -fPIC -c src/swap_bytes.c -o obj/swap_bytes.o
obj/send_stuff.o:src/send_stuff.c obj/swap_bytes.o
	gcc  -c -fPIC src/send_stuff.c obj/swap_bytes.o -o obj/send_stuff.o
obj/filheader.o:src/filheader.c obj/send_stuff.o |build
	gcc  -c -fPIC src/filheader.c obj/send_stuff.o obj/swap_bytes.o  -o obj/filheader.o 
obj/cldj.o: src/slalib/cldj.c
	cc -c -fPIC src/slalib/cldj.c -o obj/cldj.o -lf2c -lm
obj/cal2mjd.o: src/cal2mjd.cpp  |obj
	g++ -c -o $@ src/cal2mjd.cpp   -O3 -g -lf2c -lm
obj/now2mjd.o: src/now2mjd.cpp  |obj
	g++-10 -c -o $@ src/now2mjd.cpp   -O3 -g -lf2c -lm --std=c++20


slalib: libsla.so
	cd src/slalib ; $(FC) -o sla_test sla_test.f -fno-second-underscore -L../../obj -lsla
	src/slalib/sla_test

libsla.so:src/slalib/*.f
	cd src/slalib ; $(FC) $(FFLAGS) -fno-second-underscore -c -I. *.f *.F
	rm src/slalib/sla_test.o
	cd src/slalib ; $(FC) $(LIBCMD) -o ../../obj/libsla.so -fno-second-underscore *.o

build/cal2mjd:  obj/cal2mjd.o obj/cldj.o |obj
	g++ -o $@ obj/cal2mjd.o obj/cldj.o  # -lm -lf2c #-Lobj -lsla -lgfortran #-lf -lg2c -lfortran
build/now2mjd:  obj/now2mjd.o obj/cldj.o |obj
	g++ -o $@ obj/now2mjd.o obj/cldj.o  # -lm -lf2c #-Lobj -lsla -lgfortran #-lf -lg2c -lfortran

build/monitor: obj/monitor.o |build
	g++ $^ -o $@ -O3 $(CPULIBS) -g

obj/rt_waterfall.o: src/rt_waterfall.cpp |obj
	g++ -c -o $@ $< -O3 -g $(CPUINC)

build/rt_waterfall: obj/rt_waterfall.o obj/daq_queue.o obj/data_proc.o obj/cldj.o |build
	g++ $^ -o $@ -O3 $(CPULIBS) $(CPUINC) -g

obj/rt_wfft.o: src/rt_wfft.cpp |obj
	g++ -c -o $@ $< -O3 -g $(CPULIBS) $(CPUINC)

build/rt_wfft: obj/rt_wfft.o obj/daq_queue.o obj/data_proc.o obj/filheader.o obj/send_stuff.o obj/swap_bytes.o obj/cldj.o |build
	g++ $^ -o $@ -O3 $(CPULIBS) $(CPUINC)-g

obj/rt_spec.o: src/rt_spec.cpp |obj
	g++ -c -o $@ $< -O3 -g $(CPULIBS) $(CPUINC)

build/rt_spec: obj/rt_spec.o obj/daq_queue.o obj/data_proc.o obj/filheader.o obj/send_stuff.o obj/swap_bytes.o obj/cldj.o |build
	g++ $^ -o $@ -O3 $(CPULIBS) $(CPUINC) -g

obj/rt_cpubackend.o: src/rt_cpubackend.cpp |obj
	g++ -c -o $@ $< -O3 -g $(CPULIBS) $(CPUINC)

build/rt_cpubackend: obj/rt_cpubackend.o obj/daq_queue.o obj/data_proc.o obj/filheader.o obj/send_stuff.o obj/swap_bytes.o obj/cldj.o |build
	g++ $^ -o $@ -O3 $(CPUINC) $(CPULIBS) -g

obj/rt_gpu.o: src/rt_gpu.cpp |obj
	nvcc -c -o $@ $< -O3 -g $(GPUINC)

build/rt_gpu: obj/rt_gpu.o obj/daq_queue.o obj/GPU_proc.o obj/filheader.o obj/send_stuff.o obj/swap_bytes.o obj/cldj.o |build
	nvcc $^ -o $@ -O3 $(GPUINC) $(GPULIBS) -g

obj/rt_backend.o: src/rt_backend.cpp |obj
	nvcc -c -o $@ $< -O3 -g $(GPUINC) $(GPULIBS)

build/rt_backend: obj/rt_backend.o obj/daq_queue.o obj/data_proc.o obj/filheader.o obj/send_stuff.o obj/swap_bytes.o obj/cldj.o |build
	nvcc $^ -o $@ -O3 $(GPUINC) $(GPULIBS) -g


obj: 
	mkdir -p obj

build:
	mkdir -p build

clean:
	rm -rf build obj
