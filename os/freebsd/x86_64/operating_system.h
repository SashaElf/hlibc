;		event should be #defined to AUE_NULL in audit_kevents.h.
;	type	one of STD, OBSOL, UNIMPL, COMPAT, COMPAT4, COMPAT6,
;		COMPAT7, COMPAT11, NODEF, NOARGS, NOPROTO, NOSTD
;		The COMPAT* options may be combined with one or more NO*
;		options separated by '|' with no spaces (e.g. COMPAT|NOARGS)
;	name	pseudo-prototype of syscall routine
;		If one of the following alts is different, then all appear:
;	altname	name of system call if different
;	alttag	name of args struct tag if different from [o]`name'"_args"
;	altrtyp	return type if not int (bogus - syscalls always return int)
;		for UNIMPL/OBSOL, name continues with comments

; types:
;	STD	always included
;	COMPAT	included on COMPAT #ifdef
;	COMPAT4	included on COMPAT_FREEBSD4 #ifdef (FreeBSD 4 compat)
;	COMPAT6	included on COMPAT_FREEBSD6 #ifdef (FreeBSD 6 compat)
;	COMPAT7	included on COMPAT_FREEBSD7 #ifdef (FreeBSD 7 compat)
;	COMPAT10 included on COMPAT_FREEBSD10 #ifdef (FreeBSD 10 compat)
;	COMPAT11 included on COMPAT11 #ifdef (FreeBSD 11 compat)
;	OBSOL	obsolete, not included in system, only specifies name
;	UNIMPL	not implemented, placeholder only
;	NOSTD	implemented but as a lkm that can be statically
;		compiled in; sysent entry will be filled with lkmressys
;		so the SYSCALL_MODULE macro works
;	NOARGS	same as STD except do not create structure in sys/sysproto.h
;	NODEF	same as STD except only have the entry in the syscall table
;		added.  Meaning - do not create structure or function
;		prototype in sys/sysproto.h
;	NOPROTO	same as STD except do not create structure or
;		function prototype in sys/sysproto.h.  Does add a
;		definition to syscall.h besides adding a sysent.
;	NOTSTATIC syscall is loadable

; annotations:
;	SAL 2.0 annotations are used to specify how system calls treat
;	arguments that are passed using pointers. There are three basic
;	annotations.
;
;	_In_    Object pointed to will be read and not modified.
;	_Out_   Object pointed to will be written and not read.
;	_Inout_ Object pointed to will be written and read.
;
;	These annotations are used alone when the pointer refers to a single
;	object i.e. scalar types, structs, and pointers, and not NULL. Adding
;	the _opt_ suffix, e.g. _In_opt_, implies that the pointer may also
;	refer to NULL.
;
;	For pointers to arrays, additional suffixes are added:
;
;	_In_z_, _Out_z_, _Inout_z_:
;	    for a NUL terminated array e.g. a string.
;	_In_reads_z_(n),_Out_writes_z_(n), _Inout_updates_z_(n):
;	    for a NUL terminated array e.g. a string, of known length n bytes.
;	_In_reads_(n),_Out_writes_(n),_Inout_updates_(n):
;	    for an array of n elements.
;	_In_reads_bytes_(n), _Out_writes_bytes_(n), _Inout_updates_bytes(n):
;	    for a buffer of n-bytes.

; Please copy any additions and changes to the following compatability tables:
; sys/compat/freebsd32/syscalls.master

; #ifdef's, etc. may be included, and are copied to the output files.

#include <sys/param.h>
#include <sys/sysent.h>
#include <sys/sysproto.h>

; Reserved/unimplemented system calls in the range 0-150 inclusive
; are reserved for use in future Berkeley releases.
; Additional system calls implemented in vendor and other
; redistributions should be placed in the reserved range at the end
; of the current calls.

0	AUE_NULL	STD	{ int nosys(void); } syscall nosys_args int
1	AUE_EXIT	STD	{ void sys_exit(int rval); } exit \
				    sys_exit_args void
2	AUE_FORK	STD	{ int fork(void); }
3	AUE_READ	STD	{ ssize_t read(int fd, \
				    _Out_writes_bytes_(nbyte) void *buf, \
				    size_t nbyte); }
4	AUE_WRITE	STD	{ ssize_t write(int fd, \
				    _In_reads_bytes_(nbyte) const void *buf, \
				    size_t nbyte); }
5	AUE_OPEN_RWTC	STD	{ int open( \
				    _In_z_ char *path, \
				    int flags, \
				    int mode); }
; XXX should be		{ int open(const char *path, int flags, ...); }
; but we're not ready for `const' or varargs.
; XXX man page says `mode_t mode'.
6	AUE_CLOSE	STD	{ int close(int fd); }
7	AUE_WAIT4	STD	{ int wait4(int pid, \
				    _Out_opt_ int *status, \
				    int options, \
				    _Out_opt_ struct rusage *rusage); }
8	AUE_CREAT	COMPAT	{ int creat(_In_z_ char *path, int mode); }
9	AUE_LINK	STD	{ int link(_In_z_ char *path, \
				    _In_z_ char *link); }
10	AUE_UNLINK	STD	{ int unlink(_In_z_ char *path); }
11	AUE_NULL	OBSOL	execv
12	AUE_CHDIR	STD	{ int chdir(_In_z_ char *path); }
13	AUE_FCHDIR	STD	{ int fchdir(int fd); }
14	AUE_MKNOD	COMPAT11 { int mknod(_In_z_ char *path, int mode, \
				    int dev); }
15	AUE_CHMOD	STD	{ int chmod(_In_z_ char *path, int mode); }
16	AUE_CHOWN	STD	{ int chown(_In_z_ char *path, \
				    int uid, int gid); }
17	AUE_NULL	STD	{ caddr_t break(_In_ char *nsize); }
18	AUE_GETFSSTAT	COMPAT4	{ int getfsstat( \
				    _Out_writes_bytes_opt_(bufsize) \
				    struct ostatfs *buf, \
				    long bufsize, int mode); }
19	AUE_LSEEK	COMPAT	{ long lseek(int fd, long offset, \
				    int whence); }
20	AUE_GETPID	STD	{ pid_t getpid(void); }
21	AUE_MOUNT	STD	{ int mount(_In_z_ char *type, \
				    _In_z_ char *path, int flags, \
				    _In_opt_ caddr_t data); }
; XXX `path' should have type `const char *' but we're not ready for that.
22	AUE_UMOUNT	STD	{ int unmount(_In_z_ char *path, int flags); }
23	AUE_SETUID	STD	{ int setuid(uid_t uid); }
24	AUE_GETUID	STD	{ uid_t getuid(void); }
25	AUE_GETEUID	STD	{ uid_t geteuid(void); }
26	AUE_PTRACE	STD	{ int ptrace(int req, pid_t pid, \
				    _Inout_opt_ caddr_t addr, int data); }
27	AUE_RECVMSG	STD	{ int recvmsg(int s, \
				    _Inout_ struct msghdr *msg, int flags); }
28	AUE_SENDMSG	STD	{ int sendmsg(int s, _In_ struct msghdr *msg, \
				    int flags); }
29	AUE_RECVFROM	STD	{ int recvfrom(int s, \
				    _Out_writes_bytes_(len) caddr_t buf, \
				    size_t len, int flags, \
				    _Out_writes_bytes_opt_(*fromlenaddr) \
				    struct sockaddr * __restrict from, \
				    _Inout_opt_ \
				    __socklen_t * __restrict fromlenaddr); }
30	AUE_ACCEPT	STD	{ int accept(int s, \
				    _Out_writes_bytes_opt_(*anamelen) \
				    struct sockaddr * __restrict name, \
				    _Inout_opt_ \
				    __socklen_t * __restrict anamelen); }
31	AUE_GETPEERNAME	STD	{ int getpeername(int fdes, \
				    _Out_writes_bytes_(*alen) \
				    struct sockaddr * __restrict asa, \
				    _Inout_opt_ \
				    __socklen_t * __restrict alen); }
32	AUE_GETSOCKNAME	STD	{ int getsockname(int fdes, \
				    _Out_writes_bytes_(*alen) \
				    struct sockaddr * __restrict asa, \
				    _Inout_ __socklen_t * __restrict alen); }
33	AUE_ACCESS	STD	{ int access(_In_z_ char *path, int amode); }
34	AUE_CHFLAGS	STD	{ int chflags(_In_z_ const char *path, \
				    u_long flags); }
35	AUE_FCHFLAGS	STD	{ int fchflags(int fd, u_long flags); }
36	AUE_SYNC	STD	{ int sync(void); }
37	AUE_KILL	STD	{ int kill(int pid, int signum); }
38	AUE_STAT	COMPAT	{ int stat(_In_z_ char *path, \
				    _Out_ struct ostat *ub); }
39	AUE_GETPPID	STD	{ pid_t getppid(void); }
40	AUE_LSTAT	COMPAT	{ int lstat(_In_z_ char *path, \
				    _Out_ struct ostat *ub); }
41	AUE_DUP		STD	{ int dup(u_int fd); }
42	AUE_PIPE	COMPAT10	{ int pipe(void); }
43	AUE_GETEGID	STD	{ gid_t getegid(void); }
44	AUE_PROFILE	STD	{ int profil( \
				    _Out_writes_bytes_(size) caddr_t samples, \
				    size_t size, size_t offset, u_int scale); }
45	AUE_KTRACE	STD	{ int ktrace(_In_z_ const char *fname, \
				    int ops, int facs, int pid); }
46	AUE_SIGACTION	COMPAT	{ int sigaction(int signum, \
				    _In_opt_ struct osigaction *nsa, \
				    _Out_opt_ struct osigaction *osa); }
