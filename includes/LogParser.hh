#ifndef							__LOGPARSER__
# define						__LOGPARSER__

# include						"Data.hh"

class							LogParser
{
public:
  LogParser();
  ~LogParser();

  void							setFile();
  void							setFile(const std::string &filename);
  const std::pair<std::vector<t_log>, int>		getBuffer();
  const t_line						getInfo();

private:
  const std::string					extractObject(const std::string &bef, const std::string &aft, const int pos, const std::string &str) const;
  const t_log						parseLine(const std::string &line) const;
  void							warning(const std::string &line) const;
  bool							validateLine(const t_log &log) const;
  const t_line						statInfo() const;
  const t_line						statInfo(t_line info, const t_log &log);

  std::string						_filename;
  std::ifstream						_file;
  t_flags						_flags;
  std::vector<t_log>					_buffer;
};

#endif	//						__LOGPARSER__
