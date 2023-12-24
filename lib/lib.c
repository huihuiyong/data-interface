#include <semaphore.h>
#include "lib.h"

void SemUnlink(void)
{
    sem_unlink(SENDER_SEM_NAME);
    sem_unlink(RECEIVE_SEM_NAME);
}
