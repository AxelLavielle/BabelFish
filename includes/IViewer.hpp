#ifndef				__IVIEWER__
# define			__IVIEWER__

# include			<string>
# include			<vector>
# include			"Data.hh"

class				IViewer
{
public:
  virtual			~IViewer() {};
  virtual void			displayLogs(const std::vector<t_log> &data) = 0;
  virtual void			refresh() const = 0;
  virtual void			wait(const long ts) = 0;
  virtual void			displayHeader() = 0;
};

#endif	//			__IVIEWER__
