import os
import threading
import time

def     printlog(severity, component, message):
    processID = os.getpid()
    threadID = threading.current_thread()
    timestamp = time.time() * 1000
    if not os.path.exists(os.getenv("HOME")+'/.logs'):
        try:
            os.makedirs(os.getenv("HOME")+'/.logs');
        except:
            pass
    try:
        message = str(message)
    except:
        severity = "ERROR"
        message = "Message can not be converted as string."
    with open(os.getenv("HOME")+'/.logs/logs', 'ab') as f:
        f.write("[%d][%s][%s][%s][%s]--%s--\n"%(timestamp, severity, processID, threadID.ident, component, message))

def     trace(component, message):
    printlog("TRACE", component, message)

def     debug(component, message):
    printlog("DEBUG", component, message)

def     info(component, message):
    printlog("INFO", component, message)

def     warning(component, message):
    printlog("WARNING", component, message)

def     error(component, message):
    printlog("ERROR", component, message)

def     fatal(component, message):
    printlog("FATAL", component, message)

