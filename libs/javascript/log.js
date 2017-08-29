var process = require('process');
var fs = require('fs');

var log = function log(severity, component, message)
{
    var processID = process.pid;
    var threadID = 1;
    var ts = + new Date();

    if (!fs.existsSync(process.env.HOME+'/.logs'))
    {
	fs.mkdirSync(process.env.HOME+'/.logs');
    }
    try
    {
	message = message.toString();
    }
    catch (e)
    {
	severity = "ERROR";
	message = "Couldn't transform the object into a string."
    }
    fs.appendFile(process.env.HOME+'/.logs/logs', "["+ts+"]["+severity+"]["+processID+"]["+threadID+"]["+component+"]--"+message+"--\n", function (err) {
	if (err) throw err;
    });
}

log.trace = function(component, message)
{
    log("TRACE", component, message);
}

log.info = function(component, message)
{
    log("INFO", component, message);
}

log.debug = function(component, message)
{
    log("DEBUG", component, message);
}

log.warning = function(component, message)
{
    log("WARNING", component, message);
}

log.error = function(component, message)
{
    log("ERROR", component, message);
}

log.fatal = function(component, message)
{
    log("FATAL", component, message);
}

module.exports = log;