47	AUE_GETGID	STD	{ gid_t getgid(void); }
48	AUE_SIGPROCMASK	COMPAT	{ int sigprocmask(int how, osigset_t mask); }
; XXX note nonstandard (bogus) calling convention - the libc stub passes
; us the mask, not a pointer to it, and we return the old mask as the
; (int) return value.
49	AUE_GETLOGIN	STD	{ int getlogin( \
				    _Out_writes_z_(namelen) char *namebuf, \
				    u_int namelen); }
50	AUE_SETLOGIN	STD	{ int setlogin(_In_z_ char *namebuf); }
51	AUE_ACCT	STD	{ int acct(_In_z_ char *path); }
52	AUE_SIGPENDING	COMPAT	{ int sigpending(void); }
53	AUE_SIGALTSTACK	STD	{ int sigaltstack(_In_opt_ stack_t *ss, \
				    _Out_opt_ stack_t *oss); }
54	AUE_IOCTL	STD	{ int ioctl(int fd, u_long com, \
				    _Inout_opt_ caddr_t data); }
55	AUE_REBOOT	STD	{ int reboot(int opt); }
56	AUE_REVOKE	STD	{ int revoke(_In_z_ char *path); }
57	AUE_SYMLINK	STD	{ int symlink(_In_z_ char *path, \
				    _In_z_ char *link); }
58	AUE_READLINK	STD	{ ssize_t readlink(_In_z_ char *path, \
				    _Out_writes_z_(count) char *buf, \
				    size_t count); }
59	AUE_EXECVE	STD	{ int execve( \
				    _In_z_ char *fname, \
				    _In_z_ char **argv, \
				    _In_z_ char **envv); }
60	AUE_UMASK	STD	{ int umask(int newmask); }
61	AUE_CHROOT	STD	{ int chroot(_In_z_ char *path); }
62	AUE_FSTAT	COMPAT	{ int fstat(int fd, _Out_ struct ostat *sb); }
63	AUE_NULL	COMPAT	{ int getkerninfo(int op, \
				    _Out_writes_bytes_opt(*size) char *where, \
				    _Inout_opt_ size_t *size, \
				    int arg); } getkerninfo getkerninfo_args int
64	AUE_NULL	COMPAT	{ int getpagesize(void); } getpagesize \
				    getpagesize_args int
65	AUE_MSYNC	STD	{ int msync(_In_ void *addr, size_t len, \
				    int flags); }
66	AUE_VFORK	STD	{ int vfork(void); }
67	AUE_NULL	OBSOL	vread
68	AUE_NULL	OBSOL	vwrite
69	AUE_SBRK	STD	{ int sbrk(int incr); }
70	AUE_SSTK	STD	{ int sstk(int incr); }
71	AUE_MMAP	COMPAT	{ int mmap(_In_ void *addr, int len, int prot, \
				    int flags, int fd, long pos); }
72	AUE_O_VADVISE	COMPAT11	{ int vadvise(int anom); }
73	AUE_MUNMAP	STD	{ int munmap(_In_ void *addr, size_t len); }
74	AUE_MPROTECT	STD	{ int mprotect(_In_ void *addr, \
				    size_t len, int prot); }
75	AUE_MADVISE	STD	{ int madvise(_In_ void *addr, \
				    size_t len, int behav); }
76	AUE_NULL	OBSOL	vhangup
77	AUE_NULL	OBSOL	vlimit
78	AUE_MINCORE	STD	{ int mincore( \
				    _In_ const void *addr, \
				    size_t len, \
				    _Out_writes_bytes_(len/PAGE_SIZE) \
				    char *vec); }
79	AUE_GETGROUPS	STD	{ int getgroups(u_int gidsetsize, \
				    _Out_writes_opt_(gidsetsize) \
				    gid_t *gidset); }
80	AUE_SETGROUPS	STD	{ int setgroups(u_int gidsetsize, \
				    _In_reads_(gidsetsize) gid_t *gidset); }
81	AUE_GETPGRP	STD	{ int getpgrp(void); }
82	AUE_SETPGRP	STD	{ int setpgid(int pid, int pgid); }
83	AUE_SETITIMER	STD	{ int setitimer(u_int which, \
				    _In_ struct itimerval *itv, \
				    _Out_opt_ struct itimerval *oitv); }
84	AUE_WAIT4	COMPAT	{ int wait(void); }
85	AUE_SWAPON	STD	{ int swapon(_In_z_ char *name); }
86	AUE_GETITIMER	STD	{ int getitimer(u_int which, \
				    _Out_ struct itimerval *itv); }
87	AUE_SYSCTL	COMPAT	{ int gethostname( \
				    _Out_writes_z_(len) char *hostname, \
				    u_int len); } gethostname \
				    gethostname_args int
88	AUE_SYSCTL	COMPAT	{ int sethostname( \
				    _In_reads_z_(len) char *hostname, \
				    u_int len); } sethostname \
				    sethostname_args int
89	AUE_GETDTABLESIZE	STD	{ int getdtablesize(void); }
90	AUE_DUP2	STD	{ int dup2(u_int from, u_int to); }
91	AUE_NULL	UNIMPL	getdopt
92	AUE_FCNTL	STD	{ int fcntl(int fd, int cmd, long arg); }
; XXX should be	{ int fcntl(int fd, int cmd, ...); }
; but we're not ready for varargs.
93	AUE_SELECT	STD	{ int select(int nd, \
				    _Inout_opt_ fd_set *in, \
				    _Inout_opt_ fd_set *ou, \
				    _Inout_opt_ fd_set *ex, \
				    _In_opt_ struct timeval *tv); }
94	AUE_NULL	UNIMPL	setdopt
95	AUE_FSYNC	STD	{ int fsync(int fd); }
96	AUE_SETPRIORITY	STD	{ int setpriority(int which, int who, \
				    int prio); }
97	AUE_SOCKET	STD	{ int socket(int domain, int type, \
				    int protocol); }
98	AUE_CONNECT	STD	{ int connect(int s, \
				    _In_reads_bytes_(namelen) caddr_t name, \
				    int namelen); }
99	AUE_ACCEPT	COMPAT|NOARGS { int accept(int s, \
				    _Out_writes_bytes_opt_(*anamelen) \
				    caddr_t name, int *anamelen); } \
				    accept accept_args int
100	AUE_GETPRIORITY	STD	{ int getpriority(int which, int who); }
101	AUE_SEND	COMPAT	{ int send(int s, \
				    _In_reads_bytes_(len) caddr_t buf, \
				    int len, \
				    int flags); }
102	AUE_RECV	COMPAT	{ int recv(int s, \
				    _Out_writes_bytes_(len) caddr_t buf, \
				    int len, int flags); }
103	AUE_SIGRETURN	COMPAT	{ int sigreturn( \
				    _In_ struct osigcontext *sigcntxp); }
104	AUE_BIND	STD	{ int bind(int s, \
				    _In_reads_bytes_(namelen) caddr_t name, \
				    int namelen); }
105	AUE_SETSOCKOPT	STD	{ int setsockopt(int s, int level, int name, \
				    _In_reads_bytes_opt_(valsize) caddr_t val, \
				    int valsize); }
106	AUE_LISTEN	STD	{ int listen(int s, int backlog); }
107	AUE_NULL	OBSOL	vtimes
108	AUE_NULL	COMPAT	{ int sigvec(int signum, \
				    _In_opt_ struct sigvec *nsv, \
				    _Out_opt_ struct sigvec *osv); }
109	AUE_NULL	COMPAT	{ int sigblock(int mask); }
110	AUE_NULL	COMPAT	{ int sigsetmask(int mask); }
111	AUE_NULL	COMPAT	{ int sigsuspend(osigset_t mask); }
; XXX note nonstandard (bogus) calling convention - the libc stub passes
; us the mask, not a pointer to it.
112	AUE_NULL	COMPAT	{ int sigstack(_In_opt_ struct sigstack *nss, \
				    _Out_opt_ struct sigstack *oss); }
113	AUE_RECVMSG	COMPAT	{ int recvmsg(int s, \
				    _Inout_ struct omsghdr *msg, \
				    int flags); }
114	AUE_SENDMSG	COMPAT	{ int sendmsg(int s, _In_ caddr_t msg, \
				    int flags); }
115	AUE_NULL	OBSOL	vtrace
116	AUE_GETTIMEOFDAY	STD	{ int gettimeofday( \
				    _Out_ struct timeval *tp, \
				    _Out_opt_ struct timezone *tzp); }
117	AUE_GETRUSAGE	STD	{ int getrusage(int who, \
				    _Out_ struct rusage *rusage); }
118	AUE_GETSOCKOPT	STD	{ int getsockopt(int s, int level, int name, \
				    _Out_writes_bytes_opt_(*avalsize) \
				    caddr_t val, _Inout_  int *avalsize); }
119	AUE_NULL	UNIMPL	resuba (BSD/OS 2.x)
120	AUE_READV	STD	{ int readv(int fd, \
				   _Inout_updates_(iovcnt) struct iovec *iovp, \
				   u_int iovcnt); }
121	AUE_WRITEV	STD	{ int writev(int fd, \
				    _In_reads_opt_(iovcnt) struct iovec *iovp, \
				    u_int iovcnt); }
122	AUE_SETTIMEOFDAY	STD	{ int settimeofday( \
				    _In_ struct timeval *tv, \
				    _In_opt_ struct timezone *tzp); }
123	AUE_FCHOWN	STD	{ int fchown(int fd, int uid, int gid); }
124	AUE_FCHMOD	STD	{ int fchmod(int fd, int mode); }
125	AUE_RECVFROM	COMPAT|NOARGS { int recvfrom(int s, \
				    _Out_writes_(len) caddr_t buf, \
				    size_t len, int flags, \
				    _Out_writes_bytes_(*fromlenaddr) \
				    caddr_t from, \
				    _Inout_ int *fromlenaddr); } \
				    recvfrom recvfrom_args int
