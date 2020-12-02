# **************************************************************************** #
#                                                                              #
#    main.c                                                                    #
#                                                                              #
#    By: shaparder <oscar.fally@student.uclouvain.be>                          #
#                                                                              #
#    Created: 13/11/2020 22:33:23 by shaparder                                 #
#                                                                              #
# **************************************************************************** #

# executables names
NAME1			:=	philo
NAME2			:=	prodcons
NAME3			:=	readwrit
ATOM			:=	_atomic
SYNC			:=	synchro
TS				:=	testnset
TTS				:=	testntestnset

# variables
MAX_THRDS :=	8
SPLIT			:=	1
NOT_SPLIT :=	0

# directories
SRCS      :=  ./srcs
PERFS     :=  ./perfs

# compiler and flags
CC				:=	gcc
CFLAGS		:=	-Wall -Wextra -Werror -std=gnu99
LIBS			:=	-lpthread


.PHONY: all prodcons readwrit philo perf_philo perf_prodcons perf_readwrit perf_all perf_clean

all: prodcons readwrit philo

philo:
	@rm -rf $(NAME1)
	@$(CC) $(CFLAGS) $(SRCS)/$(NAME1).c -o $(NAME1) $(LIBS)
	@echo "Executable $(NAME1) created"

prodcons:
	@rm -rf $(NAME2)
	@$(CC) $(CFLAGS) $(SRCS)/$(NAME2).c -o $(NAME2) $(LIBS)
	@echo "Executable $(NAME2) created"

readwrit:
	@rm -rf $(NAME3)
	@$(CC) $(CFLAGS) $(SRCS)/$(NAME3).c -o $(NAME3) $(LIBS)
	@echo "Executable $(NAME3) created"

testnset:
	@rm -rf $(TS)
	@$(CC) $(CFLAGS) $(SRCS)/$(TS).c $(SRCS)/$(SYNC).c -o $(TS) $(LIBS)
	@echo "Executable $(TS) created"

testntestnset:
	@rm -rf $(TTS)
	@$(CC) $(CFLAGS) $(SRCS)/$(TTS).c $(SRCS)/$(SYNC).c -o $(TTS) $(LIBS)
	@echo "Executable $(TTS) created"

philo_atomic:
	@rm -rf $(NAME1)$(ATOM)
	@$(CC) $(CFLAGS) $(SRCS)/$(NAME1)$(ATOM).c $(SRCS)/$(SYNC).c -o $(NAME1)$(ATOM) $(LIBS)
	@echo "Executable $(NAME1)$(ATOM) created"

prodcons_atomic:
	@rm -rf $(NAME2)$(ATOM)
	@$(CC) $(CFLAGS) $(SRCS)/$(NAME2)$(ATOM).c $(SRCS)/$(SYNC).c -o $(NAME2)$(ATOM) $(LIBS)
	@echo "Executable $(NAME2)$(ATOM) created"

readwrit_atomic:
	@rm -rf $(NAME3)$(ATOM)
	@$(CC) $(CFLAGS) $(SRCS)/$(NAME3)$(ATOM).c $(SRCS)/$(SYNC).c -o $(NAME3)$(ATOM) $(LIBS)
	@echo "Executable $(NAME3)$(ATOM) created"

clean:
	@rm -rf $(NAME1)
	@rm -rf $(NAME1)$(ATOM)
	@rm -rf $(NAME2)
	@rm -rf $(NAME2)$(ATOM)
	@rm -rf $(NAME3)
	@rm -rf $(NAME3)$(ATOM)
	@echo "Executables cleaned."

re: clean all

cleanall: clean perf_clean

perf_philo: philo
	@echo "Performances measurements for $(NAME1) ..."
	@mkdir -p $(PERFS)
	@./threads_perf.sh $(NAME1) $(MAX_THRDS) $(PERFS)/$(NAME1)_perf.csv $(NOT_SPLIT)
	@echo "Measurements done for $(NAME1) and stored in $(PERFS)/$(NAME1)_perf.csv"
	@echo "Plotting data ..."
	@python3 plot_threads_time.py -i $(PERFS)/$(NAME1)_perf.csv -o $(NAME1)
	@echo "Plotting done and stored in $(PERFS) folder"

