# AIO implementation assessment

This projects gathers a list of benchmark pattern that allows to assess different AIO implementations.</br>
It is made of two parts:

## The benchmark implementation (directory ./srcBenchmark)
This directory contains a list of benchmark pattern.
Each pattern is implemented in the directory src/unitPerformanceAssess and extends the C++ class "Worker".
Each implementation has a specific "Compute method".  It also has a specific "save" method that allows to write in a given file (using different AIO implementations).</br>
A main programs (file src/main.cc) allows to run one of this pattern in a loop after fixing the benchmark parameters (using the command line). </br>
Each implemented pattern may be generated from the root directory using the commend: "make pattern_<pattern name>"
Then the pattern may be tested using the command.</br>
"./pattern_<patter name> [optional parameters]" </br>
The different parameters may be see by using the option "-help".



## The benchmark pattern instrumentation(./jubeTester)
The Jube file "jubeTester/benchmarkInstrumentation.xml" runs the different pattern benchmarks previously described and assess them.
The comand for that is: "jube run benchmarkInstrumentation.xml".</br>
In order to print all the defined benchmark, one can simply go to the root directory and run the command</br>
"make runAllBenchmark"



## The result printer (++++++++ TODO ++++++++)
For now, we simply print (using vi) for each tested benchmark, and for each parameter set, the time of each function.</br>


