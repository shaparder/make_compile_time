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
THRDS			:=	8

# directories
SRCS_DIR	:=	./srcs
INCS_DIR	:=	./includes
OBJS_DIR	:=	./objs

# compiler and flags
CC				:=	gcc
CFLAGS		:=	-Wall -Wextra -Werror -std=gnu99 -g
LIBS			:=	-lpthread -lm


.PHONY: time all prodcons readwrit philo

all: prodcons readwrit philo perf_prodcons

philo:

prodcons:

readwrit:

clean:
	@rm -rf $(OBJS_DIR)/*.o
	@rm -rf $(NAME1)
	@rm -rf $(NAME2)
	@rm -rf $(NAME3)
	@echo "Objects cleaned."
	@echo "Executable cleaned."

re: clean all

perf_philo: philo
	@cd ./timing
	@echo "Performances measurements for $(NAME1) ..."
	@./threads_perf.sh $(NAME1) $(THRDS) > $(PERF1)
	@echo "Plotting ..."
	@python3 plot_threads_time.py -i $(PERF1) -o $(NAME1)
	@rm -rf $(PERF1)
	@echo "Done."
	@cd ..

perf_prodcons: prodcons
	@cd ./timing
	@echo "Performances measurements for $(NAME2) ..."
	@./threads_perf.sh $(NAME2) $(THRDS) > $(PERF2)
	@echo "Plotting ..."
	@python3 plot_threads_time.py -i $(PERF2) -o $(NAME2)
	@rm -rf $(PERF2)
	@echo "Done."
	@cd ..

perf_readwrit: readwrit
	@cd ./timing
	@echo "Performances measurements for $(NAME3) ..."
	@./threads_perf.sh $(NAME3) $(THRDS) > $(PERF3)
	@echo "Plotting ..."
	@python3 plot_threads_time.py -i $(PERF3) -o $(NAME3)
	@rm -rf $(PERF3)
	@echo "Done."
	@cd ..

perf_all: perf_philo perf_prodcons perf_readwrit

clean_perf:
	@cd timing
	@rm -rf ./timing/*.png
