#include			"Rotator.hh"

Rotator::Rotator()
{
}

Rotator::~Rotator()
{
}

void				Rotator::setFile(const std::string &filename)
{
  _file.open(filename.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::app);
  if (!_file.is_open())
    {
      std::cerr << "Couldn't open the file '" << filename << "' please verify that the file exists and that you have the rights." << std::endl;
      exit(1);
    }
}

const std::vector<std::string>	Rotator::extractSep(const std::string &line, const char separator) const
{
  std::vector<std::string>	ret;
  std::size_t			bef;
  std::size_t			aft;

  bef = 0;
  while ((aft = line.find(separator, bef + 1)) != std::string::npos)
    {
      ret.push_back(line.substr(bef == 0 ? bef : (bef + 1), aft - (bef == 0 ? bef : (bef + 1))));
      bef = aft;
    }
  ret.push_back(line.substr(bef, line.length() - bef));
  return (ret);
}

void				Rotator::parse()
{
  std::string			line;
  std::vector<std::string>	cells;
  std::vector<std::string>	tmp;
  std::vector<int>		severity;
  unsigned char			i;
    
  _file.clear();
  _file.seekg(std::ios::beg);
  while (std::getline(_file, line))
    {
      t_line			extracted;
      
      cells = extractSep(line, ',');
      extracted.name = cells[0];
      try {extracted.timestamp.first = std::stol(cells[1]);} catch (...) {extracted.timestamp.first = 0;}
      try {extracted.timestamp.second = std::stol(cells[2]);} catch (...) {extracted.timestamp.second = 0;}
      extracted.component = extractSep(cells[3], '|');
      tmp = extractSep(cells[4], '|');
      i = -1;
      while (++i != tmp.size())
	try {extracted.severity.push_back(std::stoi(tmp[i]));} catch (...) {extracted.severity.push_back(1);}
      _line.push_back(extracted);
    }
  _file.close();
}

const std::string		Rotator::lineToString(const t_line &line) const
{
  std::ostringstream		oss;
  unsigned char			c;

  oss << line.name << ','
      << line.timestamp.first << ','
      << line.timestamp.second << ',';
  c = -1;
  while (++c != line.component.size())
    oss << ((c != 0) ? ("|") : ("")) << line.component[c];
  oss << ',';
  c = -1;
  while (++c != line.severity.size())
    oss << ((c != 0) ? ("|") : ("")) << line.severity[c];
  oss << std::endl;
  return (oss.str());
}

int				Rotator::checkTotalLine(const int nline) const
{
  int				i;
  int				tline;

  if (userParam.lineThreshold == -1)
    return (0);
  tline = nline;
  i = -1;
  while (tline < userParam.lineThreshold && ++i != static_cast<int>(_line.size()))
    tline += _line[i].severity[0] + _line[i].severity[1] + _line[i].severity[2] + _line[i].severity[3] + _line[i].severity[4] + _line[i].severity[5];
  return (_line.size() - (i + 1));
}

void				Rotator::popLine(const std::string &path, int nline)
{
  std::vector<std::string>	buffer;
  int				nToRemove;

  if ((nToRemove = this->checkTotalLine(nline)) == 0)
    return;
  while (nToRemove-- != 0)
    {
      unlink(_line[nToRemove].name.c_str());
      _line.erase(_line.begin() + nToRemove);
    }
  _file.open(path + "logs.csv", std::fstream::out | std::fstream::trunc);
  while (++nToRemove != static_cast<int>(_line.size()))
    if (nToRemove == 0)
      _file << this->lineToString(_line[nToRemove]);
    else
      _file << std::endl << this->lineToString(_line[nToRemove]);
  _file.close();
}

void				Rotator::rotate(const std::string &path)
{
  std::string			oldname;
  std::string			newname;
  int				id;
  std::ostringstream		oss;

  // move the file in its new name
  oldname = path + "logs";
  newname = path + "logs.";
  id = -1;
  if (_line.size() != 0)
    try {id = std::stoi(_line[_line.size() - 1].name.substr(_line[_line.size() - 1].name.find_last_of('.') + 1, _line.size() - _line[_line.size() - 1].name.find('.') - 1));} catch (...) {;}
  oss << std::setfill('0') << std::setw(5) << ++id;
  newname += oss.str();
  rename(oldname.c_str(), newname.c_str());

  // log the stats of the old file (filename - timestampBeginning - timestampEnd - components - num msg /  severity)
  LogParser			logs;
  t_line			infos;

  logs.setFile(newname);
  infos = logs.getInfo();
  logs.setFile();

  std::ofstream			file;
  oldname += ".csv";
  file.open(oldname.c_str(), std::ios_base::app);
  file << this->lineToString(infos);
  file.close();
  _line.push_back(infos);
}

const std::vector<std::string>	Rotator::getFileList() const
{
  std::vector<std::string>	ret;
  unsigned int			i;
  unsigned int			j;
  unsigned int			k;
  bool				valid;

  i = -1;
  while (++i < _line.size())
    {
      valid = true;
      j = 0;
      while (j != DEFAULT && (userParam.severity[j] == false || _line[i].severity[j] == 0))
	j++;
      if (j == DEFAULT ||
	  (userParam.timestamp_start != -1 && userParam.timestamp_start >= _line[i].timestamp.second) ||
	  (userParam.timestamp_end != -1 && userParam.timestamp_end <= _line[i].timestamp.first))
	valid = false;
      k = 0;
      j = 0;
      while (userParam.component.size() != 0 && k != _line[i].component.size() && _line[i].component[k] != userParam.component[j++])
	if (j == userParam.component.size())
	  j = k++ * 0;
      if (k == _line[i].component.size())
	valid = false;
      if (valid)
	ret.push_back(_line[i].name);
    }
  return (ret);
}
