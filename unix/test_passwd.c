// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// FileName:     test_passwd.c
// Author:       dingfang
// CreateDate:   2020-11-28 17:47:29
// ModifyAuthor: dingfang
// ModifyDate:   2020-11-28 18:24:08
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <shadow.h>
#include <error.h>
#include <errno.h>

void shadow()
{
#if 0
    struct spwd {
        char *sp_namp;     /* Login name */
        char *sp_pwdp;     /* Encrypted password */
        long  sp_lstchg;   
        long  sp_min;      /* Min # of days between changes */
        long  sp_max;      /* Max # of days between changes */
        long  sp_warn;     
        long  sp_inact;   
        long  sp_expire;   
        unsigned long sp_flag;  /* Reserved */
    };
#endif

    /* 需要root权限可以读取 */

    setspent();
    struct spwd *spwd;
    printf("showad:\n");
    while ((spwd = getspent()) != NULL)
    {
        if (!strcmp(spwd->sp_namp, "dingfang"))
        {
        printf("name:\t\t%s\n", spwd->sp_namp);
        printf("min:\t\t%ld\n", spwd->sp_min);
        printf("max:\t\t%ld\n", spwd->sp_max);
        printf("warn:\t\t%ld\n", spwd->sp_warn);
        printf("inact:\t\t%ld\n", spwd->sp_inact);
        printf("exxpire:\t%ld\n", spwd->sp_expire);
        break;
        }
    }

    printf("%s\n", strerror(errno));

    endspent();

}

void all()
{
    struct passwd *pwd = NULL;

    setpwent();

    while ((pwd = getpwent()) != NULL)
    {
        printf("name:\t(%s)\n", pwd->pw_name);
    }

    endpwent();
}

int getpwuid_test(void)
{
    struct passwd *pwd = NULL;

    pwd = getpwuid(1002);

    if (!pwd)
    {
        printf("error!");
        return -1;
    }

    printf("name:\t(%s)\n", pwd->pw_name);
    printf("passwd:\t(%s)\n", pwd->pw_passwd);
    printf("uid:\t(%d)\n", pwd->pw_uid);
    printf("gid:\t(%d)\n", pwd->pw_gid);
    printf("gecos:\t(%s)\n", pwd->pw_gecos);
    printf("dir:\t(%s)\n", pwd->pw_dir);
    printf("shell:\t(%s)\n", pwd->pw_shell);

    return 0;
}

int main(void)
{
    shadow();
    // all();
    // getpwuid_test();
    return 0;
}
