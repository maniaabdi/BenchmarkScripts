#!/bin/sh

GREEN='\033[1;32m'
NC='\033[0m'

echo "${GREEN} Run Input Alluxio, Output HDFS (local)${NC}"
./benchmark_tpch.sh 4 alluxio://neu-5-1:19998/tpch/in /user/root/tpch/out_alx 1 ALLUXIO HDFS | tee ALLUXIO_HDFS_8wrk.log

#echo "${GREEN} Run Input S3, Output HDFS (local)${NC}"
#./benchmark_tpch.sh 5 s3a://hadoop/tpch/in /user/root/tpch/out_s3 1 S3 HDFS | tee S3_HDFS_8wrk.log

echo "${GREEN} Run Input HDFS, Output HDFS (local)${NC}"
./benchmark_tpch.sh 5 /user/root/tpch/in /user/root/tpch/out_hdfs 1 HDFS HDFS | tee HDFS_HDFS_8wrk.log
