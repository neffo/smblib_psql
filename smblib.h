#ifndef _SMBLIB_
#define _SMBLIB_

/*
 * The latest attempt to create a usable samba library thing.
 * 
 */

#define MAX_HOST_LEN 35
#define MAX_IP_LEN 20
#define MAX_SHARE_LEN 30
#define MAX_DIR_LEN 1024
#define MAX_USER_LEN 25
#define MAX_PASSWD_LEN 30

typedef struct smb_shiz
{
	FILE *ppipe;
	char host[MAX_HOST_LEN];
	char ip[MAX_IP_LEN];
	char master[MAX_HOST_LEN];
	char share[MAX_SHARE_LEN];
	char dir[MAX_DIR_LEN];
	char username[MAX_USER_LEN];
	char password[MAX_PASSWD_LEN];
	int dir_change;
	int dirlevel;
	int cache_ip;
	int errnum;
	int debug;
} smb_shiz;

#define MODE_DIR 0x00001

#define URL_LISTWORKGROUPS 	1
#define URL_LISTMACHINES	2
#define URL_LISTSHARES		3
#define URL_LISTDIR		4

typedef struct smb_dent
{
	char filename[1024];
	size_t size;
	int mode;
	// date and other shiznits
	// here
} smb_dent;

typedef struct smb_workgroup
{
	char name[35];
	char master[35];
} smb_workgroup;

typedef struct smb_machine
{
	char name[35];
	char ip[15];
} smb_machine;

typedef struct smb_share
{
	char name[35];
	char comment[50];
} smb_share;

// create.c
int smb_setup(smb_shiz *smb);
int smb_setmaster(smb_shiz *smb, char *master);
int smb_sethost(smb_shiz *smb, char *host);
int smb_setshare(smb_shiz *smb, char *share);

// connect.c
int smb_openpipe(smb_shiz *smb, char *command);
int smb_closepipe(smb_shiz *smb);
int smb_connect_url(smb_shiz *smb,char *url);
int smb_connect(smb_shiz *, char *host, char *share, char *dir, char *, char *password);
int smb_connect_recurse ( smb_shiz *smb, char *host, char *share, char *user, char *password);
int smb_checkurl(char *url);
int smb_init_workgroups(smb_shiz *smb);
int smb_init_machines(smb_shiz *, char *);
int smb_init_shares(smb_shiz *, smb_machine *);
int smb_setpassword (smb_shiz *smb, char *password);

// dirlist.c
int smb_get_next_file(smb_shiz *smb, smb_dent *file);
int smb_get_next_file_r(smb_shiz *smb, smb_dent *file, char *dir); // cd = directory changed

// errors.c
void smb_error(smb_shiz *smb, int errnum);
void smb_printerror(smb_shiz *smb);

// string.c
int smb_gethost_from_url(char *url, char *host);
int smb_getshare_from_url(char *url, char *share);
int smb_getdir_from_url(char *url, char *dir);
int smb_line_to_smbdent(char *line, smb_dent *file);
int smb_url_type(char *url);
int smb_check_url(char *url);
int getword(char *line, char *word);
int escape_badchars(char *string, int size);
int get_dir_from_line(char *line, char *dir);

// machlist.c
void smb_striptabs ( char *);
void smb_split(char *, char *, char *, int);
int smb_get_workgroups ( smb_shiz *);
int smb_get_next_workgroup ( smb_shiz *, smb_workgroup *);
//int smb_init_machines( smb_shiz *, char *);
int smb_get_machines(smb_shiz *);
int smb_get_next_machine(smb_shiz *, smb_machine *);
int smb_get_shares(smb_shiz *);
int smb_get_next_share(smb_shiz *, smb_share *);
int smb_get_wgmaster(smb_shiz *, char *, char *);
int smb_check_hostname(smb_shiz *, char *);
int smb_name2ip(smb_shiz *smb,char *hostname, char *ip);
int smb_get_files( smb_shiz *smb);

void smb_ffs(char *,size_t);
#endif
