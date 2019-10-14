#!/bin/bash

GREEN='\033[1;32m'
NC='\033[0m'

#echo "${GREEN} Run Input Alluxio, Output HDFS (local)${NC}"
#./benchmark_tpch.sh 4 alluxio://neu-5-1:19998/tpch/in /user/root/tpch/out_alx 1 ALLUXIO HDFS | tee ALLUXIO_HDFS_8wrk.log

echo "${GREEN} Run Input S3, Output HDFS (local)${NC}"
for ((  j = 1 ;  j <= 5;  j++  ))
do
     ./run_tpch.sh s3a://data/tpch-8G /tpch/q6_8g_1rep_s3 1 S3
done

#echo "${GREEN} Run Input HDFS, Output HDFS (local)${NC}"
#./run_tpch.sh /tpch-pig/in /tpch-pig/out 1 HDFS  | tee pig-mapreduce-1G.log

#echo "${GREEN} Run Input Alluxio, Output HDFS (local)${NC}"
#./benchmark_tpch.sh 1 alluxio://neu-5-1:19998/tpch/in /user/root/tpch/out_4rep_alx 1 ALLUXIO HDFS | tee ALLUXIO_HDFS_4wrk_4rep.log
