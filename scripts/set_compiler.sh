#!/bin/sh

if [ -f "/etc/redhat-release" ]
	then return=$"c++"
elif [ -f "/etc/debian_version" ]
	then return="c++"
else return="c++"
fi

echo $return