126	AUE_SETREUID	STD	{ int setreuid(int ruid, int euid); }
127	AUE_SETREGID	STD	{ int setregid(int rgid, int egid); }
128	AUE_RENAME	STD	{ int rename(_In_z_ char *from, \
				    _In_z_ char *to); }
129	AUE_TRUNCATE	COMPAT	{ int truncate(_In_z_ char *path, \
				    long length); }
130	AUE_FTRUNCATE	COMPAT	{ int ftruncate(int fd, long length); }
131	AUE_FLOCK	STD	{ int flock(int fd, int how); }
132	AUE_MKFIFO	STD	{ int mkfifo(_In_z_ char *path, int mode); }
133	AUE_SENDTO	STD	{ int sendto(int s, \
				    _In_reads_bytes_(len) caddr_t buf, \
				    size_t len,  int flags, \
				    _In_reads_bytes_opt_(tolen) caddr_t to, \
				    int tolen); }
134	AUE_SHUTDOWN	STD	{ int shutdown(int s, int how); }
135	AUE_SOCKETPAIR	STD	{ int socketpair(int domain, int type, \
				    int protocol, _Out_writes_(2) int *rsv); }
136	AUE_MKDIR	STD	{ int mkdir(_In_z_ char *path, int mode); }
137	AUE_RMDIR	STD	{ int rmdir(_In_z_ char *path); }
138	AUE_UTIMES	STD	{ int utimes(_In_z_ char *path, \
				    _In_ struct timeval *tptr); }
139	AUE_NULL	OBSOL	4.2 sigreturn
140	AUE_ADJTIME	STD	{ int adjtime(_In_ struct timeval *delta, \
				    _Out_opt_ struct timeval *olddelta); }
141	AUE_GETPEERNAME	COMPAT	{ int getpeername(int fdes, \
				    _Out_writes_bytes_(*alen) \
				    caddr_t asa, \
				    _Inout_opt_ int *alen); }
142	AUE_SYSCTL	COMPAT	{ long gethostid(void); }
143	AUE_SYSCTL	COMPAT	{ int sethostid(long hostid); }
144	AUE_GETRLIMIT	COMPAT	{ int getrlimit(u_int which, _Out_ struct \
				    orlimit *rlp); }
145	AUE_SETRLIMIT	COMPAT	{ int setrlimit(u_int which, \
				    _Out_ struct orlimit *rlp); }
146	AUE_KILLPG	COMPAT	{ int killpg(int pgid, int signum); }
147	AUE_SETSID	STD	{ int setsid(void); }
148	AUE_QUOTACTL	STD	{ int quotactl( \
				    _In_z_ char *path, int cmd, int uid, \
				    _In_ caddr_t arg); }
149	AUE_O_QUOTA	COMPAT	{ int quota(void); }
150	AUE_GETSOCKNAME	COMPAT|NOARGS { int getsockname(int fdec, \
				    _Out_writes_bytes_(*alen) \
				    caddr_t asa, \
				    _Inout_ int *alen); } getsockname \
				    getsockname_args int

; Syscalls 151-180 inclusive are reserved for vendor-specific
; system calls.  (This includes various calls added for compatibity
; with other Unix variants.)
; Some of these calls are now supported by BSD...
151	AUE_NULL	UNIMPL	sem_lock (BSD/OS 2.x)
152	AUE_NULL	UNIMPL	sem_wakeup (BSD/OS 2.x)
153	AUE_NULL	UNIMPL	asyncdaemon (BSD/OS 2.x)
; 154 is initialised by the NLM code, if present.
154	AUE_NULL	NOSTD	{ int nlm_syscall(int debug_level, \
				    int grace_period, int addr_count, \
				    _In_reads_(addr_count) \
				    char **addrs); }
; 155 is initialized by the NFS code, if present.
155	AUE_NFS_SVC	NOSTD	{ int nfssvc(int flag, _In_ caddr_t argp); }
156	AUE_GETDIRENTRIES	COMPAT	{ int getdirentries(int fd, \
				    _Out_writes_bytes_(count) char *buf, \
				    u_int count, _Out_ long *basep); }
157	AUE_STATFS	COMPAT4	{ int statfs(_In_z_ char *path, \
				    _Out_ struct ostatfs *buf); }
158	AUE_FSTATFS	COMPAT4	{ int fstatfs(int fd, \
				    _Out_ struct ostatfs *buf); }
159	AUE_NULL	UNIMPL	nosys
160	AUE_LGETFH	STD { int lgetfh(_In_z_ char *fname, \
				    _Out_ struct fhandle *fhp); }
161	AUE_NFS_GETFH	STD	{ int getfh(_In_z_ char *fname, \
				    _Out_ struct fhandle *fhp); }
162	AUE_SYSCTL	COMPAT4	{ int getdomainname( \
				    _Out_writes_z_(len) char *domainname, \
				    int len); }
163	AUE_SYSCTL	COMPAT4	{ int setdomainname( \
				    _In_reads_z_(len) char *domainname, \
				    int len); }
164	AUE_NULL	COMPAT4	{ int uname(_Out_ struct utsname *name); }
165	AUE_SYSARCH	STD	{ int sysarch(int op, _In_z_ char *parms); }
166	AUE_RTPRIO	STD	{ int rtprio(int function, pid_t pid, \
				    _Inout_ struct rtprio *rtp); }
167	AUE_NULL	UNIMPL	nosys
168	AUE_NULL	UNIMPL	nosys
169	AUE_SEMSYS	NOSTD	{ int semsys(int which, int a2, int a3, \
				    int a4, int a5); }
; XXX should be	{ int semsys(int which, ...); }
170	AUE_MSGSYS	NOSTD	{ int msgsys(int which, int a2, int a3, \
				    int a4, int a5, int a6); }
; XXX should be	{ int msgsys(int which, ...); }
171	AUE_SHMSYS	NOSTD	{ int shmsys(int which, int a2, int a3, \
				    int a4); }
; XXX should be	{ int shmsys(int which, ...); }
172	AUE_NULL	UNIMPL	nosys
173	AUE_PREAD	COMPAT6	{ ssize_t pread(int fd, \
				    _Out_writes_bytes_(nbyte) void *buf, \
				    size_t nbyte, int pad, off_t offset); }
174	AUE_PWRITE	COMPAT6	{ ssize_t pwrite(int fd, \
				    _In_reads_bytes_(nbyte) \
				    const void *buf, \
				    size_t nbyte, int pad, off_t offset); }
175	AUE_SETFIB	STD	{ int setfib(int fibnum); }
176	AUE_NTP_ADJTIME	STD	{ int ntp_adjtime(_Inout_ struct timex *tp); }
177	AUE_NULL	UNIMPL	sfork (BSD/OS 2.x)
178	AUE_NULL	UNIMPL	getdescriptor (BSD/OS 2.x)
179	AUE_NULL	UNIMPL	setdescriptor (BSD/OS 2.x)
180	AUE_NULL	UNIMPL	nosys

; Syscalls 181-199 are used by/reserved for BSD
181	AUE_SETGID	STD	{ int setgid(gid_t gid); }
182	AUE_SETEGID	STD	{ int setegid(gid_t egid); }
183	AUE_SETEUID	STD	{ int seteuid(uid_t euid); }
184	AUE_NULL	OBSOL	lfs_bmapv
185	AUE_NULL	OBSOL	lfs_markv
186	AUE_NULL	OBSOL	lfs_segclean
187	AUE_NULL	OBSOL	lfs_segwait
188	AUE_STAT	COMPAT11 { int stat(_In_z_ char *path, \
				    _Out_ struct freebsd11_stat *ub); }
189	AUE_FSTAT	COMPAT11 { int fstat(int fd, \
				    _Out_ struct freebsd11_stat *sb); }
190	AUE_LSTAT	COMPAT11 { int lstat(_In_z_ char *path, \
				    _Out_ struct freebsd11_stat *ub); }
191	AUE_PATHCONF	STD	{ int pathconf(_In_z_ char *path, int name); }
192	AUE_FPATHCONF	STD	{ int fpathconf(int fd, int name); }
193	AUE_NULL	UNIMPL	nosys
194	AUE_GETRLIMIT	STD	{ int getrlimit(u_int which, \
				    _Out_ struct rlimit *rlp); } getrlimit \
				    __getrlimit_args int
195	AUE_SETRLIMIT	STD	{ int setrlimit(u_int which, \
				    _In_ struct rlimit *rlp); } setrlimit \
				    __setrlimit_args int
196	AUE_GETDIRENTRIES	COMPAT11 { int getdirentries(int fd, \
				    _Out_writes_bytes_(count) char *buf, \
				    u_int count, _Out_ long *basep); }
197	AUE_MMAP	COMPAT6	{ caddr_t mmap(_In_ caddr_t addr, size_t len, \
				    int prot, int flags, int fd, int pad, \
				    off_t pos); }
198	AUE_NULL	NOPROTO	{ int nosys(void); } __syscall \
				    __syscall_args int
199	AUE_LSEEK	COMPAT6	{ off_t lseek(int fd, int pad, \
				    off_t offset, int whence); }
200	AUE_TRUNCATE	COMPAT6	{ int truncate(_In_z_ char *path, int pad, \
				    off_t length); }
201	AUE_FTRUNCATE	COMPAT6	{ int ftruncate(int fd, int pad, \
				    off_t length); }
202	AUE_SYSCTL	STD	{ int __sysctl( \
				    _In_reads_(namelen) int *name, \
				    u_int namelen, \
				    _Out_writes_bytes_opt_(*oldlenp) \
				    void *old, \
				    _Inout_opt_ size_t *oldlenp, \
				    _In_reads_bytes_opt_(newlen) \
				    void *new, \
				    size_t newlen); } \
				    __sysctl sysctl_args int
203	AUE_MLOCK	STD	{ int mlock(_In_ const void *addr, \
				    size_t len); }
204	AUE_MUNLOCK	STD	{ int munlock(_In_ const void *addr, \
				    size_t len); }
