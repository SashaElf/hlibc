#include <sys/syscall.h>
#include "../internal/internal.h"

int execve(const char *path, char *const argv[], char *const envp[])
{
        return syscall(SYS_execve, path, argv, envp);
}

