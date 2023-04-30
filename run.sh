#sudo taskset -c 3,4,5,6 nice -20 ./build/rt_wfft --rate 56e6 --freq 100e6 --nch 2048 --batch 512 --wirefmt sc8 --gain 33 --args 'name=MyB210' --bw 40e6 --nbatch 32 --fftfile ./test.dat
#sudo taskset -c 3,4,5,6 nice -20 ./build/rt_spec --rate 56e6 --freq 100e6 --nch 2048 --batch 512 --wirefmt sc8 --gain 33 --args 'name=MyB210' --bw 40e6 --nbatch 32 --fftfile ./test.dat
#sudo taskset -c 3,4,5,6 nice -20 ./build/rt_gpu --rate 56e6 --freq 100e6 --nch 2048 --batch 512 --wirefmt sc8 --gain 33 --args 'name=MyB210' --bw 40e6 --nbatch 32 --fftfile ./test.dat
sudo taskset -c 3,4,5,6 nice -20 ./build/rt_backend --rate 56e6 --freq 100e6 --nch 2048 --batch 512 --wirefmt sc8 --gain 33 --args 'name=MyB210' --bw 40e6 --nbatch 32 --filfile ./test.fil --sourcename testsource
#sudo taskset -c 3,4,5,6 nice -20 ./build/rt_cpubackend --rate 56e6 --freq 100e6 --nch 2048 --batch 512 --wirefmt sc8 --gain 33 --args 'name=MyB210' --bw 40e6 --nbatch 32 --filfile ./test.fil --sourcename testsource

