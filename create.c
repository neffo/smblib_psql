#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smblib.h"
#include "errors.h"

int smb_setup(smb_shiz *smb)
{
	if (smb)
	{
		memset(&smb,0,sizeof(smb_shiz));
		return 0;
	}
	smb_error(smb,1);
	return 1;
}

int smb_setmaster(smb_shiz *smb, char *master)
{
	if (master)
	{
		strncpy(smb->master,master,MAX_HOST_LEN);
		return 0;
	}
	smb_error(smb,2);
	return 1;
}

int smb_sethost(smb_shiz *smb, char *host)
{
	if (host)
	{
		strncpy(smb->host,host,MAX_HOST_LEN);
		return 0;
	}
	smb_error(smb,2);
	return 1;
}

int smb_setshare(smb_shiz *smb, char *share)
{
	strncpy(smb->share,share,MAX_SHARE_LEN);
	
	return 0;
}
