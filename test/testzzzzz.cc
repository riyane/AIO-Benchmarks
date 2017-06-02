




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

#define DIM_X	10
#define DIM_Y	5


static volatile int aio_flg = 1;

struct aiocb	my_aio;
#define WAIT { while (!aio_flg); aio_flg = 0; }

static void aio_CompletionRoutine(sigval_t sigval)
{
    aio_flg = 1;
}

int main()
{
    double do_compute(double A, double B, int arr_len);
    int      i, j, res;

    my_aio.aio_fildes							= open("aio_sample2.dat", O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);;
    my_aio.aio_nbytes							= sizeof(DIM_Y+1);
    my_aio.aio_sigevent.sigev_notify			= SIGEV_THREAD;
    my_aio.aio_sigevent.sigev_notify_function	= aio_CompletionRoutine;

    /*
    ** writing
    */
    my_aio.aio_offset = -1;
    printf("Writing\n");
    char c = 'a';
    char data[DIM_Y +1];

    for (j = 0; j < DIM_Y; j++)
    {
    	data[j] = c;
    	c++;
    }
    data[DIM_Y] = '\n';
    my_aio.aio_buf = data;
    res = aio_write(&my_aio);
    if (res) {printf("res!=0\n");abort();}

/*
    for (i = 0; i < DIM_X; i++)
    {
        for (j = 0; j < DIM_Y; j++)
        {
        	data[j] = c;
        	c++;
        }
        data[DIM_Y] = '\n';
        WAIT;
        my_aio.aio_buf = memcpy(my_aio.aio_buf, data, DIM_Y+1);
        res = aio_write(&my_aio);
        if (res) {printf("res!=0\n");abort();}
    }
*/
    //
    // flushing
    //
    printf("Flushing\n");
    WAIT;
    res = aio_fsync(O_SYNC, &my_aio);
    if (res) {printf("res!=0\n");abort();}
    WAIT;
/*
    //
    // reading
    //
    printf("Reading\n");
    my_aio.aio_offset = 0;
    my_aio.aio_buf    = (volatile char*)aio_dat_tmp;
    for (i = 0; i < DIM_X; i++) {
        aio_read(&my_aio);
        for (j = 0; j < DIM_Y; j++)
            aio_dat[j] = do_compute(i, j, DIM_X);
        WAIT;
        res = aio_return(&my_aio);
        if (res != sizeof(aio_dat)) {
            printf("aio_read() did read %d bytes, expecting %d bytes\n", res, sizeof(aio_dat));
        }

        for (j = 0; j < DIM_Y; j++)
            if ( aio_dat[j] != aio_dat_tmp[j] )
                {printf("ERROR: aio_dat[j] != aio_dat_tmp[j]\n I=%d J=%d\n", i, j); abort();}
        my_aio.aio_offset += my_aio.aio_nbytes;
    }

    CloseHandle(aio_fildes);
*/
    printf("\nDone\n");

    return 0;
}
