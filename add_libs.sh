#!/usr/bin/env bash

# run with bash -x add_libs.sh to get bash to print the commands before running them

repos=(
https://github.com/jrowberg/i2cdevlib.git
https://github.com/fmalpartida/New-LiquidCrystal.git
https://github.com/SRGDamia1/NeoSWSerial.git
https://github.com/johnrickman/LiquidCrystal_I2C.git
https://github.com/pawsen/BigCrystal.git
https://github.com/adafruit/Adafruit_Sensor.git
https://github.com/adafruit/Adafruit_BusIO.git
https://github.com/adafruit/Adafruit_ADXL345.git
)


for repo in "${repos[@]}"
do
	# add repo as remote.
	# Use positive lookbehind to get ensure the match follows a /
	# and positive lookahead to match until .git
	name=$(grep -oP '(?<=\/)[^\/]+(?=.git)' <<< "$repo" )
	reponame=$(tr "/A-Z/" "/a-z/" <<< "$name")
	git remote add "$reponame" "$repo"
	# get name of default branch
	branch=$(git remote show "$reponame" | sed -n '/HEAD branch/s/.*: //p')
	echo "adding $name:$branch as a subtree"
	# add with mixed case -- not all lowercase
	git subtree add --prefix lib/"$name" --squash "$reponame" "$branch"
done
