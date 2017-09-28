#include			"TermViewer.hh"

TermViewer::TermViewer()
{

}

TermViewer::~TermViewer()
{

}

const std::string		TermViewer::logToString(const t_log &log) const
{
  std::ostringstream		oss;
  long				timestampForDate = log.timestamp / 1000;
  char				timestampAsStr[256];
  struct tm			*dt;

  dt = localtime(&timestampForDate);
  std::strftime(timestampAsStr, sizeof(timestampAsStr), "%D %T.", dt);
  oss << " " <<
    //Timestamp
    timestampAsStr << std::setfill('0') << std::setw(3) << log.timestamp - 1000 * timestampForDate << " | "
    //Severity
      << ((log.severity == TRACE) ? (GREEN)
	  : ((log.severity == DEBUG) ? (BLUE)
	     : ((log.severity == INFO) ? (CYAN)
		: ((log.severity == WARNING) ? (YELLOW)
		   : ((log.severity == ERROR) ? (RED) : (MAGENTA))))));
  oss.width(7);
  oss << std::setfill(' ') << std::left << log.severityStr << NEUTRAL << " | ";
    //ProcessID
  oss.width(5);
  oss << std::setfill('0') << std::right << log.processID << " | 0x";
    //ThreadID
  oss.width(12);
  oss << std::setfill('0') << std::hex << log.threadID << " | ";
    //Component
  oss.width(12);
  oss << std::setfill(' ') << std::left << log.component
    //Message
      << " | " << log.message;
  return (oss.str());
}

void				TermViewer::displayHeader()
{

}

void				TermViewer::displayLogs(const std::vector<t_log> &data)
{
  unsigned int			i;

  i = -1;
  _logs.str(std::string());
  while (++i < data.size())
    _logs << this->logToString(data[i]) << std::endl;
}

void				TermViewer::refresh() const
{
  std::cout << _logs.str() << std::endl;
}

void				TermViewer::wait(const long ts)
{
  usleep((ts > TIMESTAMP) ? ((ts - TIMESTAMP) * 1000) : 0);
}
