#include			"CursesViewer.hh"

CursesViewer::CursesViewer()
{
  // Initialisation
  _save = ::newterm(getenv("TERM"), stdout, stdin); // Initialises the window
  ::clear(); // Clear the terminal
  ::cbreak(); // Allows to get char / char instead of waiting a carriage return
  ::noecho(); // Allows to not print the characters typed by the user
  ::curs_set(0); // Allows to remove the cursor display
  ::start_color(); // ALlows to start color mode
  ::mousemask(ALL_MOUSE_EVENTS, NULL);

  // Creation of the windows
  getmaxyx(::stdscr, _endY, _endX);
  _headerWindow = ::newwin(3, _endX, 0, 0);
  _logBorderWindow = ::newwin(_endY - 3, _endX, 3, 0);
  _logWindow = ::newpad(10000, _endX - 2);
  ::scrollok(_logWindow, TRUE); // Allows Scrolling
  ::idlok(_logWindow, TRUE); // Allows line insertion/deletion to be in normal terminal mode
  ::idcok(_logWindow, TRUE); // Allows char insertion/deletion to be in normal terminal mode
  ::refresh();

  // Display of the boxes
  ::box(_headerWindow, 0, 0);
  ::box(_logBorderWindow, 0, 0);
  ::wrefresh(_logBorderWindow);

  // Mouse & Key handling for all windows
  keypad(_headerWindow, TRUE);
  keypad(_logWindow, TRUE);
  _scroll = LINENUMBER - _logBorderWindow->_maxy + 1;
  signal(SIGINT, this->signalHandler); // FCK U MOUSE 220 COLUMN MAX

  // Color Init White Forground Colored Background
  ::init_pair(1, COLOR_WHITE, COLOR_BLACK);
  ::init_pair(2, COLOR_WHITE, COLOR_RED);
  ::init_pair(3, COLOR_WHITE, COLOR_GREEN);
  ::init_pair(4, COLOR_WHITE, COLOR_YELLOW);
  ::init_pair(5, COLOR_WHITE, COLOR_BLUE);
  ::init_pair(6, COLOR_WHITE, COLOR_MAGENTA);
  ::init_pair(7, COLOR_WHITE, COLOR_CYAN);
  
  // Color Init White Background Colored Foreground
  ::init_pair(8, COLOR_BLACK, COLOR_WHITE);
  ::init_pair(9, COLOR_RED, COLOR_WHITE);
  ::init_pair(10, COLOR_GREEN, COLOR_WHITE);
  ::init_pair(11, COLOR_YELLOW, COLOR_WHITE);
  ::init_pair(12, COLOR_BLUE, COLOR_WHITE);
  ::init_pair(13, COLOR_MAGENTA, COLOR_WHITE);
  ::init_pair(14, COLOR_CYAN, COLOR_WHITE);

  // Color Init Black Forground Colored Background
  ::init_pair(15, COLOR_BLACK, COLOR_WHITE);
  ::init_pair(16, COLOR_BLACK, COLOR_RED);
  ::init_pair(17, COLOR_BLACK, COLOR_GREEN);
  ::init_pair(18, COLOR_BLACK, COLOR_YELLOW);
  ::init_pair(19, COLOR_BLACK, COLOR_BLUE);
  ::init_pair(20, COLOR_BLACK, COLOR_MAGENTA);
  ::init_pair(21, COLOR_BLACK, COLOR_CYAN);
  
  // Color Init Black Background Colored Foreground
  ::init_pair(22, COLOR_WHITE, COLOR_BLACK);
  ::init_pair(23, COLOR_RED, COLOR_BLACK);
  ::init_pair(24, COLOR_GREEN, COLOR_BLACK);
  ::init_pair(25, COLOR_YELLOW, COLOR_BLACK);
  ::init_pair(26, COLOR_BLUE, COLOR_BLACK);
  ::init_pair(27, COLOR_MAGENTA, COLOR_BLACK);
  ::init_pair(28, COLOR_CYAN, COLOR_BLACK);
}

CursesViewer::~CursesViewer()
{
  ::delwin(_headerWindow);
  ::delwin(_logWindow);
  ::delwin(_logBorderWindow);
  ::endwin();
  ::delscreen(_save);
}