205	AUE_UNDELETE	STD	{ int undelete(_In_z_ char *path); }
206	AUE_FUTIMES	STD	{ int futimes(int fd, \
				    _In_reads_(2) struct timeval *tptr); }
207	AUE_GETPGID	STD	{ int getpgid(pid_t pid); }
208	AUE_NULL	UNIMPL	nosys
209	AUE_POLL	STD	{ int poll( \
				    _Inout_updates_(nfds) \
				    struct pollfd *fds, \
				    u_int nfds, int timeout); }
;
; The following are reserved for loadable syscalls
;
210	AUE_NULL	NODEF|NOTSTATIC	lkmnosys lkmnosys nosys_args int
211	AUE_NULL	NODEF|NOTSTATIC	lkmnosys lkmnosys nosys_args int
212	AUE_NULL	NODEF|NOTSTATIC	lkmnosys lkmnosys nosys_args int
213	AUE_NULL	NODEF|NOTSTATIC	lkmnosys lkmnosys nosys_args int
214	AUE_NULL	NODEF|NOTSTATIC	lkmnosys lkmnosys nosys_args int
215	AUE_NULL	NODEF|NOTSTATIC	lkmnosys lkmnosys nosys_args int
216	AUE_NULL	NODEF|NOTSTATIC	lkmnosys lkmnosys nosys_args int
217	AUE_NULL	NODEF|NOTSTATIC	lkmnosys lkmnosys nosys_args int
218	AUE_NULL	NODEF|NOTSTATIC	lkmnosys lkmnosys nosys_args int
219	AUE_NULL	NODEF|NOTSTATIC	lkmnosys lkmnosys nosys_args int

220	AUE_SEMCTL	COMPAT7|NOSTD { int __semctl(int semid, int semnum, \
				    int cmd, union semun_old *arg); }
221	AUE_SEMGET	NOSTD	{ int semget(key_t key, int nsems, \
				    int semflg); }
222	AUE_SEMOP	NOSTD	{ int semop(int semid, \
				    _In_reads_(nsops) struct sembuf *sops, \
				    size_t nsops); }
223	AUE_NULL	OBSOL	semconfig
224	AUE_MSGCTL	COMPAT7|NOSTD { int msgctl(int msqid, int cmd, \
				    struct msqid_ds_old *buf); }
225	AUE_MSGGET	NOSTD	{ int msgget(key_t key, int msgflg); }
226	AUE_MSGSND	NOSTD	{ int msgsnd(int msqid, \
				    _In_reads_bytes_(msgsz) const void *msgp, \
				    size_t msgsz, int msgflg); }
227	AUE_MSGRCV	NOSTD	{ ssize_t msgrcv(int msqid, \
				    _Out_writes_bytes_(msgsz) void *msgp, \
				    size_t msgsz, long msgtyp, int msgflg); }
228	AUE_SHMAT	NOSTD	{ int shmat(int shmid, \
				    _In_ const void *shmaddr, \
				    int shmflg); }
229	AUE_SHMCTL	COMPAT7|NOSTD { int shmctl(int shmid, int cmd, \
				    struct shmid_ds_old *buf); }
230	AUE_SHMDT	NOSTD	{ int shmdt(_In_ const void *shmaddr); }
231	AUE_SHMGET	NOSTD	{ int shmget(key_t key, size_t size, \
				    int shmflg); }
;
232	AUE_NULL	STD	{ int clock_gettime(clockid_t clock_id, \
				    _Out_ struct timespec *tp); }
233	AUE_CLOCK_SETTIME	STD	{ int clock_settime(clockid_t clock_id, \
				    _In_ const struct timespec *tp); }
234	AUE_NULL	STD	{ int clock_getres(clockid_t clock_id, \
				    _Out_ struct timespec *tp); }
235	AUE_NULL	STD	{ int ktimer_create( \
				    clockid_t clock_id, \
				    _In_ struct sigevent *evp, \
				    _Out_ int *timerid); }
236	AUE_NULL	STD	{ int ktimer_delete(int timerid); }
237	AUE_NULL	STD	{ int ktimer_settime(int timerid, \
				    int flags, \
				    _In_ const struct itimerspec *value, \
				    _Out_opt_ struct itimerspec *ovalue); }
238	AUE_NULL	STD	{ int ktimer_gettime(int timerid, \
				    _Out_ struct itimerspec *value); }
239	AUE_NULL	STD	{ int ktimer_getoverrun(int timerid); }
240	AUE_NULL	STD	{ int nanosleep( \
				    _In_ const struct timespec *rqtp, \
				    _Out_opt_ struct timespec *rmtp); }
241	AUE_NULL	STD	{ int ffclock_getcounter( \
				    _Out_ ffcounter *ffcount); }
242	AUE_NULL	STD	{ int ffclock_setestimate( \
				    _In_ struct ffclock_estimate *cest); }
243	AUE_NULL	STD	{ int ffclock_getestimate( \
				    _Out_ struct ffclock_estimate *cest); }
244	AUE_NULL	STD	{ int clock_nanosleep(clockid_t clock_id, \
				    int flags, \
				    _In_ const struct timespec *rqtp, \
				    _Out_opt_ struct timespec *rmtp); }
245	AUE_NULL	UNIMPL	nosys
246	AUE_NULL	UNIMPL	nosys
247	AUE_NULL	STD	{ int clock_getcpuclockid2(id_t id, \
				    int which, _Out_ clockid_t *clock_id); }
248	AUE_NULL	STD	{ int ntp_gettime( \
				    _Out_ struct ntptimeval *ntvp); }
249	AUE_NULL	UNIMPL	nosys
; syscall numbers initially used in OpenBSD
250	AUE_MINHERIT	STD	{ int minherit( \
				    _In_ void *addr, \
				    size_t len, int inherit); }
251	AUE_RFORK	STD	{ int rfork(int flags); }
252	AUE_POLL	OBSOL	openbsd_poll
253	AUE_ISSETUGID	STD	{ int issetugid(void); }
254	AUE_LCHOWN	STD	{ int lchown(_In_z_ char *path, int uid, \
				    int gid); }
255	AUE_AIO_READ	STD	{ int aio_read( \
				    _Inout_ struct aiocb *aiocbp); }
256	AUE_AIO_WRITE	STD	{ int aio_write( \
				    _Inout_ struct aiocb *aiocbp); }
257	AUE_LIO_LISTIO	STD	{ int lio_listio(int mode, \
				    _Inout_updates_(nent) \
				    struct aiocb* const *acb_list, \
				    int nent, \
				    _In_opt_ struct sigevent *sig); }
258	AUE_NULL	UNIMPL	nosys
259	AUE_NULL	UNIMPL	nosys
260	AUE_NULL	UNIMPL	nosys
261	AUE_NULL	UNIMPL	nosys
262	AUE_NULL	UNIMPL	nosys
263	AUE_NULL	UNIMPL	nosys
264	AUE_NULL	UNIMPL	nosys
265	AUE_NULL	UNIMPL	nosys
266	AUE_NULL	UNIMPL	nosys
267	AUE_NULL	UNIMPL	nosys
268	AUE_NULL	UNIMPL	nosys
269	AUE_NULL	UNIMPL	nosys
270	AUE_NULL	UNIMPL	nosys
271	AUE_NULL	UNIMPL	nosys
272	AUE_O_GETDENTS	COMPAT11 { int getdents(int fd, \
				    _Out_writes_bytes_(count) char *buf, \
				    size_t count); }
273	AUE_NULL	UNIMPL	nosys
274	AUE_LCHMOD	STD	{ int lchmod(_In_z_ char *path, mode_t mode); }
275	AUE_NULL	OBSOL	netbsd_lchown
276	AUE_LUTIMES	STD	{ int lutimes(_In_z_ char *path, \
				    _In_ struct timeval *tptr); }
277	AUE_NULL	OBSOL	netbsd_msync
278	AUE_STAT	COMPAT11 { int nstat(_In_z_ char *path, \
				    _Out_ struct nstat *ub); }
279	AUE_FSTAT	COMPAT11 { int nfstat(int fd, _Out_ struct nstat *sb); }
280	AUE_LSTAT	COMPAT11 { int nlstat(_In_z_ char *path, \
				    _Out_ struct nstat *ub); }
281	AUE_NULL	UNIMPL	nosys
282	AUE_NULL	UNIMPL	nosys
283	AUE_NULL	UNIMPL	nosys
284	AUE_NULL	UNIMPL	nosys
285	AUE_NULL	UNIMPL	nosys
286	AUE_NULL	UNIMPL	nosys
287	AUE_NULL	UNIMPL	nosys
288	AUE_NULL	UNIMPL	nosys
289	AUE_PREADV	STD	{ ssize_t preadv(int fd, \
				    _In_reads_(iovcnt) \
				    struct iovec *iovp, \
				    u_int iovcnt, off_t offset); }
290	AUE_PWRITEV	STD	{ ssize_t pwritev(int fd, \
				    _In_reads_(iovcnt) struct iovec *iovp, \
				    u_int iovcnt, off_t offset); }
291	AUE_NULL	UNIMPL	nosys
292	AUE_NULL	UNIMPL	nosys
293	AUE_NULL	UNIMPL	nosys
294	AUE_NULL	UNIMPL	nosys
295	AUE_NULL	UNIMPL	nosys
296	AUE_NULL	UNIMPL	nosys
297	AUE_FHSTATFS	COMPAT4	{ int fhstatfs( \
				    _In_ const struct fhandle *u_fhp, \
				    _Out_ struct ostatfs *buf); }
298	AUE_FHOPEN	STD	{ int fhopen( \
				    _In_ const struct fhandle *u_fhp, \
				    int flags); }
299	AUE_FHSTAT	COMPAT11 { int fhstat( \
				    _In_ const struct fhandle *u_fhp, \
				    _Out_ struct freebsd11_stat *sb); }
