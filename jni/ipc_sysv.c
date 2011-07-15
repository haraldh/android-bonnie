/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/* see the implementation of __set_tls and pthread.c to understand this
 * code. Basically, the content of gs:[0] always is a pointer to the base
 * address of the tls region
 */
#include <stdarg.h>
#include "sem.h"
#include <asm-generic/ipc.h>

#include <sys/linux-syscalls.h>
#define __NR_ipc			(__NR_SYSCALL_BASE+117)

//#include <sys/linux-unistd.h>
#define ipc(call, first, second, third, ptr, fifth) syscall(__NR_ipc, call, first, second, third, ptr, fifth)

int semctl (int  semid, int  semnum, int  cmd, ...)
{
    union semun arg;
    va_list ap;

    va_start(ap, cmd);

    /* Get the argument only if required. */
    arg.buf = NULL;
    switch (cmd) {
        case SETVAL:
        case GETALL:
        case SETALL:
        case IPC_STAT:
        case IPC_SET:
        case SEM_STAT:
        case IPC_INFO:
        case SEM_INFO:
            va_start(ap, cmd);
            arg = va_arg (ap, union semun);
            va_end(ap);
            break;
    }

    va_end(ap);

    return ipc(SEMCTL, semid, semnum, cmd, &arg, (long)NULL);
}

int semget (key_t  key, int  nsems, int  semflg)
{
    return ipc(SEMGET, key, nsems, semflg, (void*)NULL, (long)NULL);
}

int semop (int  semid, struct sembuf*  sops, size_t  nsops)
{
    return ipc(SEMOP, semid, (int)nsops, 0, sops, (long)NULL);
}