perf_philo_atomic: philo_atomic
	@echo "Performances measurements for $(NAME1)$(ATOM) ..."
	@mkdir -p $(PERFS)
	@./threads_perf.sh $(NAME1)$(ATOM) $(MAX_THRDS) $(PERFS)/$(NAME1)$(ATOM)_perf.csv $(NOT_SPLIT)
	@echo "Measurements done for $(NAME1)$(ATOM) and stored in $(PERFS)/$(NAME1)$(ATOM)_perf.csv"
	@echo "Plotting data ..."
	@python3 plot_threads_time.py -i $(PERFS)/$(NAME1)$(ATOM)_perf.csv -o $(NAME1)$(ATOM)
	@echo "Plotting done and stored in $(PERFS) folder"

perf_prodcons: prodcons
	@echo "Performances measurements for $(NAME2) ..."
	@mkdir -p $(PERFS)
	@./threads_perf.sh $(NAME2) $(MAX_THRDS) $(PERFS)/$(PERF2) $(SPLIT)
	@echo "Measurements done for $(NAME2) and stored in $(PERFS)/$(PERF2)"
	@echo "Plotting data ..."
	@python3 plot_threads_time.py -i $(PERFS)/$(PERF2) -o $(NAME2)
	@echo "Plotting done and stored in $(PERFS) folder"

perf_prodcons_atomic: prodcons_atomic
	@echo "Performances measurements for $(NAME2)$(ATOM) ..."
	@mkdir -p $(PERFS)
	@./threads_perf.sh $(NAME2)$(ATOM) $(MAX_THRDS) $(PERFS)/$(NAME2)$(ATOM)_perf.csv $(SPLIT)
	@echo "Measurements done for $(NAME2)$(ATOM) and stored in $(PERFS)/$(NAME2)$(ATOM)_perf.csv"
	@echo "Plotting data ..."
	@python3 plot_threads_time.py -i $(PERFS)/$(NAME2)$(ATOM)_perf.csv -o $(NAME2)$(ATOM)
	@echo "Plotting done and stored in $(PERFS) folder"

perf_readwrit: readwrit
	@echo "Performances measurements for $(NAME3) ..."
	@mkdir -p $(PERFS)
	@./threads_perf.sh $(NAME3) $(MAX_THRDS) $(PERFS)/$(PERF3) $(SPLIT)
	@echo "Measurements done for $(NAME3) and stored in $(PERFS)/$(PERF3)"
	@echo "Plotting data ..."
	@python3 plot_threads_time.py -i $(PERFS)/$(PERF3) -o $(NAME3)
	@echo "Plotting done and stored in $(PERFS) folder"

perf_readwrit_atomic: readwrit_atomic
	@echo "Performances measurements for $(NAME3)$(ATOM) ..."
	@mkdir -p $(PERFS)
	@./threads_perf.sh $(NAME3)$(ATOM) $(MAX_THRDS) $(PERFS)/$(NAME3)$(ATOM)_perf.csv $(SPLIT)
	@echo "Measurements done for $(NAME3)$(ATOM) and stored in $(PERFS)/$(NAME3)$(ATOM)_perf.csv"
	@echo "Plotting data ..."
	@python3 plot_threads_time.py -i $(PERFS)/$(NAME3)$(ATOM)_perf.csv -o $(NAME3)$(ATOM)
	@echo "Plotting done and stored in $(PERFS) folder"

perf_testnset: testnset
	@echo "Performances measurements for $(TS) ..."
	@mkdir -p $(PERFS)
	@./threads_perf.sh $(TS) $(MAX_THRDS) $(PERFS)/$(TS)_perf.csv $(NOT_SPLIT)
	@echo "Measurements done for $(TS) and stored in $(PERFS)/$(TS)_perf.csv"
	@echo "Plotting data ..."
	@python3 plot_threads_time.py -i $(PERFS)/$(TS)_perf.csv -o $(TS)
	@echo "Plotting done and stored in $(PERFS) folder"


perf_all: perf_philo perf_prodcons perf_readwrit

perf_clean:
	@rm -rf $(PERFS)
	@echo "perfs folder deleted"