300	AUE_NULL	STD	{ int modnext(int modid); }
301	AUE_NULL	STD	{ int modstat(int modid, \
				    _Out_ struct module_stat* stat); }
302	AUE_NULL	STD	{ int modfnext(int modid); }
303	AUE_NULL	STD	{ int modfind(_In_z_ const char *name); }
304	AUE_MODLOAD	STD	{ int kldload(_In_z_ const char *file); }
305	AUE_MODUNLOAD	STD	{ int kldunload(int fileid); }
306	AUE_NULL	STD	{ int kldfind(_In_z_ const char *file); }
307	AUE_NULL	STD	{ int kldnext(int fileid); }
308	AUE_NULL	STD	{ int kldstat(int fileid, \
				    _Out_ struct kld_file_stat *stat); }
309	AUE_NULL	STD	{ int kldfirstmod(int fileid); }
310	AUE_GETSID	STD	{ int getsid(pid_t pid); }
311	AUE_SETRESUID	STD	{ int setresuid(uid_t ruid, uid_t euid, \
				    uid_t suid); }
312	AUE_SETRESGID	STD	{ int setresgid(gid_t rgid, gid_t egid, \
				    gid_t sgid); }
313	AUE_NULL	OBSOL	signanosleep
314	AUE_AIO_RETURN	STD	{ ssize_t aio_return( \
				    _Inout_ struct aiocb *aiocbp); }
315	AUE_AIO_SUSPEND	STD	{ int aio_suspend( \
				    _Inout_updates_(nent) \
				    struct aiocb * const * aiocbp, \
				    int nent, \
				    _In_opt_ \
				    const struct timespec *timeout); }
316	AUE_AIO_CANCEL	STD	{ int aio_cancel(int fd, \
				    _In_opt_ struct aiocb *aiocbp); }
317	AUE_AIO_ERROR	STD	{ int aio_error( \
				    _In_ struct aiocb *aiocbp); }
318	AUE_AIO_READ	COMPAT6	{ int aio_read( \
				    _Inout_  struct oaiocb *aiocbp); }
319	AUE_AIO_WRITE	COMPAT6	{ int aio_write( \
				    _Inout_ struct oaiocb *aiocbp); }
320	AUE_LIO_LISTIO	COMPAT6	{ int lio_listio(int mode, \
				    _Inout_updates_(nent) \
				    struct oaiocb * const *acb_list, \
				    int nent, \
				    _In_opt_ struct osigevent *sig); }
321	AUE_NULL	STD	{ int yield(void); }
322	AUE_NULL	OBSOL	thr_sleep
323	AUE_NULL	OBSOL	thr_wakeup
324	AUE_MLOCKALL	STD	{ int mlockall(int how); }
325	AUE_MUNLOCKALL	STD	{ int munlockall(void); }
326	AUE_GETCWD	STD	{ int __getcwd( \
				    _Out_writes_z_(buflen) char *buf, \
				    size_t buflen); }
327	AUE_NULL	STD	{ int sched_setparam (pid_t pid, \
				    _In_ const struct sched_param *param); }
328	AUE_NULL	STD	{ int sched_getparam (pid_t pid, \
				    _Out_ struct sched_param *param); }
329	AUE_NULL	STD	{ int sched_setscheduler (pid_t pid, int \
				    policy, _In_ const struct sched_param \
				    *param); }
330	AUE_NULL	STD	{ int sched_getscheduler (pid_t pid); }
331	AUE_NULL	STD	{ int sched_yield (void); }
332	AUE_NULL	STD	{ int sched_get_priority_max (int policy); }
333	AUE_NULL	STD	{ int sched_get_priority_min (int policy); }
334	AUE_NULL	STD	{ int sched_rr_get_interval (pid_t pid, \
				    _Out_ struct timespec *interval); }
335	AUE_NULL	STD	{ int utrace( \
				   _In_reads_bytes_(len) const void *addr, \
				    size_t len); }
336	AUE_SENDFILE	COMPAT4	{ int sendfile(int fd, int s, \
				    off_t offset, size_t nbytes, \
				    _In_opt_ struct sf_hdtr *hdtr, \
				    _Out_opt_ off_t *sbytes, int flags); }
337	AUE_NULL	STD	{ int kldsym(int fileid, int cmd, \
				    _In_ void *data); }
338	AUE_JAIL	STD	{ int jail( \
				    _In_ struct jail *jail); }

339	AUE_NULL	NOSTD|NOTSTATIC	{ int nnpfs_syscall(int operation, \
				    char *a_pathP, int a_opcode, \
				    void *a_paramsP, \
				    int a_followSymlinks); }
340	AUE_SIGPROCMASK	STD	{ int sigprocmask(int how, \
				    _In_opt_ const sigset_t *set, \
				    _Out_opt_ sigset_t *oset); }
341	AUE_SIGSUSPEND	STD	{ int sigsuspend( \
				    _In_ const sigset_t *sigmask); }
342	AUE_SIGACTION	COMPAT4	{ int sigaction(int sig, \
				    _In_opt_ const struct sigaction *act, \
				    _Out_opt_ struct sigaction *oact); }
343	AUE_SIGPENDING	STD	{ int sigpending(_In_ sigset_t *set); }
344	AUE_SIGRETURN	COMPAT4	{ int sigreturn( \
				    _In_ const struct ucontext4 *sigcntxp); }
345	AUE_SIGWAIT	STD	{ int sigtimedwait(_In_ const sigset_t *set, \
				    _Out_opt_ siginfo_t *info, \
				    _In_opt_ const struct timespec *timeout); }
346	AUE_NULL	STD	{ int sigwaitinfo(_In_ const sigset_t *set, \
				    _Out_opt_ siginfo_t *info); }
347	AUE_ACL_GET_FILE	STD	{ int __acl_get_file( \
				    _In_z_ const char *path, \
				    acl_type_t type, \
				    _Out_ struct acl *aclp); }
348	AUE_ACL_SET_FILE	STD	{ int __acl_set_file( \
				    _In_z_ const char *path, \
				    acl_type_t type, _In_ struct acl *aclp); }
349	AUE_ACL_GET_FD	STD	{ int __acl_get_fd(int filedes, \
				    acl_type_t type, _Out_ struct acl *aclp); }
350	AUE_ACL_SET_FD	STD	{ int __acl_set_fd(int filedes, \
				    acl_type_t type, _In_ struct acl *aclp); }
351	AUE_ACL_DELETE_FILE	STD	{ int __acl_delete_file( \
				    _In_z_ const char *path, \
				    acl_type_t type); }
352	AUE_ACL_DELETE_FD	STD	{ int __acl_delete_fd(int filedes, \
				    acl_type_t type); }
353	AUE_ACL_CHECK_FILE	STD	{ int __acl_aclcheck_file( \
				    _In_z_ const char *path, \
				    acl_type_t type, _In_ struct acl *aclp); }
354	AUE_ACL_CHECK_FD	STD	{ int __acl_aclcheck_fd(int filedes, \
				    acl_type_t type, _In_ struct acl *aclp); }
355	AUE_EXTATTRCTL	STD	{ int extattrctl(_In_z_ const char *path, \
				    int cmd, _In_z_opt_ const char *filename, \
				    int attrnamespace, \
				    _In_z_ const char *attrname); }
356	AUE_EXTATTR_SET_FILE	STD	{ ssize_t extattr_set_file( \
				    _In_z_ const char *path, \
				    int attrnamespace, \
				    _In_z_ const char *attrname, \
				    _In_reads_bytes_(nbytes) void *data, \
				    size_t nbytes); }
357	AUE_EXTATTR_GET_FILE	STD	{ ssize_t extattr_get_file( \
				    _In_z_ const char *path, \
				    int attrnamespace, \
				    _In_z_ const char *attrname, \
				    _Out_writes_bytes_(nbytes) void *data, \
				    size_t nbytes); }
358	AUE_EXTATTR_DELETE_FILE	STD	{ int extattr_delete_file( \
				    _In_z_ const char *path, \
				    int attrnamespace, \
				    _In_z_ const char *attrname); }
359	AUE_AIO_WAITCOMPLETE	STD	{ ssize_t aio_waitcomplete( \
				    _Outptr_result_maybenull_ \
				    struct aiocb **aiocbp, \
				    _In_opt_ struct timespec *timeout); }
360	AUE_GETRESUID	STD	{ int getresuid(_Out_opt_ uid_t *ruid, \
				    _Out_opt_ uid_t *euid, \
				    _Out_opt_ uid_t *suid); }
361	AUE_GETRESGID	STD	{ int getresgid(_Out_opt_ gid_t *rgid, \
				    _Out_opt_ gid_t *egid, \
				    _Out_opt_ gid_t *sgid); }
362	AUE_KQUEUE	STD	{ int kqueue(void); }
363	AUE_KEVENT	COMPAT11 { int kevent(int fd, \
				    _In_reads_opt_(nchanges) \
				    struct kevent_freebsd11 *changelist, \
				    int nchanges, \
				    _Out_writes_opt_(nevents) \
				    struct kevent_freebsd11 *eventlist, \
				    int nevents, \
				    _In_opt_ const struct timespec *timeout); }
364	AUE_NULL	OBSOL	__cap_get_proc
365	AUE_NULL	OBSOL	__cap_set_proc
366	AUE_NULL	OBSOL	__cap_get_fd
367	AUE_NULL	OBSOL	__cap_get_file
368	AUE_NULL	OBSOL	__cap_set_fd
369	AUE_NULL	OBSOL	__cap_set_file
370	AUE_NULL	UNIMPL	nosys
371	AUE_EXTATTR_SET_FD	STD	{ ssize_t extattr_set_fd(int fd, \
				    int attrnamespace, \
				    _In_z_ const char *attrname, \
				    _In_reads_bytes_(nbytes) void *data, \
				    size_t nbytes); }
