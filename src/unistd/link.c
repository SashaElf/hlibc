#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>
int link(const char *existing, const char *new)
{
#ifdef  SYS_link
	return syscall(SYS_link, existing, new);
#else
	return syscall(SYS_linkat, AT_FDCWD, existing, new);
#endif
}

