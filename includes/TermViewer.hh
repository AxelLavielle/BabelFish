#ifndef				__TERMVIEWER__
# define			__TERMVIEWER__

# include			<iomanip>
# include			<iostream>
# include			"AViewer.hh"

class				TermViewer : public AViewer
{
public:
  TermViewer();
  virtual			~TermViewer();
  virtual void			displayLogs(const std::vector<t_log> &data);
  virtual void			refresh() const;
  virtual void			wait(const long ts);
  virtual void			displayHeader();

private:
  const std::string		logToString(const t_log &log) const;
  std::ostringstream		_logs;
};

#endif	//			__TERMVIEWER__
