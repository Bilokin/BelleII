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
	echo "basf2 _RecoDecaysKres.py $file tmp-$outFileName$number$root > /dev/null &"
	basf2 RecoDecaysKres.py $file root/tmp-$outFileName$number$root > /dev/null &
	fileNames+=root/tmp-$outFileName$number$root" "
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
echo "hadd -f root/$outFileName-merged$root $fileNames"
hadd -f root/$outFileName-merged$root $fileNames
echo "Done!"
