#include <unistd.h>
#include <signal.h>
#include <syscall.h>
#include <fcntl.h>

int chown(const char *path, uid_t uid, gid_t gid)
{
#ifdef  SYS_chown
	return syscall(SYS_chown, path, uid, gid);
#else
	return syscall(SYS_fchownat, AT_FDCWD, path, uid, gid, 0);
#endif
}

