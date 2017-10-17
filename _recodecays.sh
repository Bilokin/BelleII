#!/bin/bash
parallelProcesses=3
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
outFileName=$3
if [[ -z "$3" ]]; then
	outFileName="test"
fi
fileNames=""
for (( i = 0; i < parallelProcesses; i++ )); do
	echo "basf2 RecoDecays.py $folderName/$recoFileName$number$root $outFileName$number$root > /dev/null &"
	basf2 RecoDecays.py $folderName/$recoFileName$number$root $outFileName$number$root > /dev/null &
	fileNames+=$outFileName$number$root" "
	((number++))
done
for job in `jobs -p`
do
	echo $job
	wait $job
done
number=$((startingNumber))
echo "hadd -f $outFileName-merged$root $fileNames"
hadd -f $outFileName-merged$root $fileNames
