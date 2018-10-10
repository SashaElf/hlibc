#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

extern char **__environ;

long __syscall_ret(unsigned long);


struct __DIR_s
{
	int fd;
	off_t tell;
	int buf_pos;
	int buf_end;
	int lock[2];
	char buf[2048];
};

off_t lseek(int fd, off_t offset, int whence)
{
#ifdef SYS__llseek
	off_t result;
	return __syscall(SYS__llseek, fd, offset>>32, offset, &result, whence) ? -1 : result;
#else
	return __syscall(SYS_lseek, fd, offset, whence);
#endif
}

pid_t tcgetpgrp(int fd)
{
	int pgrp;
	if (ioctl(fd, TIOCGPGRP, &pgrp) < 0)
		return -1;
	return pgrp;
}

int tcsetpgrp(int fd, pid_t pgrp)
{
	int pgrp_int = pgrp;
	return ioctl(fd, TIOCSPGRP, &pgrp_int);
}

int closedir(DIR *dir)
{
	int ret = close(dir->fd);
	free(dir);
	return ret;
}

int getdents(int fd, struct dirent *buf, size_t len)
{
	return __syscall(SYS_getdents, fd, buf, len);
}

DIR *opendir(const char *name)
{
	int fd;
	DIR *dir;

	if ((fd = open(name, O_RDONLY|O_DIRECTORY|O_CLOEXEC)) < 0)
		return 0;
	if (!(dir = calloc(1, sizeof *dir))) {
		__syscall(SYS_close, fd);
		return 0;
	}
	dir->fd = fd;
	return dir;
}

struct dirent *readdir(DIR *dir)
{
	struct dirent *de;
	
	if (dir->buf_pos >= dir->buf_end) {
		int len = getdents(dir->fd, (void *)dir->buf, sizeof dir->buf);
		if (len <= 0) 
			return 0;
		dir->buf_end = len;
		dir->buf_pos = 0;
	}
	de = (void *)(dir->buf + dir->buf_pos);
	dir->buf_pos += de->d_reclen;
	dir->tell = de->d_off;
	return de;
}

int clearenv()
{
	__environ[0] = 0;
	return 0;
}

char *getenv(const char *name)
{
	int i;
	size_t l = strlen(name);
	if (!__environ || !*name || strchr(name, '='))
		return NULL;
	for (i=0; __environ[i] && (strncmp(name, __environ[i], l)
		|| __environ[i][l] != '='); i++);
	if (__environ[i])
		return __environ[i] + l+1;
	return NULL;
}

void abort(void)
{
	//raise(SIGABRT);
	for (;;);
}

void __assert_fail(const char *expr, const char *file, int line, const char *func)
{
	fprintf(stderr, "Assertion failed: %s (%s: %s: %d)\n", expr, file, func, line);
	fflush(NULL);
	abort();
}

int raise(int sig)
{ 
	return sig;
}

int fcntl(int fd, int cmd, ...)
{
	long arg;
	va_list ap;
	va_start(ap, cmd);
	arg = va_arg(ap, long);
	va_end(ap);
	if (cmd == F_SETFL)
		arg |= O_LARGEFILE;
	if (cmd == F_SETLKW)
		return __syscall(SYS_fcntl, fd, cmd, arg);
	if (cmd == F_GETOWN)
		return __syscall(SYS_fcntl, fd, cmd, arg);
	return __syscall(SYS_fcntl, fd, cmd, arg);
}

int open(const char *filename, int flags, ...)
{

	mode_t mode;
	va_list ap;
	va_start(ap, flags);
	mode = va_arg(ap, mode_t);
	va_end(ap);
#ifdef SYS_open
	return __syscall(SYS_open, filename, flags|O_LARGEFILE, mode);
#else
	return __syscall(SYS_openat, AT_FDCWD, filename, flags|O_LARGEFILE, mode);
#endif
}

void *mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
	void *ret;
	if (sizeof(off_t) > sizeof(long))
		if (((long)off & 0xfff) | ((long)((unsigned long long)off>>(12 + 8*(sizeof(off_t)-sizeof(long))))))
			start = (void *)-1;
#ifdef SYS_mmap2
	ret = (void *)__syscall(SYS_mmap2, start, len, prot, flags, fd, off>>12);
#else
	ret = (void *)__syscall(SYS_mmap, start, len, prot, flags, fd, off);
#endif
	return ret;
}

int execv(const char *path, char *const argv[])
{
	return execve(path, argv, __environ);
}

int execve(const char *path, char *const argv[], char *const envp[])
{
	return __syscall(SYS_execve, path, argv, envp);
}

int execvp(const char *file, char *const argv[])
{
	const char *p, *z, *path = getenv("PATH");
	size_t l, k;

	errno = ENOENT;
	if (!*file)
		return -1;

	if (strchr(file, '/'))
		return execv(file, argv);

	if (!path)
		path = "/usr/local/bin:/bin:/usr/bin";
	k = strnlen(file, NAME_MAX+1);
	if (k > NAME_MAX) {
		errno = ENAMETOOLONG;
		return -1;
	}
	l = strnlen(path, PATH_MAX-1)+1;

	for(p=path; ; p=z) {
		char b[l+k+1];
		z = strchr(p, ':');
		if (!z)
			z = p+strlen(p);
		if (z-p >= l) {
			if (!*z++)
				break;
			continue;
		}
		memcpy(b, p, z-p);
		b[z-p] = '/';
		memcpy(b+(z-p)+(z>p), file, k+1);
		execv(b, argv);
		if (errno != ENOENT)
			return -1;
		if (!*z++)
			break;
	}
	return -1;
}

int poll(struct pollfd *fds, nfds_t n, int timeout)
{
	
#ifdef SYS_poll
	return __syscall(SYS_poll, fds, n, timeout);
#else
	return __syscall(SYS_ppoll, fds, n, timeout>=0 ?
		&((struct timespec){ .tv_sec = timeout/1000,
		.tv_nsec = timeout%1000*1000000 }) : 0, 0, _NSIG/8);
#endif
}

long __syscall_ret(unsigned long r)
{
	if (r > -4096UL) {
		errno = -r;
		return -1;
	}
	return r;
}

#undef syscall
long syscall(long n, ...)
{
	va_list ap;
	long a, b, c, d, e, f;
	va_start(ap, n);
	a=va_arg(ap, long);
	b=va_arg(ap, long);
	c=va_arg(ap, long);
	d=va_arg(ap, long);
	e=va_arg(ap, long);
	f=va_arg(ap, long);
	va_end(ap);
	return __syscall_ret(__syscall(n, a, b, c, d, e, f));
}

int clock_gettime(clockid_t clk, struct timespec *ts)
{ 
	int r = __syscall(SYS_clock_gettime, clk, ts);
	if (!r) return r;
	if (r == -ENOSYS) {
		if (clk == CLOCK_REALTIME) {
			__syscall(SYS_gettimeofday, clk, ts, 0);
			ts->tv_nsec = (int)ts->tv_nsec * 1000;
			return 0;
		}
		r = -EINVAL;
	}
	errno = -r;
	return -1;
}

time_t time(time_t *t)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	if (t) *t = ts.tv_sec;
	return ts.tv_sec;
}

