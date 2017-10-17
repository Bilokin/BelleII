#!/bin/bash
parallelProcesses=3
genFileName="evtgen"
recoFileName="reco-tf2signal"
root=".root"
startingNumber=$1
number=$((startingNumber))
if [[ -z "$1" ]]; then
	startingNumber=1
fi
folderName=$2
if [[ -z "$2" ]]; then
	folderName="test"
fi
fileNames=()
for (( i = 0; i < parallelProcesses; i++ )); do
	echo "basf2 Y4SEventGeneration.py $folderName/$genFileName$number$root > /dev/null &"
	basf2 Y4SEventGeneration.py $folderName/$genFileName$number$root > /dev/null &
	fileNames+=($folderName/$genFileName$number$root)
	((number++))
done
for job in `jobs -p`
do
	echo $job
	wait $job
done
number=$((startingNumber))
for (( i = 0; i < parallelProcesses; i++ )); do
	echo "basf2 SimulateAndReconstruct-withoutBeamBkg.py $folderName/$genFileName$number$root $folderName/$recoFileName$number$root > /dev/null &"
	basf2 SimulateAndReconstruct-withoutBeamBkg.py $folderName/$genFileName$number$root $folderName/$recoFileName$number$root > /dev/null &
	((number++))
done
