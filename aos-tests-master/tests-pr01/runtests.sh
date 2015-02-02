#! /bin/sh
#
# runtests.sh
# Copyright (C) 2015 benjamin <benjamin@assus>
#
# Distributed under terms of the MIT license.
#
clear
for f in ./build/*; do
	if [[ -f "$f" ]]
	then
		printf "\n"
		echo "Press [Enter] to run " $f
		read -p ""
		exec $f &
		sleep 0.2
	fi
done
