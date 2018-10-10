#include <sys/mman.h>
#include <sys/syscall.h>


int munmap(void *addr, size_t len)
{ 
        return  __syscall(SYS_munmap, addr, len);
}
