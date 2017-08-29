#ifndef							__DATA__
# define						__DATA__

# include						<vector>
# include						"Log.hpp"

# define						BLACK "\033[30m"
# define						RED "\033[31m"
# define						GREEN "\033[32m"
# define						YELLOW "\033[33m"
# define						BLUE "\033[34m"
# define						MAGENTA "\033[35m"
# define						CYAN "\033[36m"
# define						GREY "\033[37m"
# define						NEUTRAL "\033[0m"

# define						LINENUMBER	100

typedef	struct						s_flags
{
  bool							stop;
  bool							quit;
  long							timestamp_start;
  long							timestamp_end;
  std::vector<std::string>				component;
  bool							severity[6];
}							t_flags;

extern	t_flags						userParam;

typedef	struct						s_line
{
  std::string						name;
  std::pair<long, long>					timestamp;
  std::vector<std::string>				component;
  std::vector<int>					severity;
}							t_line;

typedef struct						s_log
{
  bool							valid;
  long							timestamp;
  std::string						severityStr;
  Severity						severity;
  int							processID;
  long							threadID;
  std::string						component;
  std::string						message;
}							t_log;

#endif	//						__DATA__
