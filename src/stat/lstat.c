#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int lstat(const char *restrict path, struct stat *restrict buf)
{
#ifdef SYS_lstat
        return syscall(SYS_lstat, path, buf);
#else
        return syscall(SYS_fstatat, AT_FDCWD, path, buf, AT_SYMLINK_NOFOLLOW);
#endif
}


