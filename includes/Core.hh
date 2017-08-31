#ifndef					__CORE__
# define				__CORE__

# include				<string.h>
# include				<dirent.h>
# include				"LogParser.hh"
# include				"Rotator.hh"
# include				"TermViewer.hh"
# include				"CursesViewer.hh"

struct					Data
{
  std::vector<t_log>			logs;
};

class					Core
{
public:
  Core();
  ~Core(); 
  void					argumentsHandler(const int ac, char** const& av);
  void					init();
  void					run();

private: 
  void					usage(const char * const& binary) const;
  void					deleteAll(const std::string &path) const;
  int					_speed;
  std::string				_filename;
  LogParser				_logs;
  Rotator				_rotator;
  Data					_data;
  AViewer				*_viewer;
};

#endif	//				__CORE__
