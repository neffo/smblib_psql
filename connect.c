#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smblib.h"
#include "errors.h"

int smb_openpipe(smb_shiz *smb, char *command)
{
	if (smb->ppipe)
	{
		smb_error(smb,3);
		return 1;
	}

	smb->ppipe = popen(command,"r");

	if (!smb->ppipe)
	{
		smb_error(smb,4);
		return 1;
	}

	return 0;
}

int smb_setpassword (smb_shiz *smb, char *password)
{
	strncpy(smb->password,password,MAX_PASSWD_LEN);
	return 0;
}

int smb_closepipe (smb_shiz *smb)
{
	if (!smb->ppipe)
	{
		smb_error(smb,5);
		return 1;
	}

	fflush(smb->ppipe);
	pclose(smb->ppipe);
	smb->ppipe=0;
	return 0;
}

int smb_connect_url (smb_shiz *smb, char *url)
{
	char command[1024];
	smb_check_url(url);
	smb_gethost_from_url(url,smb->host);
	smb_getshare_from_url(url,smb->share);
	smb_getdir_from_url(url,smb->dir);

	if (smb->debug)
		fprintf(stderr,"smb_connect_url:\n\tHOST: %s\n\tSHARE: %s\n\tDIR: %s\n",smb->host,smb->share,smb->dir);

	if (!*smb->password)
		sprintf(command,"smbclient \"//%s/%s\" -U guest%% -d 0 -D \"%s\" -c \"dir;quit\"",smb->host,smb->share,smb->dir);
	else
		sprintf(command,"smbclient \"//%s/%s\" -U guest%%%s -d 0 -D \"%s\" -c \"dir;quit\"",smb->host,smb->share,smb->password,smb->dir);

	if (*smb->ip)
                sprintf(command,"%s -I %s",command,smb->ip);

	if (smb->debug)
		fprintf(stderr,"smb_connect_url(): command = %s\n",command);

	return smb_openpipe(smb,command);
}

int smb_connect (smb_shiz *smb, char *host, char *share, char *dir, char *user, char *password)
{
	char command[1024];

	if (*smb->password)
		sprintf(command,"smbclient \"//%s/%s\" -U %s%%%s -d 0 -D \"%s\" -c \"dir;quit\"",host,share,user,password,dir);
	else
		sprintf(command,"smbclient \"//%s/%s\" -U guest%% -d 0 -D \"%s\" -c \"dir;quit\"",host,share,dir);

	if (*smb->ip)
		sprintf(command,"%s -I %s",command,smb->ip);

	if (smb->debug)
		fprintf(stderr,"smb_connect(): %s\n",command);

	return smb_openpipe(smb,command);
}

int smb_connect_recurse ( smb_shiz *smb, char *host, char *share, char *user, char *password)
{
	char command[1024];

	if (*smb->password)
		sprintf(command,"smbclient \"//%s/%s\" -U %s%%%s -d 0 -c \"more;recurse;dir;more;quit\"",host,share,user,password);
	else
		sprintf(command,"smbclient \"//%s/%s\" -U guest%% -d 0 -c \"more;recurse;dir;more;quit\"",host,share);

	if (*smb->ip)
		sprintf(command,"%s -I %s",command,smb->ip);

	if (smb->debug)
		fprintf(stderr,"smb_connect_recurse(): %s\n",command);

	return smb_openpipe(smb,command);
}

int smb_init_workgroups (smb_shiz *smb)
{
	char command[128];
	
	if (smb->master)
		sprintf(command,"smbclient -U guest%% -d 0 -L \"%s\"",smb->master);
	else
		sprintf(command,"smbclient -L serverbox");

	return smb_openpipe(smb,command);
}

int smb_init_machines ( smb_shiz *smb, char *wgmaster )
{
	char command[128];

	if (smb->debug)
		fprintf(stderr,"smb_init_machines(): wgmaster = %s\n",wgmaster);

	if (wgmaster)
		sprintf(command,"smbclient -U guest%% -d 0 -L \"%s\"",wgmaster);
	else
		return 1;

	return smb_openpipe(smb,command);
}

int smb_init_shares ( smb_shiz *smb, smb_machine * machine)
{
	char command[128];

	if (smb->debug)
		fprintf(stderr,"smb_init_shares(): %s\n",machine->name);

	if (machine && machine->name[0])
	{
		//if (machine->ip[0])
		//	sprintf(command,"smbclient -U %% -d 0 -I %s -L \"%s\"",machine->ip,machine->name);
		//else
		strcpy(command,"smbclient");

		if (*smb->ip)
			sprintf(command,"%s -I %s",command,smb->ip);

		/*if (*smb->password)
			snprintf(command,128,"%s*/
		
		if (*smb->password)
			sprintf(command,"%s -U guest%%%s -d 0 -L \"%s\"",command,smb->password,machine->name);
		else
			sprintf(command,"%s -U guest%% -d 0 -L \"%s\"",command,machine->name);
		
		if (smb->debug)
			fprintf(stderr,"DEBUG: command = %s\n",command);
	}
	else
		return 1;

	return smb_openpipe(smb,command);
}

int smb_checkurl(char *url)
{
	return 0;
}
