#!/usr/bin/env python3

import sys, getopt, os.path
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def main(argv):
  inputfile = ''
  outputfile = 'performances.png'

  try:
    opts, args = getopt.getopt(argv,"hi:o:",["ifile=","ofile="])
  except getopt.GetoptError:
    print('plot_threads_time.py -i <inputfile> [-o <outputfile>]')
    sys.exit(2)
  for opt, arg in opts:
    if opt == '-h':
      print('plot_threads_time.py -i <inputfile> [-o <outputfile>]')
      sys.exit()
    elif opt in ("-i", "--ifile"):
      inputfile = arg
    elif opt in ("-o", "--ofile"):
      outputfile = arg

  if os.path.isfile(inputfile):
    df = pd.read_csv(inputfile)
    boxplot(inputfile, outputfile, df)
    plot(inputfile, outputfile, df)
  else:
    print('Unable to open input file: ' + inputfile)
    sys.exit()

def plot(inputfile, outputfile, df):
  plt.figure()
  arr = df.values

  #compute means for all threads
  means = []
  stdev = []
  for column in arr.T:
    means.append(np.mean(column))
    stdev.append(np.std(column))

  #graph setup
  plt.title('Performance test')
  plt.xlabel('Threads')
  plt.ylabel('Time')
  plt.xlim(1, len(means))
  plt.grid(True)
  plt.xticks(range(0,len(means) + 2))
  plt.yticks(range(0,30))

  #plotting
  plt.plot(range(1, len(means) + 1), means, label='mean')
  plt.plot(range(1, len(stdev) + 1), stdev, label='standard deviation')

  plt.legend()
  plt.savefig('plot_' + outputfile + '.png')
  #plt.show()
  plt.close()

def boxplot(inputfile, outputfile, df):
  plt.figure()

  # find max value in dataframe
  maxy = df.max().max()

  # setup graph
  plt.title('Performance test')
  plt.xlabel('Threads')
  plt.ylabel('Time')
  plt.yticks(range(0, maxy.astype(np.int64) + 2))

  # plot data
  df.boxplot()

  plt.savefig('boxplot_' + outputfile + '.png')
  #plt.show()
  plt.close()

if __name__ == "__main__":
   main(sys.argv[1:])
