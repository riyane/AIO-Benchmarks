#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <aio.h>

#include "util.h"



#define DIM_X	10
#define DIM_Y	10

static volatile int aio_flg = 0;

static void aio_CompletionRoutine(sigval_t sigval)
{
	printf("Handler routine\n");
	aio_flg = 1;
}


int main()
{
	char tmpfname[256];
	char *buf[DIM_X];
//	char check[DIM_Y+2];
	int fd;
	struct aiocb aiocb;
	int err;
	int ret;

	snprintf(tmpfname, sizeof(tmpfname), "pts_aio_write.data");
	unlink(tmpfname);
	fd = open(tmpfname, O_CREAT | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
	if (fd == -1)
		LOGGER_error("Error at open()", strerror(errno), 1);

//  unlink(tmpfname);

	memset(&aiocb, 0, sizeof(struct aiocb));
	aiocb.aio_fildes							= fd;
	aiocb.aio_nbytes							= DIM_X;
	aiocb.aio_sigevent.sigev_notify				= SIGEV_THREAD;
	aiocb.aio_sigevent.sigev_notify_function	= aio_CompletionRoutine;

	char c = 'a';
	int i, j;
	for (i=0; i<DIM_Y; i++)
	{
		for (j=0; j<DIM_Y; j++)
		{

		}
	}
	aiocb.aio_buf								= buf;
	if (aio_write(&aiocb) == -1)
	{
		close(fd);
		LOGGER_error("Error at aio_write()", strerror(errno), 1);
	}

	/* Wait until completion */
	while (aio_error (&aiocb) == EINPROGRESS);

	err = aio_error(&aiocb);
	ret = aio_return(&aiocb);

	if (err != 0)
	{
		close (fd);
		LOGGER_error(" Error at aio_error()", strerror (err), 1);
	}

	if (ret != BUF_SIZE)
	{
		close (fd);
		LOGGER_error(" Error at aio_return()\n", "", 1);
	}

	/* check the values written */

	if (lseek(fd, 0, SEEK_SET) == -1)
	{
		close(fd);
		LOGGER_error("Error at lseek(): %s\n", strerror(errno), 1);
	}

	/* we try to read more than we wrote to be sure of the size written */

	check[BUF_SIZE] = 1;
	if (read(fd, check, BUF_SIZE + 1) != BUF_SIZE)
	{
		close(fd);
		LOGGER_error(" Error at read()", strerror(errno), 1);
	}

	if ( check[BUF_SIZE] != 1)
	{
		close(fd);
		LOGGER_error(" Buffer overflow\n", "", 1);
	}

	if (memcmp(buf, check, BUF_SIZE))
	{
		close(fd);
		LOGGER_error(" Bad value in buffer", "", 1);
	}

	close(fd);
	printf ("Test PASSED\n");
	return 0;
}
