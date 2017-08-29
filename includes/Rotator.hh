#ifndef					__ROTATOR__
# define				__ROTATOR__

# include				<iomanip>
# include				"LogParser.hh"

class					Rotator
{
public:
  Rotator();
  ~Rotator();
  void					setFile(const std::string &filename);
  void					parse();
  void					rotate(const std::string &path);
  const std::vector<std::string>	getFileList() const;

private:
  const std::vector<std::string>	extractSep(const std::string &line, const char separator) const;
  const std::string			lineToString(const t_line &line) const;

  std::fstream				_file;
  std::vector<t_line>			_line;
};

#endif	//				__ROTATOR__
