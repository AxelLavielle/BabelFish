# BabelFish
BabelFish is a log system that have for purpose to be logging from all languages and to be reading &amp; rotating &amp; parsing logs. Next step will also be to monitor the processes that logs something. I started this project during my internship at **Scortex**, and will continue to develop and maintain it during the next years.

### Actually contains:
- libraries for Python, Bash, C, C++ and Javascript(NodeJS)
- examples for Python, Bash, C, C++ and Javascript(NodeJS)
- rotate working, with optimisations
- logs are nicely interpretated both in terminal view and ncurses view
- ncurses view allows the user to change the severity filters on click
- can be filtered by timestamp, component, severity, ... (`-h` for more informations)
- bunch of arguments fully handled

### Specificities:
- C++ needs to be compiled with `-pthread -std=c++11`

### Usage for ./BabelFish:
no required arguments, here are the optional arguments:
- `--speed TIMEINMS` or `-s` Maximum speed for refresh, Default value is 1.
- `--timestamp_begin TIME` or `-tb` Minimum timestamp of display.
- `--timestamp_end TIME` or `-te` Maximum timestamp of display.
- `--component COMPONENT` or `-c` Add the component to the list of display, default is all.
- `--severity SEVERITY` or `-e` Put a severity level in OFF mode and doesn't display it anymore.
Levels: 0 - TRACE ; 1 - DEBUG ; 2 - INFO ; 3 - WARNING ; 4 - ERROR ; 5 - FATAL
- `--delete` or `-d` Delete precedents logs.
- `--nolaunch` or `-n` Doesn't launch the logger.
- `--help` or `-h` Display current helper.
- `--view VIEWER` or `-v` Allows the user to choose the type of view he wants.
Displays possbilites are: 0 - TERMINAL ; 1 - NCURSES ; 2 - GRAPHICAL ; (1 & 2 are currently in developement), default value is 1
