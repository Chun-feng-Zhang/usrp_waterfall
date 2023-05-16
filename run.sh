sudo sysctl -w net.core.rmem_max=200000000
sudo sh -c "echo 1024 > /sys/module/usbcore/parameters/usbfs_memory_mb"
#sudo taskset -c 3,4,5,6 nice -20 ./build/rt_wfft --rate 56e6 --freq 100e6 --nch 2048 --batch 512 --wirefmt sc8 --gain 33 --args 'name=MyB210' --bw 40e6 --nbatch 32 #--fftfile ./test.dat
#sudo taskset -c 3,4,5,6 nice -20 ./build/rt_spec --rate 56e6 --freq 100e6 --nch 2048 --batch 512 --wirefmt sc8 --gain 33 --args 'name=MyB210' --bw 40e6 --nbatch 32 --fftfile ./test.dat
#sudo taskset -c 3,4,5,6 nice -20 ./build/rt_gpu --rate 56e6 --freq 100e6 --nch 2048 --batch 512 --wirefmt sc8 --gain 33 --args 'name=MyB210' --bw 40e6 --nbatch 32 --fftfile ./test.dat
#sudo taskset -c 3,4,5,6 nice -20 ./build/rt_backend --rate 56e6 --freq 100e6 --nch 2048 --batch 512 --wirefmt sc8 --gain 33 --args 'name=MyB210' --bw 40e6 --nbatch 32 --filfile ./test.fil --sourcename testsource 
sudo taskset -c 5,6,7,8 nice -20 ./build/rt_cpubackend --rate 40e6 --freq 100e6 --nch 2048 --batch 512 --wirefmt sc8 --gain 1 --args 'name=MyB210' --bw 40e6 --nbatch 32 --setup 1 --filfile ./test.fil --sourcename testsource
#sudo taskset -c 3,4,5,6 nice -20 ./build/rt_cpubackend --rate 40e6 --freq 100e6 --nch 2048 --batch 512 --wirefmt sc8 --gain 1 --args 'name=MyB210' --bw 40e6 --nbatch 32 

