
#ifndef OS_SPECIFIC_H
#define OS_SPECIFIC_H






#if ( defined(__MACOS_X__) || defined(__MACOS_X) || defined(_MACOS_X) || defined(MACOS_X) || \
	defined(__DARWIN__) || defined(__DARWIN) || defined(_DARWIN) || defined(DARWIN) || \
	defined(__MACOSX__) || defined(__MACOSX) || defined(_MACOSX) || defined(MACOSX) || \
	defined(__OSX__) || defined(__OSX) || defined(_OSX) || defined(OSX) || \
	defined(__APPLE__) || defined(__APPLE) || defined(_APPLE) || defined(APPLE) || \
	defined(__MACINTOSH__) || defined(__MACINTOSH) || defined(_MACINTOSH) || defined(MACINTOSH) || \
	defined(__MAC__) || defined(__MAC) || defined(_MAC) || defined(MAC))

#include <sys/param.h>
#include <mach-o/dyld.h>
#endif


#if ( defined (_WIN32) || defined (_WIN64) )
#include <windows.h>
#include <conio.h>
DWORD oldprot;
// https://msdn.microsoft.com/en-us/library/windows/desktop/aa366898(v=vs.85).aspx
// BOOL WINAPI VirtualProtect
// If the function succeeds, the return value is nonzero.
// If the function fails, the return value is zero.To get extended error information, call GetLastError.
#define unprotect(addr,len) (VirtualProtect(addr, len, PAGE_EXECUTE_READWRITE, &oldprot))
#define GETPAGESIZE()        getpagesize()
#define EXPORT               __declspec (dllexport)
// popen in Microbosoft C++: _popen
#define POPEN(x,y)           _popen (x, y)
#define PCLOSE(x)            _pclose(x)

unsigned long getpagesize(void)
{
	static long g_pagesize = 0;
	if (!g_pagesize)
	{
		SYSTEM_INFO system_info;
		GetSystemInfo(&system_info);
		g_pagesize = system_info.dwPageSize;
	}
	return (unsigned long)g_pagesize;
}

unsigned long uslngPageSize = GETPAGESIZE();
unsigned long uslngPageMask = (~(uslngPageSize - 1));

#else // LINUX / UNIX / OS X
// #include <unistd.h> //STDIN_FILENO
#include <termios.h> //termios, TCSANOW, ECHO, ICANON
#include <sys/mman.h>

// int mprotect(void *addr, size_t len, int prot);
// On success, mprotect() returns zero. On error, -1 is returned, and errno is set appropriately.
#define unprotect(addr,len)  (mprotect(addr,len,PROT_READ|PROT_WRITE|PROT_EXEC))
#define GETPAGESIZE()         sysconf (_SC_PAGE_SIZE)
#define POPEN(x,y)            popen (x, y)
#define PCLOSE(x)             pclose(x)

#endif




#endif // OS_SPECIFIC_H 