372	AUE_EXTATTR_GET_FD	STD	{ ssize_t extattr_get_fd(int fd, \
				    int attrnamespace, \
				    _In_z_ const char *attrname, \
				    _Out_writes_bytes_(nbytes) void *data, \
				    size_t nbytes); }
373	AUE_EXTATTR_DELETE_FD	STD	{ int extattr_delete_fd(int fd, \
				    int attrnamespace, \
				    _In_z_ const char *attrname); }
374	AUE_SETUGID	STD	{ int __setugid(int flag); }
375	AUE_NULL	OBSOL	nfsclnt
376	AUE_EACCESS	STD	{ int eaccess(_In_z_ char *path, int amode); }
377	AUE_NULL	NOSTD|NOTSTATIC	{ int afs3_syscall(long syscall, \
				    long parm1, long parm2, long parm3, \
				    long parm4, long parm5, long parm6); }
378	AUE_NMOUNT	STD	{ int nmount( \
				    _In_reads_(iovcnt) struct iovec *iovp, \
				    unsigned int iovcnt, int flags); }
379	AUE_NULL	OBSOL	kse_exit
380	AUE_NULL	OBSOL	kse_wakeup
381	AUE_NULL	OBSOL	kse_create
382	AUE_NULL	OBSOL	kse_thr_interrupt
383	AUE_NULL	OBSOL	kse_release
384	AUE_NULL	STD	{ int __mac_get_proc( \
				    _In_ struct mac *mac_p); }
385	AUE_NULL	STD	{ int __mac_set_proc( \
				    _In_ struct mac *mac_p); }
386	AUE_NULL	STD	{ int __mac_get_fd(int fd, \
				    _In_ struct mac *mac_p); }
387	AUE_NULL	STD	{ int __mac_get_file( \
				    _In_z_ const char *path_p, \
				    _In_ struct mac *mac_p); }
388	AUE_NULL	STD	{ int __mac_set_fd(int fd, \
				    _In_ struct mac *mac_p); }
389	AUE_NULL	STD	{ int __mac_set_file( \
				    _In_z_ const char *path_p, \
				    _In_ struct mac *mac_p); }
390	AUE_NULL	STD	{ int kenv(int what, \
				    _In_z_opt_ const char *name, \
				    _Inout_updates_opt_(len) \
				    char *value, int len); }
391	AUE_LCHFLAGS	STD	{ int lchflags(_In_z_ const char *path, \
				    u_long flags); }
392	AUE_NULL	STD	{ int uuidgen( \
				    _Out_writes_(count) struct uuid *store, \
				    int count); }
393	AUE_SENDFILE	STD	{ int sendfile(int fd, int s, \
				    off_t offset, \
				    size_t nbytes, \
				    _In_opt_ struct sf_hdtr *hdtr, \
				    _Out_opt_ off_t *sbytes, int flags); }
394	AUE_NULL	STD	{ int mac_syscall( \
				    _In_z_ const char *policy, \
				    int call, \
				    _In_opt_ void *arg); }
395	AUE_GETFSSTAT	COMPAT11 { int getfsstat( \
				    _Out_writes_bytes_opt_(bufsize) \
				    struct freebsd11_statfs *buf, \
				    long bufsize, int mode); }
396	AUE_STATFS	COMPAT11 { int statfs(_In_z_ char *path, \
				    _Out_ struct freebsd11_statfs *buf); }
397	AUE_FSTATFS	COMPAT11 { int fstatfs(int fd, \
				    _Out_ struct freebsd11_statfs *buf); }
398	AUE_FHSTATFS	COMPAT11 { int fhstatfs( \
				    _In_ const struct fhandle *u_fhp, \
				    _Out_ struct freebsd11_statfs *buf); }
399	AUE_NULL	UNIMPL	nosys
400	AUE_SEMCLOSE	NOSTD	{ int ksem_close(semid_t id); }
401	AUE_SEMPOST	NOSTD	{ int ksem_post(semid_t id); }
402	AUE_SEMWAIT	NOSTD	{ int ksem_wait(semid_t id); }
403	AUE_SEMTRYWAIT	NOSTD	{ int ksem_trywait(semid_t id); }
404	AUE_SEMINIT	NOSTD	{ int ksem_init(_Out_ semid_t *idp, \
				    unsigned int value); }
405	AUE_SEMOPEN	NOSTD	{ int ksem_open(_Out_ semid_t *idp, \
				    _In_z_ const char *name, int oflag, \
				    mode_t mode, unsigned int value); }
406	AUE_SEMUNLINK	NOSTD	{ int ksem_unlink( \
				    _In_z_ const char *name); }
407	AUE_SEMGETVALUE	NOSTD	{ int ksem_getvalue(semid_t id, \
				    _Out_ int *val); }
408	AUE_SEMDESTROY	NOSTD	{ int ksem_destroy(semid_t id); }
409	AUE_NULL	STD	{ int __mac_get_pid(pid_t pid, \
				    _In_ struct mac *mac_p); }
410	AUE_NULL	STD	{ int __mac_get_link( \
				    _In_z_ const char *path_p, \
				    _In_ struct mac *mac_p); }
411	AUE_NULL	STD	{ int __mac_set_link( \
				    _In_z_ const char *path_p, \
				    _In_ struct mac *mac_p); }
412	AUE_EXTATTR_SET_LINK	STD	{ ssize_t extattr_set_link( \
				    _In_z_ const char *path, \
				    int attrnamespace, \
				    _In_z_ const char *attrname, \
				    _In_reads_bytes_(nbytes) void *data, \
				    size_t nbytes); }
413	AUE_EXTATTR_GET_LINK	STD	{ ssize_t extattr_get_link( \
				    _In_z_ const char *path, \
				    int attrnamespace, \
				    _In_z_ const char *attrname, \
				    _Out_writes_bytes_(nbytes) void *data, \
				    size_t nbytes); }
414	AUE_EXTATTR_DELETE_LINK	STD	{ int extattr_delete_link( \
				    _In_z_ const char *path, \
				    int attrnamespace, \
				    _In_z_ const char *attrname); }
415	AUE_NULL	STD	{ int __mac_execve(_In_z_ char *fname, \
				    _In_ char **argv, \
				    _In_ char **envv, \
				    _In_ struct mac *mac_p); }
416	AUE_SIGACTION	STD	{ int sigaction(int sig, \
				    _In_opt_ const struct sigaction *act, \
				    _Out_opt_ struct sigaction *oact); }
417	AUE_SIGRETURN	STD	{ int sigreturn( \
				    _In_ const struct __ucontext *sigcntxp); }
418	AUE_NULL	UNIMPL	__xstat
419	AUE_NULL	UNIMPL	__xfstat
420	AUE_NULL	UNIMPL	__xlstat
421	AUE_NULL	STD	{ int getcontext( \
				    _Out_ struct __ucontext *ucp); }
422	AUE_NULL	STD	{ int setcontext( \
				    _In_ const struct __ucontext *ucp); }
423	AUE_NULL	STD	{ int swapcontext( \
				    _Out_ struct __ucontext *oucp, \
				    _In_ const struct __ucontext *ucp); }
424	AUE_SWAPOFF	STD	{ int swapoff(_In_z_ const char *name); }
425	AUE_ACL_GET_LINK	STD	{ int __acl_get_link( \
				    _In_z_ const char *path, acl_type_t type, \
				    _Out_ struct acl *aclp); }
426	AUE_ACL_SET_LINK	STD	{ int __acl_set_link( \
				    _In_z_ const char *path, acl_type_t type, \
				    _In_ struct acl *aclp); }
427	AUE_ACL_DELETE_LINK	STD	{ int __acl_delete_link( \
				    _In_z_ const char *path, \
				    acl_type_t type); }
428	AUE_ACL_CHECK_LINK	STD	{ int __acl_aclcheck_link( \
				    _In_z_ const char *path, acl_type_t type, \
				    _In_ struct acl *aclp); }
429	AUE_SIGWAIT	STD	{ int sigwait(_In_ const sigset_t *set, \
				    _Out_ int *sig); }
430	AUE_THR_CREATE	STD	{ int thr_create(_In_ ucontext_t *ctx, \
				    _Out_ long *id, int flags); }
431	AUE_THR_EXIT	STD	{ void thr_exit(_Out_opt_ long *state); }
432	AUE_NULL	STD	{ int thr_self(_Out_ long *id); }
433	AUE_THR_KILL	STD	{ int thr_kill(long id, int sig); }
434	AUE_NULL	UNIMPL	nosys
435	AUE_NULL	UNIMPL	nosys
436	AUE_JAIL_ATTACH	STD	{ int jail_attach(int jid); }
437	AUE_EXTATTR_LIST_FD	STD	{ ssize_t extattr_list_fd(int fd, \
				    int attrnamespace, \
				    _Out_writes_bytes_opt_(nbytes) \
				    void *data, \
				    size_t nbytes); }
438	AUE_EXTATTR_LIST_FILE	STD	{ ssize_t extattr_list_file( \
				    _In_z_ const char *path, \
				    int attrnamespace, \
				    _Out_writes_bytes_opt_(nbytes) \
				    void *data, size_t nbytes); }
439	AUE_EXTATTR_LIST_LINK	STD	{ ssize_t extattr_list_link( \
				    _In_z_ const char *path, \
				    int attrnamespace, \
				    _Out_writes_bytes_opt_(nbytes) \
				    void *data, size_t nbytes); }
440	AUE_NULL	OBSOL	kse_switchin
441	AUE_SEMWAIT	NOSTD	{ int ksem_timedwait(semid_t id, \
				    _In_opt_ const struct timespec *abstime); }
442	AUE_NULL	STD	{ int thr_suspend( \
				    _In_opt_ const struct timespec *timeout); }
