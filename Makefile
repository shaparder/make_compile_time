# **************************************************************************** #
#                                                                              #
#    main.c                                                                    #
#                                                                              #
#    By: shaparder <oscar.fally@student.uclouvain.be>                          #
#                                                                              #
#    Created: 13/11/2020 22:33:23 by shaparder                                 #
#                                                                              #
# **************************************************************************** #

# executable name
NAME 			:=

# directories
SRCS_DIR	:=	./srcs
INCS_DIR	:=	./includes
OBJS_DIR	:=	./objs


.PHONY: time

time:
	@cd ./timing
	@./threads_perf.sh 8 > threads_time.csv
	@python3 plot_threads_time.py
	@cd ..
