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

inline const std::string	getFile()
{
  static std::string home = getpwuid(getuid())->pw_dir;
  static std::string filepath = home + "/.logs/";
  static int useless = mkdir(filepath.c_str(), 0755);
  (void)useless;
  return (filepath+LOGFILE);
}

class		Log
{
private:
  std::ostringstream	oss;
public:
  Log(const std::string &component, const Severity sev)
  {
    long			timestamp = TIMESTAMP;
    pid_t			processID = getpid();
    std::thread::id	threadID = std::this_thread::get_id();

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
	this->oss << "DEFAULT";
	break;
      }
    this->oss << "][" << processID << "][" << threadID << "][" << component << "]--";
  }

  ~Log()
  {
    std::ofstream outfile;

    this->oss << "--" << std::endl;
    outfile.open(getFile(), std::ofstream::out | std::ofstream::app);
    outfile << this->oss.str();
    outfile.close();
  }
  template<typename T>
  Log	&	operator<<(T msg)
  {
    this->oss << msg;
    return (*this);
  }
};
  
#endif	//	__LOG__
