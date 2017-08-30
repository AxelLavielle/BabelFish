#include	"Log.h"

int		main(int ac, char **av)
{
  Log(TRACE, "LOESS", "%s", "Hello World !");
  Log(DEBUG, "LOESS", "%s", "Hello World !");
  Log(INFO, "LOESS", "%s", "Hello World !");
  Log(WARNING, "LOESS", "%s", "Hello World !");
  Log(ERROR, "LOESS", "%s", "Hello World !");
  Log(FATAL, "LOESS", "%s", "Hello World !");
  return (0);
}
