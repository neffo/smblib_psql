#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smblib.h"
#include "errors.h"

int smb_init_download ( smb_shiz *smb, char *host, char *share, char *dir, char *file )
{
	char command[256];

	char *end;

	sprintf(command,"smbclient //%s/%s -d 0 -E -U guest%% -D \"%s\" -c \"get \\\"%s\\\" -;quit\" ",host,share,dir,file);

	if (smb->debug)
		fprintf(stderr,"smb_init_download (%s) : cmd %s\n",file,command);

	return smb_openpipe(smb,command);
}


int smb_start_download ( smb_shiz *smb )
{

	return 0;
}


int smb_download_read ( smb_shiz *smb, char *buf, int size)
{
	//int sz;

	return fread ( buf, 1, size, smb->ppipe);

	//printf("Read %d \n",sz);

	//return sz;
}
