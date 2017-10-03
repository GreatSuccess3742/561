#!/bin/bash
echo "\n"
echo "#############################################"
echo "#   CSCI561 HW1 CHECKER Ver2   BY TERRY     #"
echo "#############################################"
echo "\n"

javac *.java 
cd ./hw1checker/fail_cases
rm *.txt
cd ../../
cp ./hw1checker/init/log.txt ./hw1checker/log.txt
cd ./hw1checker
javac *.java
cd ../

for((i=0;i<50;i++))
do
    cd ./hw1checker
    java GeneratorHW1
    cd ../
    cp ./hw1checker/input.txt input.txt
    ./homework
    cp output.txt ./hw1checker/output.txt
    cd ./hw1checker
    java CheckerHW1
    cd ../
done
rm input.txt
rm output.txt
cp ./hw1checker/log.txt log.txt
# cp ./hw1checker/fail_cases ./log_fail_cases
more log.txt