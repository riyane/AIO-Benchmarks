




// icl -c do_compute.c
// icl aio_sample2.c do_compute.obj
// aio_sample2.exe
#include <aio.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>





#define DIM_Y	10

struct aiocb	my_aio;




int main()
{
	double do_compute(double A, double B, int arr_len);
	int      i, j, res;

	unlink("aio_sample2.dat");
	my_aio.aio_fildes							= open("aio_sample2.dat", O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);;
	my_aio.aio_offset							= -1;
	my_aio.aio_nbytes							= sizeof(DIM_Y+1);
//    my_aio.aio_sigevent.sigev_notify			= SIGEV_THREAD;
//    my_aio.aio_sigevent.sigev_notify_function	= aio_CompletionRoutine;
//    my_aio.aio_sigevent.sigev_signo = SIGALRM;

    /*
     * writing
     */
    printf("Writing\n");
    char c = 'a';
    char data[DIM_Y + 1];

    for (j = 0; j < DIM_Y; j++)
    {
    	data[j] = c;
    	c++;
    }
    data[DIM_Y] = '\n';
    my_aio.aio_buf		= data;
    my_aio.aio_nbytes	= DIM_Y+1;
    res = aio_write(&my_aio);
    if (res) {printf("res!=0\n");abort();}

    //
    // flushing
    //
    /* Wait until completion */
    while (aio_error (&my_aio) == EINPROGRESS);
    printf("End flushing\n");



    close(my_aio.aio_fildes);
    printf("\nDone\n");

    return 0;
}
