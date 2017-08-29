/**
 ** Copyright (c) 2017 Scortex SAS
 */

#include	"Core.hh"

t_flags		userParam;

int		main(int ac, char **av)
{
  Core		core;

  core.argumentsHandler(ac, av);
  core.init();
  core.run();
  return (0);
}
