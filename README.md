# LINFO1252 Projet 1

C programs using multithreading, based on 3 well known problems to study their implementations and performances. The bash and python scripts measure performances. 2 different implementations, one using posix threads and one using primitives to lock atomicly values. Then comparing time performances of both.

## Files architecture:
- **srcs** : Contains all the C files to compile.
- **Makefile** : File used to define commands compiling and executing the program.
- **plot_threads_time.py** && **threads_perf.sh** : Scripts handling performance (time-related) measurements.

## Program names:
Call 'make [program_name]' or 'make perf_[program_name]' with one of these three problems name to compile executable or compute threads performances.
- **philo** : Dining Philosophers problem.
- **prodcons** : Producers Consumers problem.
- **readwrit** : Readers Writers problem.


## Usage:

Two choices:

- compiling executables is done with the make command
```
make [program_name]
```

- compute and plot threads performances with the make command
```
make perf_[program_name]
```

