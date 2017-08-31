import os
import threading
import time

def     printlog(severity, component, *args):
    processID = os.getpid()
    threadID = threading.current_thread()
    timestamp = time.time() * 1000
    if not os.path.exists(os.getenv("HOME")+'/.logs'):
        try:
            os.makedirs(os.getenv("HOME")+'/.logs');
        except:
            pass
    try:
        message = "";
        for arg in args:
            message += str(arg)
    except:
        severity = "ERROR"
        message = "Message can not be converted as string."
    with open(os.getenv("HOME")+'/.logs/logs', 'ab') as f:
        f.write("[%d][%s][%s][%s][%s]--%s--\n"%(timestamp, severity, processID, threadID.ident, component, message))

def     trace(component, *args):
    printlog("TRACE", component, *args)

def     debug(component, *args):
    printlog("DEBUG", component, *args)

def     info(component, *args):
    printlog("INFO", component, *args)

def     warning(component, *args):
    printlog("WARNING", component, *args)

def     error(component, *args):
    printlog("ERROR", component, *args)

def     fatal(component, *args):
    printlog("FATAL", component, *args)