void				CursesViewer::printOneLog(int i, const t_log &log)
{
  std::ostringstream		oss;
  long				timestampForDate = log.timestamp / 1000;
  char				timestampAsStr[256];
  struct tm			*dt;
  int				column;

  column = 0;
  dt = localtime(&timestampForDate);
  std::strftime(timestampAsStr, sizeof(timestampAsStr), "%D %T.", dt);
  oss << " " <<
    //Timestamp
    timestampAsStr << std::setfill('0') << std::setw(3) << log.timestamp - 1000 * timestampForDate << " | ";
    //Severity
  oss.width(7);
  ::mvwprintw(_logWindow, i + _missingLines, 0, oss.str().c_str());
  column += oss.str().size();
  oss.str("");
  oss << std::setfill(' ') << std::left << log.severityStr;
    //ProcessID
  ::wattron(_logWindow, ((log.severity == TRACE) ? (GREENB)
			 : ((log.severity == DEBUG) ? (BLUEB)
			    : ((log.severity == INFO) ? (CYANB)
			       : ((log.severity == WARNING) ? (YELLOWB)
				  : ((log.severity == ERROR) ? (REDB) : (MAGENTAB)))))));
  ::mvwprintw(_logWindow, i + _missingLines, column, oss.str().c_str());
  ::wattroff(_logWindow, ((log.severity == TRACE) ? (GREENB)
			 : ((log.severity == DEBUG) ? (BLUEB)
			    : ((log.severity == INFO) ? (CYANB)
			       : ((log.severity == WARNING) ? (YELLOWB)
				  : ((log.severity == ERROR) ? (REDB) : (MAGENTAB)))))));
  column += oss.str().size();
  oss.str("");
  oss << " | ";
  oss.width(5);
  oss << std::setfill('0') << std::right << log.processID << " | "
    //ThreadID
      << "0x" << std::hex << log.threadID << " | ";
    //Component
  oss.width(12);
  oss << std::setfill(' ') << std::left << log.component
    //Message
      << " | " << log.message;
  ::mvwprintw(_logWindow, i + _missingLines, column, oss.str().c_str());
  _missingLines += (oss.str().size() + column) / (_logWindow->_maxx + 2);
}

void				CursesViewer::checkWindow()
{
  if (::stdscr->_maxy + 1 != _endY || ::stdscr->_maxx + 1 != _endX)
    {
      _scroll -= ::stdscr->_maxy + 1 - _endY;
      getmaxyx(::stdscr, _endY, _endX);
      ::wresize(_headerWindow, 3, _endX);
      ::wresize(_logBorderWindow, _endY - 3, _endX);
      ::wresize(_logWindow, 10000, _endX - 2);
      ::wclear(::stdscr);
      ::wclear(_headerWindow);
      ::wclear(_logBorderWindow);
      ::wclear(_logWindow);
      ::refresh();
      ::box(_headerWindow, 0, 0);
      ::box(_logBorderWindow, 0, 0);
      ::wrefresh(_logBorderWindow);
    }
}
void				CursesViewer::displayHeader()
{
  if (userParam.severity[0])
    ::wattron(_headerWindow, BGREEN);
  else
    ::wattron(_headerWindow, GREENB);
  ::mvwprintw(_headerWindow, 1, 2, "TRACE");
  if (userParam.severity[0])
    ::wattroff(_headerWindow, BGREEN);
  else
    ::wattroff(_headerWindow, GREENB);
  ::mvwprintw(_headerWindow, 1, 8, "|");
  if (userParam.severity[1])
    ::wattron(_headerWindow, BBLUE);
  else
    ::wattron(_headerWindow, BLUEB);
  ::mvwprintw(_headerWindow, 1, 10, "DEBUG");
  if (userParam.severity[1])
    ::wattroff(_headerWindow, BBLUE);
  else
    ::wattroff(_headerWindow, BLUEB);
  ::mvwprintw(_headerWindow, 1, 16, "|");
  if (userParam.severity[2])
    ::wattron(_headerWindow, BCYAN);
  else
    ::wattron(_headerWindow, CYANB);
  ::mvwprintw(_headerWindow, 1, 18, "INFO");
  if (userParam.severity[2])
    ::wattroff(_headerWindow, BCYAN);
  else
    ::wattroff(_headerWindow, CYANB);
  ::mvwprintw(_headerWindow, 1, 23, "|");
  if (userParam.severity[3])
    ::wattron(_headerWindow, BYELLOW);
  else
    ::wattron(_headerWindow, YELLOWB);
  ::mvwprintw(_headerWindow, 1, 25, "WARNING");
  if (userParam.severity[3])
    ::wattroff(_headerWindow, BYELLOW);
  else
    ::wattroff(_headerWindow, YELLOWB);
  ::mvwprintw(_headerWindow, 1, 33, "|");
  if (userParam.severity[4])
    ::wattron(_headerWindow, BRED);
  else
    ::wattron(_headerWindow, REDB);
  ::mvwprintw(_headerWindow, 1, 35, "ERROR");
  if (userParam.severity[4])
    ::wattroff(_headerWindow, BRED);
  else
    ::wattroff(_headerWindow, REDB);
  ::mvwprintw(_headerWindow, 1, 41, "|");
  if (userParam.severity[5])
    ::wattron(_headerWindow, BMAGENTA);
  else
    ::wattron(_headerWindow, MAGENTAB);
  ::mvwprintw(_headerWindow, 1, 43, "FATAL");
  if (userParam.severity[5])
    ::wattroff(_headerWindow, BMAGENTA);
  else
    ::wattroff(_headerWindow, MAGENTAB);
  ::mvwprintw(_headerWindow, 1, 41, "|");
}

