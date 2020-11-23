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

# perf data files
PERF1			:=	perf_philo.csv
PERF2			:=	perf_prodcons.csv
PERF3			:=	perf_readwrit.csv

# variables
MAX_THRDS :=	8

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

prodcons:
	@$(CC) $(CFLAGS) $(SRCS)/$(NAME2).c -o $(NAME2) $(LIBS)
	@echo "Executable $(NAME2) created"

readwrit:

clean:
	@rm -rf $(NAME1)
	@rm -rf $(NAME2)
	@rm -rf $(NAME3)
	@echo "Executables cleaned."

re: clean all

cleanall: clean perf_clean

perf_philo: philo


perf_prodcons: prodcons
	@echo "Performances measurements for $(NAME2)"
	@echo "Hold up it's going to take a while ..."
	@./threads_perf.sh $(NAME2) $(MAX_THRDS) $(PERFS)/$(PERF2)
	@echo "Measurements done for $(NAME2) and stored in $(PERFS)/$(PERF2)"
	@echo "Plotting data ..."
	@python3 plot_threads_time.py -i $(PERFS)/$(PERF2) -o $(NAME2)
	@echo "Plotting done and stored in $(PERFS) folder"

perf_readwrit: readwrit


perf_all: perf_philo perf_prodcons perf_readwrit

perf_clean:
	@rm -rf $(PERFS)/*.png
	@rm -rf $(PERFS)/*.csv
	@echo ".csv and .png cleaned"
