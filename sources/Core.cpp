#include					"Core.hh"

Core::Core()
{
  _filename = getpwuid(getuid())->pw_dir;
  _filename += "/.logs/";
  _speed = 1;
  _viewer = NULL;
}

Core::~Core()
{
  delete(_viewer);
}

void						Core::usage(const char * const& binary) const
{
  if (_viewer != NULL)
    delete(_viewer);
  std::cout << "Usage for " << binary << ":" << std::endl;
  std::cout << "no required arguments, here are the optional arguments:" << std::endl;
  std::cout << "'--speed TIMEINMS' or '-s' Maximum speed for refresh, Default value is 1." << std::endl;
  std::cout << "'--timestamp_begin TIME' or '-tb' Minimum timestamp of display." << std::endl;
  std::cout << "'--timestamp_end TIME' or '-te' Maximum timestamp of display." << std::endl;
  std::cout << "'--component COMPONENT' or '-c' Add the component to the list of display, default is all." << std::endl;
  std::cout << "'--severity SEVERITY' or '-e' Put a severity level in OFF mode and doesn't display it anymore." << std::endl;
  std::cout << "Levels: 0 - TRACE ; 1 - DEBUG ; 2 - INFO ; 3 - WARNING ; 4 - ERROR ; 5 - FATAL" << std::endl;
  std::cout << "'--delete' or '-d' Delete precedents logs." << std::endl;
  std::cout << "'--line LINE' or '-l' Set the number of line to keep, oldest lines are deleted during rotate." << std::endl;
  std::cout << "'--nolaunch' or '-n' Doesn't launch the logger." << std::endl;
  std::cout << "'--help' or '-h' Display current helper." << std::endl;
  std::cout << "'--view VIEWER' or '-v' Allows the user to choose the type of view he wants." << std::endl;
  std::cout << "Displays possbilites are: 0 - TERMINAL ; 1 - NCURSES ; 2 - GRAPHICAL ; (1 & 2 are currently in developement), default value is 1" << std::endl;
  exit(1);
}

void						Core::deleteAll(const std::string &path) const
{
  DIR						*dir;
  struct dirent					*elem;
  std::string					buf;
  struct stat					statbuf;

  if (path == "")
    {
      deleteAll(_filename);
      return;
    }
  dir = opendir(path.c_str());
  if (dir)
    {
      while ((elem = readdir(dir)))
	{
	  if (!strcmp(elem->d_name, ".") || !strcmp(elem->d_name, ".."))
	    continue;
	  buf = path + elem->d_name;
	  if (!stat(buf.c_str(), &statbuf))
	    {
	      if (S_ISDIR(statbuf.st_mode))
	      	deleteAll(buf.c_str());
	      else
	      	unlink(buf.c_str());
	    }
	}
      closedir(dir);
    }
  rmdir(path.c_str());
}

void						Core::argumentsHandler(const int ac, char** const& av)
{
  int						i;

  i = 0;
  while (++i != ac)
    if ((!strcmp(av[i], "--speed") || !strcmp(av[i], "-s")) && (av[i + 1] != NULL))
      _speed = atoi(av[++i]);
    else if ((!strcmp(av[i], "--timestamp_begin") || !strcmp(av[i], "-tb")) && (av[i + 1] != NULL))
      userParam.timestamp_start = atol(av[++i]);
    else if ((!strcmp(av[i], "--timestamp_end") || !strcmp(av[i], "-te")) && (av[i + 1] != NULL))
      userParam.timestamp_end = atol(av[++i]);
    else if ((!strcmp(av[i], "--component") || !strcmp(av[i], "-c")) && (av[i + 1] != NULL))
      userParam.component.push_back(av[++i]);
    else if ((!strcmp(av[i], "--severity") || !strcmp(av[i], "-e")) && (av[i + 1] != NULL))
      userParam.severity[atoi(av[++i])] = false;
    else if ((!strcmp(av[i], "--line") || !strcmp(av[i], "-l")) && (av[i + 1] != NULL))
      userParam.lineThreshold = atoi(av[++i]);
    else if (!strcmp(av[i], "--delete") || !strcmp(av[i], "-d"))
      this->deleteAll("");
    else if (!strcmp(av[i], "--nolaunch") || !strcmp(av[i], "-n"))
      userParam.quit = true;
    else if ((!strcmp(av[i], "--view") || !strcmp(av[i], "-v")) && (av[i + 1] != NULL))
      switch (atoi(av[++i]))
	{
	case 0:
	  _viewer = new TermViewer();
	  break;
	case 1:
	  _viewer = new CursesViewer();
	  break;
	default:
	  this->usage(av[0]);
	  break;
	}
    else if (!strcmp(av[i], "--help") || !strcmp(av[i], "-h"))
      this->usage(av[0]);
    else
      {
	std::cerr << "Unknown argument / parameter: " << av[i] << std::endl;
	this->usage(av[0]);
      }
}

void						Core::init()
{
  if (!userParam.quit)
    {
      mkdir(_filename.c_str(), 0755);
      _rotator.setFile(_filename + "logs.csv");
      _rotator.parse();
      if (_viewer == NULL)
	_viewer = new CursesViewer();
    }
}

void						Core::run()
{
  long						start;
  std::pair<std::vector<t_log>, int>		logs;
  std::vector<std::string>		 	files;
  std::vector<t_log>			 	tmp_logs;
  unsigned int					i;

  while (!userParam.quit)
    {
      start = TIMESTAMP;

      // First, we try to have the informations from the current log file
      if (!userParam.stop)
	{
	  _logs.setFile(_filename + "logs");
	  logs = _logs.getBuffer();
	  _data.logs = logs.first;
	  _logs.setFile();

	  // If we don't have enough, we start to search for other files
	  if (_data.logs.size() < LINENUMBER)
	    {
	      files = _rotator.getFileList();
	      i = -1;
	      while (_data.logs.size() < LINENUMBER && ++i != files.size())
		{
		  _logs.setFile(files[files.size() - (i + 1)]);
		  tmp_logs = _logs.getBuffer().first;
		  _logs.setFile();
		  tmp_logs.insert(tmp_logs.end(), _data.logs.begin(), _data.logs.end());
		  _data.logs = tmp_logs;
		}
	    }

	  // We keep only the number of line that we want
	  if (_data.logs.size() > LINENUMBER)
	    _data.logs.erase(_data.logs.begin(), _data.logs.begin() + (_data.logs.size() - LINENUMBER));
	}

      // We print the logs
      _viewer->displayHeader();
      _viewer->displayLogs(_data.logs);
      _viewer->refresh();

      // Rotation
      if (logs.second > 380)
	_rotator.rotate(_filename);
      _rotator.popLine(_filename, logs.second);
      // Game Loop
      _viewer->wait(start + _speed);
    }
}
