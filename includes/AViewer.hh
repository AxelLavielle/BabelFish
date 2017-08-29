#ifndef				__AVIEWER__
# define			__AVIEWER__

# include			"IViewer.hpp"

class				AViewer : public IViewer
{
public:
  AViewer();
  virtual			~AViewer();
  virtual void			displayLogs(const std::vector<t_log> &data) = 0;
  virtual void			refresh() const = 0;
  virtual void			wait(const long ts) = 0;
  virtual void			displayHeader() = 0;

protected:
};

#endif	//			__AVIEWER__
