#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smblib.h"

int smb_get_workgroups(smb_shiz *smb)
{
	static char line[1024];

	line[0]=0;

	readline_wg:	
	fgets(line,254,smb->ppipe);

	if (line[0]==0)
		return 1;
	
	if (strstr(line,"failed"))
		return 1;

	if (!strstr(line,"Workgroup"))
		goto readline_wg;
	fgets(line,254,smb->ppipe);

	return 0;
}

int smb_get_next_workgroup( smb_shiz *smb, smb_workgroup *group)
{
	static char line[1024];
	char wg[35],wgmaster[35];

	line[0]=0;
	group->name[0]=0;
	group->master[0]=0;

	fgets(line,254,smb->ppipe);
	if (strlen(line)<4)
		return 1;

	smb_striptabs(line);
	smb_split(line,wg,wgmaster,21);
//	printf("LINE: %s\n",line);
	strncpy(group->name,wg,25);
	strncpy(group->master,wgmaster,25);

	return 0;
}

int smb_get_machines ( smb_shiz *smb )
{
	static char line[1024];

	line[0]=0;

	readline_mach:
	
	fgets(line,1023,smb->ppipe);

	if (smb->debug)
		printf("LINE: %s",line); 

	if (line[0]==0)
		return 1;

	if (strstr(line,"failed"))
		return 1;

	if (!(strstr(line,"Comment") && strstr(line,"Server")))
		goto readline_mach;

	fgets(line,1023,smb->ppipe);

	return 0;
}

int smb_get_next_machine (smb_shiz *smb, smb_machine *mach)
{
	static char line[1024];
	static char machine[35];

//	if (smb->debug)
//		printf("smb_get_next_machine() : %s\n",smb->master);

	mach->name[0]=0;
//	mach->ip[0]=0; // FIXME

	fgets(line,254,smb->ppipe);
	if (strlen(line)<4)
		return 1;

	smb_striptabs(line);
	smb_split(line,machine,0,21);
	strcpy(mach->name,machine);

	return 0;
}

int smb_get_shares ( smb_shiz *smb)
{
        static char line[1024];

	if (smb->debug)
		printf("smb_get_shares()\n");

        line[0]=0;

	if (!smb->ppipe)
	{
		if (smb->debug)
			printf("NULL pipe\n");
		return 1;
	}

        readline_mach:

        fgets(line,1023,smb->ppipe);

	if (smb->debug)
		printf("DEBUG: %s\n",line);
	
	if (line[0]==0)
		return 1;

	if (strstr(line,"failed"))
		return 1;

	if (!(strstr(line,"Comment") && strstr(line,"Sharename")))
		goto readline_mach;

	fgets(line,1023,smb->ppipe);
	
	return 0;                                                                                              return 0;
}

int smb_get_files( smb_shiz *smb)
{
	static char line[1024];

	if (smb->debug)
		fprintf(stderr,"smb_get_files()\n");

	line[0]=0;

	if (!smb->ppipe)
	{
		if (smb->debug)
			fprintf(stderr,"smb_get_files(): NULL pipe\n");
		return 1;
	}

	readline_files:

	fgets(line, 1023, smb->ppipe);

	if (smb->debug)
		fprintf(stderr,"DEBUG: %s\n",line);

	if (!*line)
		return 1;

	if (strstr(line,"ERRDOS"))
		return 1;

	if (strstr(line,"ERRSRV"))
		return 1;

	if (strstr(line,"more <filename>"))
		return 0;

	goto readline_files;

}

int smb_get_next_share (smb_shiz *smb, smb_share *share)
{
        static char line[1024];

	if (smb->debug)
                printf("smb_get_next_share()\n");

        share->name[0]=0;

        fgets(line,254,smb->ppipe);
	
	if (smb->debug)
		printf("DEBUG: %s",line);

        if (strlen(line)<4)
                return 1;

	if (strstr(line,"Error returning"))
		return 1;

	if (strstr(line,"failed"))
		return 1;

	if (strstr(line,"ERRnoaccess"))
		return 1;

        smb_striptabs(line);
        smb_split(line,share->name,0,15);

        return 0;
}

void smb_striptabs(char *string)
{
	unsigned int i;

	for(i=0;i<strlen(string);i++)
	{
		if (string[i]=='\t')
		{
			string[i]=' ';
		}
	}
}

