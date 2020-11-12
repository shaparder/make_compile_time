


.PHONY: time

time:
	@cd ./timing
	./threads_perf.sh 8 > threads_time.csv
	python3 plot_threads_time.py
