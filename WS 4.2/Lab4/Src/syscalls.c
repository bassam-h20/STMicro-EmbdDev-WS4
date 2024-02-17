/**
*****************************************************************************
**
**  File        : syscalls.c
**
**  Abstract    : System Workbench Minimal System calls file
**
** 		          For more information about which c-functions
**                need which of these lowlevel functions
**                please consult the Newlib libc-manual
**
**  Environment : System Workbench for MCU
**
**  Distribution: The file is distributed �as is,� without any warranty
**                of any kind.
**
**  (c)Copyright System Workbench for MCU.
**  You may use this file as-is or modify it according to the needs of your
**  project. Distribution of this file (unmodified or modified) is not
**  permitted. System Workbench for MCU permit registered System Workbench(R) users the
**  rights to distribute the assembled, compiled & linked contents of this
**  file as part of an application binary file, provided that it is built
**  using the System Workbench for MCU toolchain.
**
*****************************************************************************
*/

//References:
//- Priya, B. (2021) Explain the END OF FILE (EOF) with a C Program. Available from: https://www.tutorialspoint.com/explain-the-end-of-file-eof-with-a-c-program [Accessed 10 January 2023].
//- Gay, D.M. (1991) The Red Hat newlib C Library. Available from: https://sourceware.org/newlib/libc.html#Document-Index [Accessed 10 January 2023]. 

/* Includes */
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>


/* Variables */
//#undef errno
extern int errno;
#define FreeRTOS
#define MAX_STACK_SIZE 0x2000

extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));

#ifndef FreeRTOS
  register char * stack_ptr asm("sp");
#endif


register char * stack_ptr asm("sp");

char *__env[1] = { 0 };
char **environ = __env;


/* Functions */
void initialise_monitor_handles()
{
}

int _getpid(void)
{
	return 1;
}

int _kill(int pid, int sig)
{
	errno = EINVAL;
	return -1;
}

void _exit (int status)
{
	_kill(status, -1);
	while (1) {}		/* Make sure we hang here */
}

/*
int _read (int file, char *ptr, int len)
{
	/* The I/O library uses an internal buffer */
	/* It asks for 1024 characters even if only getc() is used. */
	/* If we use a for(;;) loop on the number of characters requested, */
	/* the user is forced to enter the exact number requested, even if only one is needed. */
	/* So here we return only 1 character even if requested length is > 1 */
	//printf("before *ptr = getchar\n");
	//*ptr = __io_getchar();

	//return 1;
//}

int _read (int file, char *ptr, int len)
{
	printf("\nStarting _read() system call.\n");
    int i;
    /* Check if file descriptor is valid */
    if (file < 0) {
        return -1;
    }
    /* Loop through the requested length */
    for (i = 0; i < len; i++) {
        /* Get input from user */
        *ptr = __io_getchar();
        /* Print character */
        printf("read character: %c\n", *ptr);
        /* Check for end of file */
        if (*ptr == EOF) {
            return i;
        }
        /* Increment pointer to next buffer element */
        ptr++;
    }
    /* Return the number of characters read */
    printf("\n_read system call finished.\n");
	return len;
	
}


int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		__io_putchar(*ptr++);
	}
	return len;
}

caddr_t _sbrk(int incr)
{
	extern char end asm("end");
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0)
		heap_end = &end;

	prev_heap_end = heap_end;
	if (heap_end + incr > stack_ptr)
	{
//		write(1, "Heap and stack collision\n", 25);
//		abort();
		errno = ENOMEM;
		return (caddr_t) -1;
	}

	heap_end += incr;

	return (caddr_t) prev_heap_end;
}

int _close(int file)
{
	return -1;
}


int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _open(char *path, int flags, ...)
{
	/* Pretend like we always fail */
	return -1;
}

int _wait(int *status)
{
	errno = ECHILD;
	return -1;
}

int _unlink(char *name)
{
	errno = ENOENT;
	return -1;
}

int _times(struct tms *buf)
{
	return -1;
}

int _stat(char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}

int _fork(void)
{
	errno = EAGAIN;
	return -1;
}

int _execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}
