#!/bin/bash

function	TRACE {
    return "TRACE"
}

function	DEBUG {
    return "DEBUG"
}

function	INFO {
    return "INFO"
}

function	WARNING {
    return "WARNING"
}

function	ERROR {
    return "ERROR"
}

function	FATAL {
    return "FATAL"
}

function	Log {
    if [[ $1 != TRACE && $1 != DEBUG && $1 != INFO && $1 != WARNING && $1 != ERROR && $1 != FATAL ]]
    then
	echo "Severity is not okay, please use: TRACE DEBUG INFO WARNING ERROR FATAL as first argument.";
	return
    fi
    if [[ $2 == "" ]]
    then
	echo "Component is not okay, please enter the name of your component as second argument.";
	return
    fi
    if [[ $3 == "" ]]
    then
	echo "Message is not okay, please enter a message as third argument.";
	return
    fi
    timestamp=`date +%s%3N`
    severity=$1
    processID=$BASHPID
    threadID=$$
    component=$2
    shift
    shift
    message=$@
    echo "["$timestamp"]["$severity"]["$processID"]["$threadID"]["$component"]--"$message"--" >> ~/.logs/logs
}