void smb_split(char *string, char *part1, char *part2, int splitchar)
{
	int i=0;
	int j=0;
	int last=0;

	while (string[i]==' ')
	{
		i++;
	}
	
	while (string[i] && i < splitchar)
	{
		part1[j]=string[i];
		if (string[i] != ' ')
			last = j;
		i++;
		j++;
	}
	
	
	part1[last+1]=0;
	i=splitchar;
	if (part2)
	{
		while (string[i]==' ')
		{
			i++;
		}
		j=0;
		while (string[i]!=' ' && string[i]!='\n' && string[i] )
		{
			part2[j]=string[i];
			i++;
			j++;
		}
		part2[j]=0;
	}
}

int smb_get_wgmaster(smb_shiz *smb, char *group, char *master)
{
	static smb_workgroup wg;
	int found;
	// we need the pipe to be free
	if (smb->ppipe)
		return 1;
	master[0]=0;

	smb_init_workgroups(smb);

	if (smb_get_workgroups(smb))
		return 1;

	found=0;
	while (smb_get_next_workgroup(smb,&wg) == 0)
	{
		if (strcasecmp(wg.name,group)==0)
		{
			found++;
			strcpy(master,wg.master);
		}
	}
	
	smb_closepipe(smb);

	return found;	

}

// return 0 if not host or workgroup
// return 1 if host
// return 2 if workgroup
int smb_check_hostname(smb_shiz *smb, char *hostname)
{
	static char command[1024];
	static char line[256];
	int found = 0;
	smb_workgroup group;

	sprintf(command,"smbclient -d 0 -N -L \"%s\"",hostname);

	smb_setmaster(smb,"serverbox");
	
	smb_openpipe(smb,command);	

	readline_chkhost:
	line[0]=0;
	fgets(line,255,smb->ppipe);
//	printf("test %s\n",line);

	if (line[0])
	{
		if (strstr(line,"failed"))
			found=-1;
		goto readline_chkhost;
	}
	smb_closepipe(smb);

	if (!found)
		return 1;

	smb_init_workgroups(smb);

	smb_get_workgroups(smb);

	group.name[0]=0;
	group.master[0]=0;

//	printf("blah\n");
	
	while ( smb_get_next_workgroup(smb,&group)==0 )
	{
//		printf("NAME: %s\n",group.name);
		if ( strcasecmp(group.name,hostname)==0 )
			found=2;
	}
	smb_closepipe(smb);
	return found;
}

/*
char *hostnametoip(char *hostname, char *ip)
{
	char line[256];
	char cmd[256];
	int i=0;

	// 012345678901234
	// 255.255.255.255 = longest possible ip
	#define MAXIPLENGTH 15
	

	char *nmblookup="/usr/bin/nmblookup -d 0 ";

	FILE *fpipe;

	sprintf(cmd,"%s %s",nmblookup,hostname);

	fpipe=popen(cmd,"r");

	while(fgets(line,254,fpipe) && (!strstr(line,"query")))
	{
		//bleh
	}

	fgets(line,254,fpipe);

	while ( line[i]!=' ' && i < MAXIPLENGTH )
	{
		ip[i]=line[i];
		i++;
	}
	ip[i]='\0';

	//fprintf(stderr,"ip of %s = \"%s\"\n",hostname,ip);

	fflush(fpipe);
	pclose(fpipe);

	return ip;

}	
*/
/*
char *getipfrommachinelist(machine *first, char *hostname)
{
	machine *cur=0;
	char *ip=0;

	cur=first;

	while (cur)
	{
		if (strcmp(hostname,cur->host)==0)
			ip=cur->ip;
		cur=cur->next;
	}

	return ip;
}
*/	
	
int smb_name2ip(smb_shiz *smb,char *hostname, char *ip)
{
	static char word[20];
	static char line[80];
	static char command[80];

	ip[0]=0;

	if (hostname)
		snprintf(command,80,"nmblookup \"%s\"",hostname);
	else
		snprintf(command,80,"nmblookup -M -");
	
	smb->ppipe = popen(command,"r");

	// FIXME: add proper error checking shiznits

	if (!smb->ppipe)
		return 1;

	while (fgets(line,80,smb->ppipe))
	{
		line[20]=0;
//		printf("line: %s\n",line);
		getword(line,word);
//		printf("word: %s\n",word);
		if (strchr(word,'.')<(&word+10))
			strcpy(ip,word);
	}
	
//	fflush(ppipe);
	smb_closepipe(smb);

	if (smb->debug)
		printf("IP: %s %s\n",hostname,ip);
	
	if (!*ip)
		return 1;
	return 0;
}