443	AUE_NULL	STD	{ int thr_wake(long id); }
444	AUE_MODUNLOAD	STD	{ int kldunloadf(int fileid, int flags); }
445	AUE_AUDIT	STD	{ int audit( \
				    _In_reads_bytes_(length) \
				    const void *record, \
				    u_int length); }
446	AUE_AUDITON	STD	{ int auditon(int cmd, \
				    _In_opt_ void *data, \
				    u_int length); }
447	AUE_GETAUID	STD	{ int getauid(_Out_ uid_t *auid); }
448	AUE_SETAUID	STD	{ int setauid(_In_ uid_t *auid); }
449	AUE_GETAUDIT	STD	{ int getaudit( \
				    _Out_ struct auditinfo *auditinfo); }
450	AUE_SETAUDIT	STD	{ int setaudit( \
				    _In_ struct auditinfo *auditinfo); }
451	AUE_GETAUDIT_ADDR	STD	{ int getaudit_addr( \
				    _Out_writes_bytes_(length) \
				    struct auditinfo_addr *auditinfo_addr, \
				    u_int length); }
452	AUE_SETAUDIT_ADDR	STD	{ int setaudit_addr( \
				    _In_reads_bytes_(length) \
				    struct auditinfo_addr *auditinfo_addr, \
				    u_int length); }
453	AUE_AUDITCTL	STD	{ int auditctl(_In_z_ char *path); }
454	AUE_NULL	STD	{ int _umtx_op(_Inout_ void *obj, int op, \
				    u_long val, _In_ void *uaddr1, \
				    _In_ void *uaddr2); }
455	AUE_THR_NEW	STD	{ int thr_new(_In_ struct thr_param *param, \
				    int param_size); }
456	AUE_NULL	STD	{ int sigqueue(pid_t pid, int signum, \
				    _In_ void *value); }

457	AUE_MQ_OPEN	NOSTD	{ int kmq_open( \
				    _In_z_ const char *path, \
				    int flags, \
				    mode_t mode, \
				    _In_opt_ const struct mq_attr *attr); }
458	AUE_MQ_SETATTR	NOSTD	{ int kmq_setattr(int mqd, \
				    _In_opt_ const struct mq_attr *attr, \
				    _Out_opt_ struct mq_attr *oattr); }
459	AUE_MQ_TIMEDRECEIVE	NOSTD	{ int kmq_timedreceive( \
				    int mqd, \
				    _Out_writes_bytes_(msg_len) char *msg_ptr, \
				    size_t msg_len, \
				    _Out_opt_ unsigned *msg_prio, \
				    _In_opt_ \
				    const struct timespec *abs_timeout); }
460	AUE_MQ_TIMEDSEND	NOSTD	{ int kmq_timedsend(int mqd, \
				    _In_reads_bytes_(msg_len) \
				    const char *msg_ptr, size_t msg_len, \
				    unsigned msg_prio, \
				    _In_opt_ \
				    const struct timespec *abs_timeout); }
461	AUE_MQ_NOTIFY	NOSTD	{ int kmq_notify(int mqd, \
				    _In_opt_ const struct sigevent *sigev); }
462	AUE_MQ_UNLINK	NOSTD	{ int kmq_unlink(_In_z_ const char *path); }
463	AUE_NULL	STD	{ int abort2(_In_z_ const char *why, \
				    int nargs, \
				    _In_reads_(nargs) void **args); }
464	AUE_NULL	STD	{ int thr_set_name(long id, \
				    _In_z_ const char *name); }
465	AUE_AIO_FSYNC	STD	{ int aio_fsync(int op, \
				    _In_ struct aiocb *aiocbp); }
466	AUE_RTPRIO	STD	{ int rtprio_thread(int function, \
				    lwpid_t lwpid, \
				    _Inout_ struct rtprio *rtp); }
467	AUE_NULL	UNIMPL	nosys
468	AUE_NULL	UNIMPL	nosys
469	AUE_NULL	UNIMPL	__getpath_fromfd
470	AUE_NULL	UNIMPL	__getpath_fromaddr
471	AUE_SCTP_PEELOFF	NOSTD	{ int sctp_peeloff(int sd, \
				    uint32_t name); }
472	AUE_SCTP_GENERIC_SENDMSG	NOSTD	{ int sctp_generic_sendmsg( \
				    int sd, \
				    _In_reads_bytes_(mlen) caddr_t msg, \
				    int mlen, \
				    _In_reads_bytes_(tolen) \
				    caddr_t to, \
				    __socklen_t tolen, \
				    _In_opt_ struct sctp_sndrcvinfo *sinfo, \
				    int flags); }
473	AUE_SCTP_GENERIC_SENDMSG_IOV	NOSTD	{ \
				int sctp_generic_sendmsg_iov( \
				    int sd, \
				    _In_reads_(iovlen) struct iovec *iov, \
				    int iovlen, \
				    _In_reads_bytes_(tolen) \
				    caddr_t to, \
				    __socklen_t tolen, \
				    _In_opt_ struct sctp_sndrcvinfo *sinfo, \
				    int flags); }
474	AUE_SCTP_GENERIC_RECVMSG	NOSTD	{ int sctp_generic_recvmsg( \
				    int sd, \
				    _In_reads_(iovlen) struct iovec *iov, \
				    int iovlen, \
				    _Out_writes_bytes_(*fromlenaddr) \
				    struct sockaddr *from, \
				    _Out_ __socklen_t *fromlenaddr, \
				    _In_opt_ struct sctp_sndrcvinfo *sinfo, \
				    _Out_opt_ int *msg_flags); }
475	AUE_PREAD	STD	{ ssize_t pread(int fd, \
				    _Out_writes_bytes_(nbyte) void *buf, \
				    size_t nbyte, off_t offset); }
476	AUE_PWRITE	STD	{ ssize_t pwrite(int fd, \
				    _In_reads_bytes_(nbyte) \
				    const void *buf, \
				    size_t nbyte, off_t offset); }
477	AUE_MMAP	STD	{ caddr_t mmap(_In_ caddr_t addr, size_t len, \
				    int prot, int flags, int fd, off_t pos); }
478	AUE_LSEEK	STD	{ off_t lseek(int fd, off_t offset, \
				    int whence); }
479	AUE_TRUNCATE	STD	{ int truncate(_In_z_ char *path,  \
				    off_t length); }
480	AUE_FTRUNCATE	STD	{ int ftruncate(int fd, off_t length); }
481	AUE_THR_KILL2	STD	{ int thr_kill2(pid_t pid, long id, int sig); }
482	AUE_SHMOPEN	STD	{ int shm_open( \
				    _In_z_ const char *path, \
				    int flags, \
				    mode_t mode); }
483	AUE_SHMUNLINK	STD	{ int shm_unlink(_In_z_ const char *path); }
484	AUE_NULL	STD	{ int cpuset(_Out_ cpusetid_t *setid); }
485	AUE_NULL	STD	{ int cpuset_setid(cpuwhich_t which, id_t id, \
				    cpusetid_t setid); }
486	AUE_NULL	STD	{ int cpuset_getid(cpulevel_t level, \
				    cpuwhich_t which, id_t id, \
				    _Out_ cpusetid_t *setid); }
487	AUE_NULL	STD	{ int cpuset_getaffinity(cpulevel_t level, \
				    cpuwhich_t which, id_t id, \
				    size_t cpusetsize, \
				    _Out_ cpuset_t *mask); }
488	AUE_NULL	STD	{ int cpuset_setaffinity(cpulevel_t level, \
				    cpuwhich_t which, id_t id, \
				    size_t cpusetsize, \
				    _Out_ const cpuset_t *mask); }
489	AUE_FACCESSAT	STD	{ int faccessat(int fd, _In_z_ char *path, \
				    int amode, int flag); }
490	AUE_FCHMODAT	STD	{ int fchmodat(int fd, \
				    _In_z_ char *path, mode_t mode, \
				    int flag); }
491	AUE_FCHOWNAT	STD	{ int fchownat(int fd, _In_z_ char *path, \
				    uid_t uid, gid_t gid, int flag); }
492	AUE_FEXECVE	STD	{ int fexecve(int fd, \
				    _In_ char **argv, \
				    _In_ char **envv); }
493	AUE_FSTATAT	COMPAT11 { int fstatat(int fd, _In_z_ char *path, \
				    _Out_ struct freebsd11_stat *buf, \
				    int flag); }
494	AUE_FUTIMESAT	STD	{ int futimesat(int fd, \
				    _In_z_ char *path, \
				    _In_reads_(2) struct timeval *times); }
495	AUE_LINKAT	STD	{ int linkat(int fd1, \
				    _In_z_ char *path1, \
				    int fd2, \
				    _In_z_ char *path2, \
				    int flag); }
496	AUE_MKDIRAT	STD	{ int mkdirat(int fd, _In_z_ char *path, \
				mode_t mode); }
497	AUE_MKFIFOAT	STD	{ int mkfifoat(int fd, \
				_In_z_ char *path, mode_t mode); }
498	AUE_MKNODAT	COMPAT11 { int mknodat(int fd, _In_z_ char *path, \
				    mode_t mode, \
				    uint32_t dev); }
; XXX: see the comment for open
499	AUE_OPENAT_RWTC	STD	{ int openat(int fd, _In_z_ char *path,  \
				    int flag, mode_t mode); }
500	AUE_READLINKAT	STD	{ int readlinkat(int fd, \
				    _In_z_ char *path, \
				    _Out_writes_bytes_(bufsize) char *buf, \
				    size_t bufsize); }
501	AUE_RENAMEAT	STD	{ int renameat(int oldfd, \
				    _In_z_ char *old, \
				    int newfd, _In_z_ char *new); }
502	AUE_SYMLINKAT	STD	{ int symlinkat(_In_z_ char *path1, int fd, \
				    _In_z_ char *path2); }
503	AUE_UNLINKAT	STD	{ int unlinkat(int fd, _In_z_ char *path, \
				    int flag); }
