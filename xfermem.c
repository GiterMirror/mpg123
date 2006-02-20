/*
 *   xfermem.c
 *
 *   Oliver Fromme  <oliver.fromme@heim3.tu-clausthal.de>
 *   Sun Apr  6 02:26:26 MET DST 1997
 *
 *   See xfermem.h for documentation/description.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <fcntl.h>

#include "xfermem.h"

#ifndef USE_MMAP
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

extern int errno;

#if defined (USE_MMAP) && defined(MAP_ANONYMOUS) && !defined(MAP_ANON)
#define MAP_ANON MAP_ANONYMOUS
#endif

void xfermem_init (txfermem **xf, int bufsize)
{
	int regsize = bufsize + sizeof(txfermem);

#ifdef USE_MMAP
#  ifdef MAP_ANON
	if ((*xf = (txfermem *) mmap(0, regsize, PROT_READ | PROT_WRITE,
			MAP_ANON | MAP_SHARED, -1, 0)) == (txfermem *) -1) {
		perror ("mmap()");
		exit (1);
	}
#  else
	int devzero;
	if ((devzero = open("/dev/zero", O_RDWR, 0)) == -1) {
		perror ("open(/dev/zero)");
		exit (1);
	}
	if ((*xf = (txfermem *) mmap(0, regsize, PROT_READ | PROT_WRITE,
			MAP_SHARED, devzero, 0)) == (txfermem *) -1) {
		perror ("mmap()");
		exit (1);
	}
	close (devzero);
#  endif
#else
	struct shmid_ds shmemds;
	int shmemid;
	if ((shmemid = shmget(IPC_PRIVATE, regsize, IPC_CREAT | 0600)) == -1) {
		perror ("shmget()");
		exit (1);
	}
	if ((*xf = (txfermem *) shmat(shmemid, 0, 0)) == (txfermem *) -1) {
		perror ("shmat()");
		shmctl (shmemid, IPC_RMID, &shmemds);
		exit (1);
	}
	if (shmctl(shmemid, IPC_RMID, &shmemds) == -1) {
		perror ("shmctl()");
		exit (1);
	}
#endif
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, (*xf)->fd) < 0) {
		perror ("socketpair()");
		munmap ((caddr_t) *xf, regsize);
		exit (1);
	}
	(*xf)->freeindex = (*xf)->readindex = 0;
	(*xf)->wakeme[0] = (*xf)->wakeme[1] = FALSE;
	(*xf)->data = ((byte *) *xf) + sizeof(txfermem);
	(*xf)->size = bufsize;
}

void xfermem_done (txfermem *xf)
{
#ifdef USE_MMAP
	munmap ((caddr_t) xf, xf->size + sizeof(txfermem));
#else
	if (shmdt((void *) xf) == -1) {
		perror ("shmdt()");
		exit (1);
	}
#endif
}

void xfermem_init_writer (txfermem *xf)
{
	close (xf->fd[XF_READER]);
}

void xfermem_init_reader (txfermem *xf)
{
	close (xf->fd[XF_WRITER]);
}

int xfermem_get_freespace (txfermem *xf)
{
	int readindex;

	if ((readindex = xf->readindex) > xf->freeindex)
		return ((readindex - xf->freeindex) - 1);
	else
		return ((xf->size - (xf->freeindex - readindex)) - 1);
}

int xfermem_get_usedspace (txfermem *xf)
{
	int freeindex;

	if ((freeindex = xf->freeindex) >= xf->readindex)
		return (freeindex - xf->readindex);
	else
		return (xf->size - (xf->readindex - freeindex));
}

int xfermem_write (txfermem *xf, byte *data, int count)
{
	int nbytes;

	if ((nbytes = xfermem_get_freespace(xf))> count)
		nbytes = count;
	if (nbytes) {
		if (xf->freeindex + nbytes > xf->size) {
			int first = xf->size - xf->freeindex;
			memcpy (xf->data + xf->freeindex, data, first);
			memcpy (xf->data, data + first, nbytes - first);
		}
		else
			memcpy (xf->data + xf->freeindex, data, nbytes);
		xf->freeindex = (xf->freeindex + nbytes) % xf->size;
	}
	return (nbytes);
}

int xfermem_read (txfermem *xf, byte *data, int count)
{
	int nbytes;

	if ((nbytes = xfermem_get_usedspace(xf))> count)
		nbytes = count;
	if (nbytes) {
		if (xf->readindex + nbytes > xf->size) {
			int first = xf->size - xf->readindex;
			memcpy (data, xf->data + xf->readindex, first);
			memcpy (data + first, xf->data, nbytes - first);
		}
		else
			memcpy (data, xf->data + xf->readindex, nbytes);
		xf->readindex = (xf->readindex + nbytes) % xf->size;
	}
	return (nbytes);
}

int xfermem_getcmd (int fd, int block)
{
	fd_set selfds;
	struct timeval selto = {0, 0};
	byte cmd;

	FD_ZERO (&selfds);
	FD_SET (fd, &selfds);
	for (;;)
		switch (select(FD_SETSIZE, &selfds, NULL, NULL, block ? NULL : &selto)) {
			case 0:
				if (!block)
					return (0);
				continue;
			case -1:
				if (errno == EINTR)
					continue;
				return (-2);
			case 1:
				if (FD_ISSET(fd, &selfds))
					switch (read(fd, &cmd, 1)) {
						case 0: /* EOF */
							return (-1);
						case -1:
							if (errno == EINTR)
								continue;
							return (-3);
						case 1:
							return (cmd);
						default: /* ?!? */
							return (-4);
					}
				else /* ?!? */
					return (-5);
			default: /* ?!? */
				return (-6);
		}
}

int xfermem_putcmd (int fd, byte cmd)
{
	for (;;)
		switch (write(fd, &cmd, 1)) {
			case 1:
				return (1);
			case -1:
				if (errno != EINTR)
					return (-1);
		}
}

int xfermem_block (int readwrite, txfermem *xf)
{
	int myfd = xf->fd[readwrite];
	int result;

	xf->wakeme[readwrite] = TRUE;
	if (xf->wakeme[1 - readwrite])
		xfermem_putcmd (myfd, XF_CMD_WAKEUP);
	result = xfermem_getcmd(myfd, TRUE);
	xf->wakeme[readwrite] = FALSE;
	return ((result <= 0) ? -1 : result);
}

/* EOF */
