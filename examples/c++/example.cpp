#include	"../../libs/c++/Log.hpp"

int	main(int ac, char **av)
{
  LOG("LOESS", TRACE) << "Hello World !";
  LOG("LOESS", DEBUG) << "Hello World !";
  LOG("LOESS", INFO) << "Hello World !";
  LOG("LOESS", WARNING) << "Hello World !";
  LOG("LOESS", ERROR) << "Hello World !";
  LOG("LOESS", FATAL) << "Hello World !";
}
