#!/bin/bash
parallelProcesses=4
root=".root"
query=$1
outFileName=$2
number=1
if [[ -z "$2" ]]; then
	outFileName="test"
fi
fileNames=""
for file in `ls $query`
do
	echo "basf2 RecoDecays.py $file tmp-$outFileName$number$root > /dev/null &"
	basf2 RecoDecays-noK.py $file tmp-$outFileName$number$root > /dev/null &
	fileNames+=tmp-$outFileName$number$root" "
	if [[ $number%$parallelProcesses -eq 0 ]]; then
		echo "And now we wait..."
		for job in `jobs -p`
		do
			echo $job
			wait $job
		done
	fi
	((number++))
done
echo "Waiting for the last ones..."
for job in `jobs -p`
do
	echo $job
	wait $job
done
echo "hadd -f $outFileName-merged$root $fileNames"
hadd -f $outFileName-merged$root $fileNames
echo "Done!"
