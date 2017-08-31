#include					"LogParser.hh"

LogParser::LogParser()
{
  userParam.stop = false;
  userParam.quit = false;
  userParam.timestamp_start = -1;
  userParam.timestamp_end = -1;
  userParam.severity[0] = true;
  userParam.severity[1] = true;
  userParam.severity[2] = true;
  userParam.severity[3] = true;
  userParam.severity[4] = true;
  userParam.severity[5] = true;
  userParam.lineThreshold = -1;
}

LogParser::~LogParser()
{
  _file.close();
}

void						LogParser::setFile()
{
  _file.close();
}

void						LogParser::setFile(const std::string &filename)
{
  std::ofstream					opener;

  _filename = filename;
  _file.close();
  //try opening normally the file
  _file.open(_filename.c_str());
  if (!_file.is_open())
    {//if failed, force open
      opener.open(_filename.c_str());
      opener.close();
      _file.open(_filename.c_str());
      if (!_file.is_open())//see if force open worked
	{
	  std::cerr << "Couldn't open the file '" << filename << "' please verify that the file exists and that you have the rights." << std::endl;
	  exit(1);
	}
    }
}

const std::string				LogParser::extractObject(const std::string &bef, const std::string &aft, const int pos, const std::string &str) const
{
  std::size_t					ptrBef;
  std::size_t					ptrAft;
  int						counter;

  ptrBef = -1;
  ptrAft = -1;
  counter = -1;
  while ((ptrBef != std::string::npos && ptrAft != std::string::npos) || counter == -1)
    if ((ptrBef = str.find(bef, ptrAft + 1)) != std::string::npos &&
	(ptrAft = str.find(aft, ptrBef + 1)) != std::string::npos &&
	++counter == pos)
      return (str.substr(ptrBef + bef.length(), ptrAft - ptrBef - bef.length()));
    else if (counter == -1)
      return ("");
  return ("");
}

const t_log					LogParser::parseLine(const std::string &line) const
{
  t_log						log;
  std::string					tmp;

  log.valid = true;

  //Extracting Timestamp
  if ((tmp = this->extractObject("[", "]", 0, line)) == "")
    log.valid = false;
  try {log.timestamp = std::stol(tmp);} catch (...) {log.valid = false;}

  //Extracting Severity as String
  if ((tmp = this->extractObject("[", "]", 1, line)) == "")
    log.valid = false;
  log.severityStr = tmp;

  //Extracting Severity as Severity
  log.severity = (log.severityStr == "TRACE") ? (TRACE)
    : ((log.severityStr == "DEBUG") ? (DEBUG)
       : ((log.severityStr == "INFO") ? (INFO)
	  : ((log.severityStr == "WARNING") ? (WARNING)
	     : ((log.severityStr == "ERROR") ? (ERROR)
		: ((log.severityStr == "FATAL") ? (FATAL) : (DEFAULT))))));
  if (log.severity == DEFAULT)
    log.valid = false;

  //Extracting ProcessID
  if ((tmp = this->extractObject("[", "]", 2, line)) == "")
    log.valid = false;
  try {log.processID = std::stoi(tmp);} catch (...) {log.valid = false;}

  //Extracting ThreadID
  if ((tmp = this->extractObject("[", "]", 3, line)) == "")
    log.valid = false;
  try {log.threadID = std::stol(tmp);} catch (...) {log.valid = false;}

  //Extracting Component
  if ((tmp = this->extractObject("[", "]", 4, line)) == "")
    log.valid = false;
  log.component = tmp;

  //Extracting Message
  if ((tmp = this->extractObject("--", "--", 0, line)) == "")
    log.valid = false;
  log.message = tmp;
  return (log);
}

void						LogParser::warning(const std::string &line) const
{
  std::cout << YELLOW << "WARNING !!! Wrong log message formating for: \"" + line + "\"" << NEUTRAL << std::endl;
}

bool						LogParser::validateLine(const t_log &log) const
{
  unsigned int					i;

  if (!userParam.severity[log.severity])
    return (false);
  if (userParam.timestamp_start != -1 && userParam.timestamp_start >= log.timestamp)
    return (false);
  if (userParam.timestamp_end != -1 && userParam.timestamp_end <= log.timestamp)
    return (false);
  i = -1;
  while (++i != userParam.component.size() && userParam.component[i] != log.component);
  if (userParam.component.size() && i == userParam.component.size())
    return (false);
  return (true);
}

const std::pair<std::vector<t_log>, int>	LogParser::getBuffer()
{
  std::string					line;
  std::string					tmp;
  t_log						log;
  int						i;

  _file.seekg(std::ios::beg);
  _file.clear();
  _buffer.clear();
  tmp = "";
  i = 0;
  while (std::getline(_file, line))
    {
      while (line.find("--", line.length() - 3) == std::string::npos && std::getline(_file, tmp))
	line += "\n" + tmp;
      if (line.find("--", line.length() - 3) != std::string::npos)
	{
	  log = this->parseLine(tmp);
	  if (log.valid)
	    warning(line.substr(0, line.length() - tmp.length() - 1));
	  else
	    {
	      log = this->parseLine(line);
	      if (!log.valid)
		warning(line);
	    }
	  if (this->validateLine(log) && ++i)
	    _buffer.push_back(log);
	}
      else
	warning(line);
    }
  return (std::make_pair(_buffer, i));
}

const t_line					LogParser::statInfo() const
{
  t_line					info;

  info.name = _filename;
  info.timestamp.first = -1;
  info.timestamp.second = -1;
  info.severity.push_back(0);
  info.severity.push_back(0);
  info.severity.push_back(0);
  info.severity.push_back(0);
  info.severity.push_back(0);
  info.severity.push_back(0);
  return (info);
}

const t_line					LogParser::statInfo(t_line info, const t_log &log)
{
  unsigned int					c;

  if (info.timestamp.first > log.timestamp || info.timestamp.first == -1)
    info.timestamp.first = log.timestamp;
  if (info.timestamp.second < log.timestamp || info.timestamp.second == -1)
    info.timestamp.second = log.timestamp;
  c = 0;
  while (c != info.component.size() && info.component[c] != log.component)
    c++;
  if (c == info.component.size())
    info.component.push_back(log.component);
  info.severity[log.severity] += 1;
  return (info);
}

const t_line					LogParser::getInfo()
{
  std::string					line;
  std::string					tmp;
  t_log						log;
  t_line					info;

  _file.seekg(std::ios::beg);
  _file.clear();
  _buffer.clear();
  tmp = "";
  info = statInfo();
  while (std::getline(_file, line))
    {
      while (line.find("--", line.length() - 3) == std::string::npos && std::getline(_file, tmp))
	line += "\n" + tmp;
      if (line.find("--", line.length() - 3) != std::string::npos)
	{
	  log = this->parseLine(tmp);
	  if (!log.valid)
	    log = this->parseLine(line);
	  if (log.valid)
	    info = statInfo(info, log);
	}
    }
  return (info);
}
