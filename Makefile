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

# perf data files
PERF1			:=	philo_perf.csv
PERF2			:=	prodcons_perf.csv
PERF3			:=	readwrit_perf.csv

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

philo_atomic:
	@rm -rf $(NAME1)_atomic
	@$(CC) $(CFLAGS) $(SRCS)/$(NAME1)$(ATOM).c -o $(NAME1)$(ATOM) $(LIBS)
	@echo "Executable $(NAME1)$(ATOM) created"

clean:
	@rm -rf $(NAME1)
	@rm -rf $(NAME2)
	@rm -rf $(NAME3)
	@echo "Executables cleaned."

re: clean all

cleanall: clean perf_clean

perf_philo: philo
	@echo "Performances measurements for $(NAME1) ..."
	@mkdir -p $(PERFS)
	@./threads_perf.sh $(NAME1) $(MAX_THRDS) $(PERFS)/$(PERF1) $(NOT_SPLIT)
	@echo "Measurements done for $(NAME1) and stored in $(PERFS)/$(PERF1)"
	@echo "Plotting data ..."
	@python3 plot_threads_time.py -i $(PERFS)/$(PERF1) -o $(NAME1)
	@echo "Plotting done and stored in $(PERFS) folder"

perf_philo_atomic: philo_atomic
	@echo "Performances measurements for $(NAME1)$(ATOM) ..."
	@mkdir -p $(PERFS)
	@./threads_perf.sh $(NAME1)$(ATOM) $(MAX_THRDS) $(PERFS)/$(PERF1)$(ATOM) $(NOT_SPLIT)
	@echo "Measurements done for $(NAME1)$(ATOM) and stored in $(PERFS)/$(PERF1)$(ATOM)"
	@echo "Plotting data ..."
	@python3 plot_threads_time.py -i $(PERFS)/$(PERF1)$(ATOM) -o $(NAME1)$(ATOM)
	@echo "Plotting done and stored in $(PERFS) folder"

perf_prodcons: prodcons
	@echo "Performances measurements for $(NAME2) ..."
	@mkdir -p $(PERFS)
	@./threads_perf.sh $(NAME2) $(MAX_THRDS) $(PERFS)/$(PERF2) $(SPLIT)
	@echo "Measurements done for $(NAME2) and stored in $(PERFS)/$(PERF2)"
	@echo "Plotting data ..."
	@python3 plot_threads_time.py -i $(PERFS)/$(PERF2) -o $(NAME2)
	@echo "Plotting done and stored in $(PERFS) folder"

perf_readwrit: readwrit
	@echo "Performances measurements for $(NAME3) ..."
	@mkdir -p $(PERFS)
	@./threads_perf.sh $(NAME3) $(MAX_THRDS) $(PERFS)/$(PERF3) $(SPLIT)
	@echo "Measurements done for $(NAME3) and stored in $(PERFS)/$(PERF3)"
	@echo "Plotting data ..."
	@python3 plot_threads_time.py -i $(PERFS)/$(PERF3) -o $(NAME3)
	@echo "Plotting done and stored in $(PERFS) folder"


perf_all: perf_philo perf_prodcons perf_readwrit

perf_clean:
	@rm -rf $(PERFS)
	@echo "perfs folder deleted"