504	AUE_POSIX_OPENPT	STD	{ int posix_openpt(int flags); }
; 505 is initialised by the kgssapi code, if present.
505	AUE_NULL	NOSTD	{ int gssd_syscall(_In_z_ char *path); }
506	AUE_JAIL_GET	STD	{ int jail_get( \
				    _In_reads_(iovcnt) struct iovec *iovp, \
				    unsigned int iovcnt, int flags); }
507	AUE_JAIL_SET	STD	{ int jail_set( \
				    _In_reads_(iovcnt) struct iovec *iovp, \
				    unsigned int iovcnt, int flags); }
508	AUE_JAIL_REMOVE	STD	{ int jail_remove(int jid); }
509	AUE_CLOSEFROM	STD	{ int closefrom(int lowfd); }
510	AUE_SEMCTL	NOSTD	{ int __semctl(int semid, int semnum, \
				    int cmd, _Inout_ union semun *arg); }
511	AUE_MSGCTL	NOSTD	{ int msgctl(int msqid, int cmd, \
				    _Inout_opt_ struct msqid_ds *buf); }
512	AUE_SHMCTL	NOSTD	{ int shmctl(int shmid, int cmd, \
				    _Inout_opt_ struct shmid_ds *buf); }
513	AUE_LPATHCONF	STD	{ int lpathconf(_In_z_ char *path, \
				    int name); }
514	AUE_NULL	OBSOL	cap_new
515	AUE_CAP_RIGHTS_GET	STD	{ int __cap_rights_get(int version, \
				    int fd, _Out_ cap_rights_t *rightsp); }
516	AUE_CAP_ENTER	STD	{ int cap_enter(void); }
517	AUE_CAP_GETMODE	STD	{ int cap_getmode(_Out_ u_int *modep); }
518	AUE_PDFORK	STD	{ int pdfork(_Out_ int *fdp, int flags); }
519	AUE_PDKILL	STD	{ int pdkill(int fd, int signum); }
520	AUE_PDGETPID	STD	{ int pdgetpid(int fd, _Out_ pid_t *pidp); }
521	AUE_PDWAIT	UNIMPL	pdwait4
522	AUE_SELECT	STD	{ int pselect(int nd, \
				    _Inout_opt_ fd_set *in, \
				    _Inout_opt_ fd_set *ou, \
				    _Inout_opt_ fd_set *ex, \
				    _In_opt_ const struct timespec *ts, \
				    _In_opt_ const sigset_t *sm); }
523	AUE_GETLOGINCLASS	STD	{ int getloginclass( \
				    _Out_writes_z_(namelen) char *namebuf, \
				    size_t namelen); }
524	AUE_SETLOGINCLASS	STD	{ int setloginclass( \
				    _In_z_ const char *namebuf); }
525	AUE_NULL	STD	{ int rctl_get_racct( \
				    _In_reads_bytes_(inbuflen) \
				    const void *inbufp, size_t inbuflen, \
				    _Out_writes_bytes_(outbuflen) \
				    void *outbufp, size_t outbuflen); }
526	AUE_NULL	STD	{ int rctl_get_rules( \
				    _In_reads_bytes_(inbuflen) \
				    const void *inbufp, size_t inbuflen, \
				    _Out_writes_bytes_(outbuflen) \
				    void *outbufp, size_t outbuflen); }
527	AUE_NULL	STD	{ int rctl_get_limits( \
				    _In_reads_bytes_(inbuflen) \
				    const void *inbufp, size_t inbuflen, \
				    _Out_writes_bytes_(outbuflen) \
				    void *outbufp, size_t outbuflen); }
528	AUE_NULL	STD	{ int rctl_add_rule( \
				    _In_reads_bytes_(inbuflen) \
				    const void *inbufp, size_t inbuflen, \
				    _Out_writes_bytes_(outbuflen) \
				    void *outbufp, size_t outbuflen); }
529	AUE_NULL	STD	{ int rctl_remove_rule( \
				    _In_reads_bytes_(inbuflen) \
				    const void *inbufp, size_t inbuflen, \
				    _Out_writes_bytes_(outbuflen) \
				    void *outbufp, size_t outbuflen); }
530	AUE_POSIX_FALLOCATE	STD	{ int posix_fallocate(int fd, \
				    off_t offset, off_t len); }
531	AUE_POSIX_FADVISE	STD	{ int posix_fadvise(int fd, \
				    off_t offset, \
				    off_t len, int advice); }
532	AUE_WAIT6	STD	{ int wait6(idtype_t idtype, id_t id, \
				    _Out_opt_ int *status, int options, \
				    _Out_opt_ struct __wrusage *wrusage, \
				    _Out_opt_ siginfo_t *info); }
533	AUE_CAP_RIGHTS_LIMIT	STD	{ int cap_rights_limit(int fd, \
				    _In_ cap_rights_t *rightsp); }
534	AUE_CAP_IOCTLS_LIMIT	STD	{ int cap_ioctls_limit(int fd, \
				    _In_reads_(ncmds) const u_long *cmds, \
				    size_t ncmds); }
535	AUE_CAP_IOCTLS_GET	STD	{ ssize_t cap_ioctls_get(int fd, \
				    _Out_writes_(maxcmds) u_long *cmds, \
				    size_t maxcmds); }
536	AUE_CAP_FCNTLS_LIMIT	STD	{ int cap_fcntls_limit(int fd, \
				    uint32_t fcntlrights); }
537	AUE_CAP_FCNTLS_GET	STD	{ int cap_fcntls_get(int fd, \
				    _Out_ uint32_t *fcntlrightsp); }
538	AUE_BINDAT	STD	{ int bindat(int fd, int s, \
				    _In_reads_bytes_(namelen) \
				    caddr_t name, \
				    int namelen); }
539	AUE_CONNECTAT	STD	{ int connectat(int fd, int s, \
				    _In_reads_bytes_(namelen) \
				    caddr_t name, \
				    int namelen); }
540	AUE_CHFLAGSAT	STD	{ int chflagsat(int fd, \
				    _In_z_ const char *path, \
				    u_long flags, int atflag); }
541	AUE_ACCEPT	STD	{ int accept4(int s, \
				    _Out_writes_bytes_opt_(*anamelen) \
				    struct sockaddr * __restrict name, \
				    _Inout_opt_ \
				    __socklen_t * __restrict anamelen, \
				    int flags); }
542	AUE_PIPE	STD	{ int pipe2(_Out_writes_(2) int *fildes, \
				    int flags); }
543	AUE_AIO_MLOCK	STD	{ int aio_mlock(_In_ struct aiocb *aiocbp); }
544	AUE_PROCCTL	STD	{ int procctl(idtype_t idtype, id_t id, \
				    int com, _In_opt_ void *data); }
545	AUE_POLL	STD	{ int ppoll( \
				    _Inout_updates_(nfds) struct pollfd *fds, \
				    u_int nfds, \
				    _In_opt_ const struct timespec *ts, \
				    _In_opt_ const sigset_t *set); }
546	AUE_FUTIMES	STD	{ int futimens(int fd, \
				    _In_reads_(2) \
				    struct timespec *times); }
547	AUE_FUTIMESAT	STD	{ int utimensat(int fd, \
				    _In_z_ char *path, \
				    _In_reads_(2) \
				    struct timespec *times, \
				    int flag); }
548	AUE_NULL	OBSOL	numa_getaffinity
549	AUE_NULL	OBSOL	numa_setaffinity
550	AUE_FSYNC	STD	{ int fdatasync(int fd); }
551	AUE_FSTAT	STD	{ int fstat(int fd, _Out_ struct stat *sb); }
552	AUE_FSTATAT	STD	{ int fstatat(int fd, _In_z_ char *path, \
                                    _Out_ struct stat *buf, int flag); }
553	AUE_FHSTAT	STD	{ int fhstat(_In_ const struct fhandle *u_fhp, \
                                    _Out_ struct stat *sb); }
554	AUE_GETDIRENTRIES STD { ssize_t getdirentries(int fd, \
				    _Out_writes_bytes_(count) char *buf, \
                                    size_t count, _Out_ off_t *basep); }
555	AUE_STATFS	STD	{ int statfs(_In_z_ char *path, \
				    _Out_ struct statfs *buf); }
556	AUE_FSTATFS	STD	{ int fstatfs(int fd, \
				    _Out_ struct statfs *buf); }
557	AUE_GETFSSTAT	STD	{ int getfsstat( \
				    _Out_writes_bytes_opt_(bufsize) \
				    struct statfs *buf, \
                                    long bufsize, int mode); }
558	AUE_FHSTATFS	STD	{ int fhstatfs( \
				    _In_ const struct fhandle *u_fhp, \
                                    _Out_ struct statfs *buf); }
559	AUE_MKNODAT	STD	{ int mknodat(int fd, _In_z_ char *path, \
				    mode_t mode, dev_t dev); }
560	AUE_KEVENT	STD	{ int kevent(int fd, \
				    _In_reads_opt_(nchanges) \
				    struct kevent *changelist, \
				    int nchanges, \
				    _Out_writes_opt_(nevents) \
				    struct kevent *eventlist, int nevents, \
				    _In_opt_ const struct timespec *timeout); }
561	AUE_NULL	STD	{ int cpuset_getdomain(cpulevel_t level, \
				    cpuwhich_t which, id_t id, \
				    size_t domainsetsize, \
				    _Out_writes_bytes_(domainsetsize) \
				    domainset_t *mask, \
				    _Out_ int *policy); }
562	AUE_NULL	STD	{ int cpuset_setdomain(cpulevel_t level, \
				    cpuwhich_t which, id_t id, \
				    size_t domainsetsize, \
				    _In_ domainset_t *mask, \
				    int policy); }
563	AUE_NULL	STD	{ int getrandom( \
				    _Out_writes_bytes_(buflen) void *buf, \
				    size_t buflen, unsigned int flags); }
