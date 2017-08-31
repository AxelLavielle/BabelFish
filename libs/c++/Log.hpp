#ifndef		__LOG__
# define	__LOG__

# include	<string>
# include	<fstream>
# include	<sstream>
# include	<iostream>
# include	<ctime>
# include	<chrono>
# include	<thread>
# include	<unistd.h>
# include	<sys/stat.h>
# include	<sys/types.h>
# include	<pwd.h>

enum		Severity
  {
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL,
    DEFAULT
  };

# define	LOG(x, y) Log(x, y)
# define	LOGFILE "logs"
# define	TIMESTAMP std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()

inline const std::string	___getFile__()
{
  static std::string home = getpwuid(getuid())->pw_dir;
  static std::string filepath = home + "/.logs/";
  static int useless = mkdir(filepath.c_str(), 0755);
  (void)useless;
  return (filepath+LOGFILE);
}

class		Log
{
public:
  Log(const std::string &component, const Severity sev)
  {
    long			timestamp = TIMESTAMP;
    pid_t			processID = getpid();
    std::thread::id	threadID = std::this_thread::get_id();

    _write = true;
    _written = false;
    this->oss << "[" << timestamp << "][";
    switch (sev)
      {
      case TRACE:
	this->oss << "TRACE";
	break;
      case DEBUG:
	this->oss << "DEBUG";
	break;
      case INFO:
	this->oss << "INFO";
	break;
      case WARNING:
	this->oss << "WARNING";
	break;
      case ERROR:
	this->oss << "ERROR";
	break;
      case FATAL:
	this->oss << "FATAL";
	break;
      default:
	this->oss << "ERROR";
	_write = false;
	break;
      }
    this->oss << "][" << processID << "][" << threadID << "][" << component << "]--";
    if (!_write)
      this->oss << "Bad severity, please choose one of the following: TRACE DEBUG INFO WARNING ERROR FATAL";
  }

  ~Log()
  {
    std::ofstream outfile;

    if (!_written && _write)
      this->oss << "Empty message, please enter something using left shift (<<)";
    this->oss << "--" << std::endl;
    outfile.open(___getFile__(), std::ofstream::out | std::ofstream::app);
    outfile << this->oss.str();
    outfile.close();
  }
  template<typename T>
  Log	&	operator<<(T msg)
  {
    _written = true;
    if (_write)
      this->oss << msg;
    return (*this);
  }
private:
  std::ostringstream	oss;
  bool			_write;
  bool			_written;
};
  
#endif	//	__LOG__
