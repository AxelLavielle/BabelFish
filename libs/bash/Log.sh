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
    mkdir ~/.logs > /dev/null 2>&1
    timestamp=`date +%s%3N`
    severity=$1
    processID=$BASHPID
    threadID=$$
    component=$2
    shift
    shift
    message=$@
    if [[ $severity != TRACE && $severity != DEBUG && $severity != INFO && $severity != WARNING && $severity != ERROR && $severity != FATAL ]]
    then
	severity=ERROR;
	message="Severity is not okay, please use: TRACE DEBUG INFO WARNING ERROR FATAL as first argument.";
    fi
    if [[ $component == "" ]]
    then
	severity=ERROR;
	component="Logger";
	message="Component is not okay, please enter the name of your component as second argument.";
    fi
    if [[ $message == "" ]]
    then
	severity=ERROR;
	message="Message is not okay, please enter a message as third argument.";
    fi
    echo "["$timestamp"]["$severity"]["$processID"]["$threadID"]["$component"]--"$message"--" >> ~/.logs/logs
}
