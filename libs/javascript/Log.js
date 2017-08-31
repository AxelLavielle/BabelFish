var process = require('process');
var fs = require('fs');

var Log = function Log(severity, component, argument)
{
    var processID = process.pid;
    var threadID = 1;
    var ts = + new Date();
    var message = "";

    if (!fs.existsSync(process.env.HOME+'/.logs'))
    {
	fs.mkdirSync(process.env.HOME+'/.logs');
    }
    try
    {
	for (let arg of argument)
	{
	    message += arg.toString();
	}
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

Log.trace = function(component, ...message)
{
    Log("TRACE", component, message);
}

Log.info = function(component, ...message)
{
    Log("INFO", component, message);
}

Log.debug = function(component, ...message)
{
    Log("DEBUG", component, message);
}

Log.warning = function(component, ...message)
{
    Log("WARNING", component, message);
}

Log.error = function(component, ...message)
{
    Log("ERROR", component, ...message);
}

Log.fatal = function(component, ...message)
{
    Log("FATAL", component, message);
}

module.exports = Log;
