#!/usr/bin/env bash

# Show usage if we go less than 3 arguments
if [ $# -lt 6 ]; then
  echo "Usage: $0 input_dir output_dir reducers [hadoop_opts]"
  echo ""
  echo "   repeats:     The number of times to run the script"
  echo "   input_dir:   The input directory on HDFS"
  echo "   output_dir:  The output directory on HDFS"
  echo "   reducers:    The number of reduce tasks to use per MapReduce job"
  echo "   read_type:   HDFS or ALLUXIO"
  echo "   write_type:  HDFS or ALLUXIO"
  echo ""
  exit 1
fi

# Get the input parameters
N_REPEATS=$1
INPUT_DIR=$2
OUTPUT_DIR=$3
REDUCERS=$4
READ_TYPE=$5
WRITE_TYPE=$6

shift; shift; shift;

###########################################################
# EXECUTE THE BENCHMARK
declare cmd=""
declare total_pig_times=0

for ((  i = 1 ;  i <= $N_REPEATS;  i++  ))
do
    echo "Running Repeat #$i $READ_TYPE $WRITE_TYPE"
    cmd="${PWD}/run_tpch.sh ${INPUT_DIR} ${OUTPUT_DIR}${i} ${REDUCERS} ${READ_TYPE}"
    echo "Going to run:"
    echo $cmd
    echo ""

    # Run the pig script      
    start=$(date +%s)
    $cmd
    end=$(date +%s)
    pig_times=$(( $end - $start ))
    total_pig_times=$(( $total_pig_times + $pig_times ))
  
    echo "Run Round$i times (sec):	$pig_times"
    echo ""

done

# Done
echo "Total times (sec):	$total_pig_times"
echo ""

