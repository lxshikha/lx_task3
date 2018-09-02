#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

#define LX_ACCELL_DEV "/dev/lxaccell"
#define STR_MAX 30

int main(int argc, char *argv[])
{
    int file = 0, flags = 0, length = 0, sig = 0;
    sigset_t waitset;
    char buffer[STR_MAX];

    /* Check if called with arguments */
    if (argc >= 2) {
        printf("Usage: Script should not be called with any argument\n");
    }

    /* Open the device */
    if ((file = open(LX_ACCELL_DEV, O_RDWR)) >= 0) {
	/* Setup async operation */
        fcntl (file, F_SETOWN, getpid());
        flags = fcntl (file, F_GETFL);
        fcntl (file, F_SETFL, flags | FASYNC);

    } else {
        printf("Failed to open "LX_ACCELL_DEV"\n");
    }
    
    
    sigemptyset( &waitset );
    sigaddset( &waitset, SIGIO );
    sigprocmask( SIG_BLOCK, &waitset, NULL );
    
    printf("Waiting for acceleration data\n");

    /* Never terminates - Unless CTRL-C*/
    while (1) {
        if (!sigwait(&waitset, &sig)){
            if ((sig == SIGIO) && (read(file, buffer, STR_MAX) > 0)) {
                printf(buffer);
            }
	}
	
    }

    return 0;
}
