#ifndef				__CURSESVIEWER__
# define			__CURSESVIEWER__

# include			<iomanip>
# include			<curses.h>
# include			<ncurses.h>
# include			<csignal>
# include			"AViewer.hh"

// White Forground Colored Background
# define			WBLACK		COLOR_PAIR(1)
# define			WRED		COLOR_PAIR(2)
# define			WGREEN		COLOR_PAIR(3)
# define			WYELLOW		COLOR_PAIR(4)
# define			WBLUE		COLOR_PAIR(5)
# define			WMAGENTA	COLOR_PAIR(6)
# define			WCYAN		COLOR_PAIR(7)

// White Background Colored Foreground
# define			BLACKW		COLOR_PAIR(8)
# define			REDW		COLOR_PAIR(9)
# define			GREENW		COLOR_PAIR(10)
# define			YELLOWW		COLOR_PAIR(11)
# define			BLUEW		COLOR_PAIR(12)
# define			MAGENTAW	COLOR_PAIR(13)
# define			CYANW		COLOR_PAIR(14)

// Black Forground Colored Background
# define			BWHITE		COLOR_PAIR(15)
# define			BRED		COLOR_PAIR(16)
# define			BGREEN		COLOR_PAIR(17)
# define			BYELLOW		COLOR_PAIR(18)
# define			BBLUE		COLOR_PAIR(19)
# define			BMAGENTA	COLOR_PAIR(20)
# define			BCYAN		COLOR_PAIR(21)

// Black Background Colored Foreground
# define			WHITEB		COLOR_PAIR(22)
# define			REDB		COLOR_PAIR(23)
# define			GREENB		COLOR_PAIR(24)
# define			YELLOWB		COLOR_PAIR(25)
# define			BLUEB		COLOR_PAIR(26)
# define			MAGENTAB	COLOR_PAIR(27)
# define			CYANB		COLOR_PAIR(28)  

class				CursesViewer : public AViewer
{
public:
  CursesViewer();
  virtual			~CursesViewer();
  virtual void			displayLogs(const std::vector<t_log> &data);
  virtual void			refresh() const;
  virtual void			wait(const long ts);
  virtual void			displayHeader();
  static void			signalHandler(const int signum);

private:
  void				printOneLog(int i, const t_log &log);
  void				checkWindow();
  ::WINDOW			*_headerWindow;
  ::WINDOW			*_logWindow;
  ::WINDOW			*_logBorderWindow;
  ::SCREEN			*_save;
  int				_endX;
  int				_endY;
  int				_scroll;
  int				_lineNumber;
  unsigned short		_missingLines;
};

#endif	//			__CURSESVIEWER__
