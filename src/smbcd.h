#include "smblib.h"
#define SMBCD ".newsmbcd"


typedef struct smbcd_t
{
        char host[MAX_HOST_LEN];
        char share[MAX_SHARE_LEN];
        char dir[1024];
        char username[MAX_USER_LEN];
        char password[MAX_PASSWD_LEN];
	char master[MAX_HOST_LEN];
        int anonymous;
} smbcd_t;

int getcd(smbcd_t *);
int setcd(smbcd_t *);
