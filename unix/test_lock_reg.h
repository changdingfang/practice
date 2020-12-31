// =======================================================================
//  FileName:     test_lock_reg.h
//  Author:       dingfang
//  CreateDate:   2020-12-28 20:36:08
//  ModifyAuthor: dingfang
//  ModifyDate:   2020-12-31 08:58:40
// =======================================================================

#ifndef __TEST_LOCK_REG_H__
#define __TEST_LOCK_REG_H__


#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>


#define FILE_MODE  (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

static volatile sig_atomic_t sigflag;
static sigset_t newmask, oldmask, zeromask;

static void sig_usr(int signo)
{
    sigflag = 1;
}


static void TELL_WAIT(void)
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
    {
        fprintf(stderr, "signal SIGUSR1 error!\n");
        exit(-1);
    }

    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
    {
        fprintf(stderr, "signal SIGUSR2 error!\n");
        exit(-1);
    }

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
    {
        fprintf(stderr, "signal SIGUSR2 error!\n");
        exit(-1);
    }
}


static void TELL_PARENT(pid_t pid)
{
    kill(pid, SIGUSR2);
}


static void WAIT_PARENT(void)
{
    while (sigflag == 0)
    {
        sigsuspend(&zeromask);
    }
    sigflag = 0;
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    {
        fprintf(stderr, "sigprocmask failed!\n");
        exit(-1);
    }
}


static void TELL_CHILD(pid_t pid)
{
    kill(pid, SIGUSR1);
}


static void WAIT_CHILD(void)
{
    while (sigflag == 0)
    {
        sigsuspend(&zeromask);
    }
    sigflag = 0;

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
    {
        fprintf(stderr, "sigprocmask failed!\n");
        exit(-1);
    }
}


static int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;

    lock.l_type     = type;
    lock.l_start    = offset;
    lock.l_whence   = whence;
    lock.l_len      = len;

    return (fcntl(fd, cmd, &lock));
}


static pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;

    lock.l_type     = type;
    lock.l_start    = offset;
    lock.l_whence   = whence;
    lock.l_len      = len;

    if (fcntl(fd, F_GETLK, &lock) < 0)
    {
        printf("fcntl get lock failed!\n");
        exit(-1);
    }

    if (lock.l_type == F_UNLCK)
    {
        return (0);
    }

    return (lock.l_pid);
}

#define is_read_lockable(fd, offset, whence, len) \
    (lock_test((fd), F_RDLCK, (offset), (whence), (len)))
#define is_write_lockable(fd, offset, whence, len) \
    (lock_test((fd), F_WRLCK, (offset), (whence), (len)))

#define read_lock(fd, offset, whence, len) \
    (lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len)))
#define write_lock(fd, offset, whence, len) \
    (lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len)))
#define writew_lock(fd, offset, whence, len) \
    (lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len)))
#define un_lock(fd, offset, whence, len) \
    (lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len)))



void set_fl(int fd, int flags)
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
    {
        printf("fcntl GET failed!\n");
        exit(-1);
    }

    val |= flags;

    if ((val = fcntl(fd, F_SETFL, 0)) < 0)
    {
        printf("fcntl SET failed!\n");
        exit(-1);
    }
}


#endif /* __TEST_LOCK_REG_H__ */
