# Assignment 1


Language: C++

Execute Programs:
Part I:
Execution:
g++ shellskel.cpp -o shellskel
./shellskel

Part II:
Execution
g++ serial.cpp -o serial
./serial

time ./serial
real    0m2.686s
user    0m0.013s
sys     0m0.063s
Execution:
g++ parallel.cpp -o parallel
./parallel

time ./parallel
real    0m1.229s
user    0m0.030s
sys     0m0.041s

These are the file links we used for the file downloaders:
https://designyoutrust.com/wp-content/uploads/2020/08/111.jpg
http://photos1.blogger.com/x/blogger/6472/1203/1600/601713/Golden%20Rain%202.jpg
https://locofunjumps.files.wordpress.com/2012/12/another_colorful_fractal_by_darkdissolution-d4arljg.png
https://sites.psu.edu/siowfa15/wp-content/uploads/sites/29639/2015/09/dinosaurs-god_00428328.jpg
The urls.txt will contain 10 of the same link which has a 100MB downloaders

Answer to Questions:
In the output of time, what is the difference between real, user, and sys times?

Real time is wall clock time much like a stopwatch, time perceived by the user.
User and system time is the amount of time the process spends executing on the CPU.
User time is the time spent in user mode whereas sys time is the time spent in system mode.

Which is longer: user time or sys time? Use your knowledge to explain why.

Sys time is going to take longer than user time. This is because system time measures
the amount of time within the kernel. Since our program is mostly system calls such as
fork(), execlp(), and wait(), the machine is in system mode for the majority of execution of the process.
Therefore, it must be greater.

When downloading the files above, which downloader finishes faster? Why do you think that is?

The downloader that finishes the fastest is the parallel downloader. This is because the parallel
downloader downloads multiple files in parallel while the serial downloader downloads each file
individually which would take more time since the files are downloaded one at a time. We are also
maximizing resource utilization when in parallel, making parallel faster.


Repeat the experiment for 10 files (any reasonably large-sized files, e.g., 100 MB, will do).
Is the downloader in the previous question still faster? If not so, why do you think that is?

Serial:
real    2m21.410s
user    0m0.773s
sys    0m15.574s
Parallel
real    0m50.047s
user    0m0.632s
sys    0m13.681s

Yes, the parallel downloader is still faster than the serial downloader when downloading 10 files of 100MB.

Did not implement extra credit

Source: https://blog.gceasy.io/2016/12/08/real-time-greater-than-user-and-sys-time/
