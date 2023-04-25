TARGETS=build/rt_waterfall build/monitor build/rt_wfft build/rt_spec build/rt_gpu
all: $(TARGETS)

#LIBS=-luhd -lboost_program_options -lboost_date_time -lboost_filesystem -lboost_serialization -lboost_thread -lboost_unit_test_framework -lboost_system  -lboost_atomic `pkg-config --libs libusb-1.0` -lboost_chrono -pthread -ldl -lfftw3f `pkg-config --libs sdl2`
#INC=-I ./include `pkg-config --cflags sdl2`

CPULIBS=-luhd -lboost_program_options -lboost_date_time -lboost_filesystem -lboost_serialization -lboost_thread -lboost_unit_test_framework -lboost_system  -lboost_atomic `pkg-config --libs libusb-1.0` -lboost_chrono -lpthread -ldl `pkg-config --libs libudev` -latomic -lrt -lfftw3f `pkg-config --libs sdl2` 

GPULIBS=-luhd -lboost_program_options -lboost_date_time -lboost_filesystem -lboost_serialization -lboost_thread -lboost_unit_test_framework -lboost_system  -lboost_atomic `pkg-config --libs libusb-1.0` -lboost_chrono -lpthread -ldl `pkg-config --libs libudev` -latomic -lrt -lcufft -lcufftw `pkg-config --libs sdl2` -L/usr/local/cuda/lib64/

CPUINC=-I ./include `pkg-config --cflags sdl2`

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

build/monitor: obj/monitor.o |build
	g++ $^ -o $@ -O3 $(CPULIBS) -g

obj/rt_waterfall.o: src/rt_waterfall.cpp |obj
	g++ -c -o $@ $< -O3 -g $(CPUINC)

build/rt_waterfall: obj/rt_waterfall.o obj/daq_queue.o obj/utils.o obj/data_proc.o |build
	g++ $^ -o $@ -O3 $(CPULIBS) -g

obj/rt_wfft.o: src/rt_wfft.cpp |obj
	g++ -c -o $@ $< -O3 -g $(CPUINC)

build/rt_wfft: obj/rt_wfft.o obj/daq_queue.o obj/utils.o obj/data_proc.o |build
	g++ $^ -o $@ -O3 $(CPULIBS) -g

obj/rt_spec.o: src/rt_spec.cpp |obj
	g++ -c -o $@ $< -O3 -g $(CPUINC)

build/rt_spec: obj/rt_spec.o obj/daq_queue.o obj/utils.o obj/data_proc.o |build
	g++ $^ -o $@ -O3 $(CPULIBS) -g

obj/rt_gpu.o: src/rt_gpu.cpp |obj
	nvcc -c -o $@ $< -O3 -g $(GPUINC)

build/rt_gpu: obj/rt_gpu.o obj/daq_queue.o obj/utils.o obj/GPU_proc.o |build
	nvcc $^ -o $@ -O3 $(GPULIBS) -g

obj: 
	mkdir -p obj

build:
	mkdir -p build

clean:
	rm -rf build obj
