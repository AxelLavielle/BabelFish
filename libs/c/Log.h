#ifndef			__LOG__
# define		__LOG__

# include		<unistd.h>
# include		<stdlib.h>
# include		<stdio.h>
# include		<stdarg.h>
# include		<string.h>
# include		<sys/syscall.h>
# include		<sys/stat.h>
# include		<sys/types.h>
# include		<fcntl.h>
# include		<pwd.h>

# define		TRACE		0
# define		DEBUG		1
# define		INFO		2
# define		WARNING		3
# define		ERROR		4
# define		FATAL		5
# define		DEFAULT		6

# define		LOGDIR		"/.logs"
# define		LOGFILE		"/.logs/logs"
# define		TIMESTAMP	10

char			*strkitten(char *str, char *str2)
{
  char			*new;
  short			i;
  short			j;

  if ((new = malloc(strlen(str) + strlen(str2) + 1)) == NULL)
    exit(printf("Error in memory allocation.\n") * 0 - 1);

  i = -1;
  while (str[++i])
    new[i] = str[i];
  j = -1;
  while (str2[++j])
    new[i + j] = str2[j];
  new[i + j] = 0;
  return (new);
}

char			*___getFile__()
{
  static char		*filepath = NULL;
  char			*tmp;

  if (filepath == NULL)
    {
      filepath = strkitten(getpwuid(getuid())->pw_dir, LOGFILE);
      tmp = strkitten(getpwuid(getuid())->pw_dir, LOGDIR);
      mkdir(tmp, 0755);
      free(tmp);
    }
  return (filepath);
}

void			Log(int severity, char *component, char *format, ...)
{
  va_list		ap;
  long			timestamp;
  struct timeval	tv;
  const	char		*sev;
  pid_t			processID;
  pid_t			threadID;
  char			*message;
  char			*logmessage;
  int			fd;

  // TIMESTAMP
  gettimeofday(&tv, NULL);
  timestamp = tv.tv_sec * 1000 + tv.tv_usec / 1000;

  // MESSAGE
  va_start(ap, format);
  vasprintf(&message, format, ap);
  va_end(ap);
  if (!message[0])
    {
      message = strdup("Wrong message, maybe bad formatting or bad arguments");
      severity = ERROR;
    }

  // SEVERITY
  switch (severity)
    {
    case TRACE:
      sev = "TRACE";
      break;
    case DEBUG:
      sev = "DEBUG";
      break;
    case INFO:
      sev = "INFO";
      break;
    case WARNING:
      sev = "WARNING";
      break;
    case ERROR:
      sev = "ERROR";
      break;
    case FATAL:
      sev = "FATAL";
      break;
    default:
      sev = "ERROR";
      message = strdup("Bad severity, please choose between the following: TRACE DEBUG INFO WARNING ERROR FATAL");
      break;
    }

  // PROCESSID
  processID = getpid();

  // THREADID
  threadID = syscall(SYS_gettid);

  // FORMAT LOGMESSAGE
  if (component != "")
    asprintf(&logmessage, "[%ld][%s][%d][%d][%s]--%s--\n", timestamp, sev, processID, threadID, component, message);
  else
    asprintf(&logmessage, "[%ld][%s][%d][%d][%s]--%s--\n", timestamp, "ERROR", processID, threadID, "Logger", "Bad component, please enter the name of you program");
  free(message);

  // WRITE IT
  fd = open(___getFile__(), O_WRONLY | O_APPEND | O_CREAT, 0644);
  write(fd, logmessage, strlen(logmessage));
  close(fd);
  free(logmessage);
}


#endif		//	__LOG__
