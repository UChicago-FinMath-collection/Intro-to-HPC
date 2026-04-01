How to access rcc:

logs for getting the file.

[glotov@midway3-login1 ~]$ cd hpc2026/
[glotov@midway3-login1 hpc2026]$ cp /project2/finm32950/L1Demo.tar .
[glotov@midway3-login1 hpc2026]$ ls
L1Demo.tar
[glotov@midway3-login1 hpc2026]$ tar -xvf L1Demo.tar 
L1Demo/
L1Demo/AdvCPP/
L1Demo/AdvCPP/demo
L1Demo/AdvCPP/demo.cpp
L1Demo/Profiling/
L1Demo/Profiling/bs1
L1Demo/Profiling/Makefile
L1Demo/Profiling/CMake/
L1Demo/Profiling/CMake/CMakeLists.txt
L1Demo/Profiling/CMake/src/
L1Demo/Profiling/CMake/src/bs1.cpp
L1Demo/Profiling/bs1.cpp
L1Demo/Profiling/run_vtune
[glotov@midway3-login1 hpc2026]$ ls
L1Demo  L1Demo.tar
[glotov@midway3-login1 hpc2026]$ 

[glotov@midway3-login1 hpc2026]$ cd L1Demo/
[glotov@midway3-login1 L1Demo]$ ls
AdvCPP  Profiling
[glotov@midway3-login1 L1Demo]$ cd Profiling/
[glotov@midway3-login1 Profiling]$ ls
bs1  bs1.cpp  CMake  Makefile  run_vtune
[glotov@midway3-login1 Profiling]$ nano bs1.cpp 
bash: nano: command not found
[glotov@midway3-login1 Profiling]$ vim bs1.cpp 
[glotov@midway3-login1 Profiling]$ 

