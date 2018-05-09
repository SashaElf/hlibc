#include <stddef.h>
#include <string.h>

char *strrchr(const char *s, int c)
{
	size_t len = strlen(s);
	for (; len > 0; --len) {
		if (s[len] == c) {
			return (char *)s + len;
		}
	}
	return s[0] == c ? (char*)s : NULL;
}