void				CursesViewer::displayLogs(const std::vector<t_log> &data)
{
  unsigned short		i;

  ::wclear(_logWindow);
  i = -1;
  checkWindow();
  _missingLines = 0;
  ::wattron(_logWindow, WBLACK);
  while (++i != data.size())
    printOneLog(i, data[i]);
  _lineNumber = i;
  ::wattroff(_logWindow, WBLACK);
}

void				CursesViewer::refresh() const
{
  ::wrefresh(_headerWindow);
  ::prefresh(_logWindow, _scroll + _missingLines, 0, 4, 1, _logBorderWindow->_maxy + 2, 10000);
}

void				CursesViewer::signalHandler(const int signum) //Catch of sigint because of the mouse
{
  unsigned char			c;

  ::timeout(0);
  c = ::getch();
  if (c == 255)
    userParam.quit = true;
  (void)signum;
}

void				CursesViewer::wait(const long ts)
{
  unsigned char			c[6];
  bool				first;

  first = true;
  if (_scroll > _lineNumber - (_logBorderWindow->_maxy - 1))
    {
      _scroll = _lineNumber - (_logBorderWindow->_maxy - 1);
      if (_scroll < 0)
  	_scroll = 0;
      this->refresh();
    }
  while (ts > TIMESTAMP || first)
    {
      //initialise
      c[0] = 255;
      c[1] = 255;
      c[2] = 255;
      c[3] = 255;
      c[4] = 255;
      c[5] = 255;

      first = false;
      ::timeout((ts - TIMESTAMP > 100 ? ts - TIMESTAMP : 0));
      c[0] = ::getch();
      ::timeout(0);
      switch (c[0])
      	{
      	case 27: // SPECIAL KEY
	  c[1] = ::getch();
      	  switch (c[1])
      	    {
      	    case 91: // ARROW KEY & MOUSE
	      c[2] = ::getch();
      	      switch (c[2])
      		{
      		case 65: // UP
      		  if (_scroll > 0)
      		    {
      		      --_scroll;
		      this->refresh();
      		    }
      		  break;
      		case 66: // DOWN
      		  if (_scroll < _lineNumber + _missingLines - (_logBorderWindow->_maxy - 1))
      		    {
      		      ++_scroll;
		      this->refresh();
      		    }
      		  break;
      		case 67: // RIGHT
      		  break;
      		case 68: // LEFT
      		  break;
      		case 77: // MOUSECLIC POSITIONS are (x, y) (c[4], c[5])
		  c[3] = ::getch();
		  c[4] = ::getch();
		  c[5] = ::getch();
      		  switch (c[3])
      		    {
      		    case 32: // LEFT
		      if (c[5] <= 35 && c[5] >= 33)
			{
			  if (c[4] >= 34 && c[4] <= 40)
			    userParam.severity[0] = !userParam.severity[0];
			  else if (c[4] >= 42 && c[4] <= 48)
			    userParam.severity[1] = !userParam.severity[1];
			  else if (c[4] >= 50 && c[4] <= 55)
			    userParam.severity[2] = !userParam.severity[2];
			  else if (c[4] >= 57 && c[4] <= 65)
			    userParam.severity[3] = !userParam.severity[3];
			  else if (c[4] >= 67 && c[4] <= 73)
			    userParam.severity[4] = !userParam.severity[4];
			  else if (c[4] >= 75 && c[4] <= 81)
			    userParam.severity[5] = !userParam.severity[5];
			  else
			    break;
			  _scroll = LINENUMBER;
			  return;
			}
      		      break;
      		    case 33: // MIDDLE
      		      break;
      		    case 34: // RIGHT
      		      break;
      		    case 35: // RELEASE
      		      break;
      		    case 96: // SCROLL UP
      		      if (_scroll > -_missingLines)
      			{
      			  --_scroll;
      			  return;
      			}
      		      break;
      		    case 97: // SCROLL DOWN
      		      if (_scroll < _lineNumber - _logBorderWindow->_maxy + 1)
      			{
      			  ++_scroll;
      			  return;
      			}
      		      break;
      		    default:
      		      break;
      		    }
      		  break;
      		default:
      		  break;
      		}
      	      break;
      	    default:
      	      userParam.quit = true;
      	      return;
      	      break;
      	    }
      	  break;
	case 32:
	  userParam.stop = !userParam.stop;
	  break;
	case 113:
	  userParam.quit = true;
	  return;
	  break;
	case 115:
	  userParam.stop = !userParam.stop;
      	default:
      	  break;
      	}
    }
  std::cout << "I'm done with key getters" << std::endl;
}
