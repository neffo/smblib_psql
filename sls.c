#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smblib.h"
#include "smbcd.h"

int listdir(smb_shiz *, smbcd_t *, char *);
int listwg(smb_shiz *, smbcd_t *, char *, char *);


int main (int varc, char **var)
{
	smbcd_t cd;
//	char url[1024];
	int hosttype;
	smb_shiz smb;

	smb_setup(&smb);

//	smb_setmaster(&smb,"serverbox");

	getcd(&cd);

//	printf("%s %s %s\n",cd.host,cd.share,cd.dir);

	smb_setmaster(&smb,cd.master);

	//if (!var[1])
//	{
//		fprintf(stderr,"No host specified.\n");
//		exit(1);
//	}

//	strcpy(cd.host,var[1]);
//	strcpy(cd.share,"");
//	strcpy(cd.dir,"");
/*	strcpy(cd.username,"");
	strcpy(cd.password,"");
	strcpy(cd.master,"serverbox");
	cd.anonymous=1;
*/

	hosttype = smb_check_hostname(&smb,cd.host);

	if (hosttype == 1)
		listdir(&smb,&cd,var[1]);
	else if (hosttype == 2)
		listwg(&smb,&cd,cd.host,var[1]);
	else
		fprintf(stderr,"blah\n");

	exit(0);
}

int listdir(smb_shiz *smb, smbcd_t *cd, char *mask)
{
	static char ffs[15];
	smb_dent file;
//	smb_shiz smb;
	
//	smb_setup(smb);

	if (cd->master[0])
		smb_setmaster(smb,cd->master);

	if (smb_connect(smb,cd->host,cd->share,cd->dir,0,0))
	{
		fprintf(stderr,"sls.c: smb_connect() failed.\n");
		goto end_listdir;
	}

	while (smb_get_next_file(smb,&file)==0)
	{
		if (*file.filename)
		{
			smb_ffs(ffs,file.size);
			printf("%c[01;%dm%5s%c[00m %s\n",27,file.mode?34:32,file.mode?"[DIR]":ffs,27,file.filename);
		}
	}

	end_listdir:
	smb_closepipe(smb);
	
	return 0;
}

int listwg(smb_shiz *smb, smbcd_t *cd, char *host, char *mask)
{
	static char master[MAX_HOST_LEN];
	smb_machine machine;
//	smb_shiz smb;

//	smb_setup(smb); // doesn't bad shit

	if (cd->master[0])
		smb_setmaster(smb,cd->master);

	smb_get_wgmaster(smb,host,master);

	smb_init_machines(smb,master);

	smb_get_machines(smb);
	
	while (smb_get_next_machine(smb,&machine)==0)
	{
		printf("//%c[01;34m%s%c[00m/\n",27,machine.name,27);
	}

	return 0;
}
