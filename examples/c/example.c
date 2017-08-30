#include	"Log.h"

int		main(int ac, char **av)
{
  Log(INFO, "MYCOMPONENT", "%s-%d-", "What a test :", 42);
  return (0);
}
