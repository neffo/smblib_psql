#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/time.h>
#include <unistd.h>
#include "smblib.h"
#include "errors.h"

int smb_leach_dir(smb_shiz *smb);

int smb_parse_leach_file(smb_shiz *smb, char *filename);

int main ( void )
{
	smb_shiz smb;

	smb_setup(&smb);

	smb.debug = 0;

	while (1)
	{
		smb_leach_dir(&smb);
		sleep(60);
	}

	return 0;
}

int smb_leach_dir(smb_shiz *smb)
{
	DIR *de;
	struct dirent *di;

	de = opendir(TOLEACHDIR);

	if (!de)
	{
		fprintf(stderr,"ERROR: unable to open leach directory\n");
		return 1;
	}

	while ( di = readdir(de) )
	{
		if (*di->d_name != '.')
		{
			printf("Parsing %s...\n",di->d_name);
			smb_parse_leach_file(smb,di->d_name);
		}
	}

	closedir(de);

	return 0;
}

#define BUFSIZE 32768

int smb_parse_leach_file(smb_shiz *smb, char *filename)
{
	char buf[BUFSIZE]; //
	char line[512];
	char ffn[256];
	char dfn[256];
	char *data[5];
	char *p;
	int i;
	size_t total;
	size_t smb_in;
	struct timeval ts;
	struct timeval te;
	long tt;
	double rate;
	FILE *f;

	sprintf(ffn,"%s/%s",TOLEACHDIR,filename);
	
	//printf("FILE: %s\n",ffn);
	
	f = fopen(ffn,"r");

	if (!f)
	{
		fprintf(stderr,"ERROR: unable to open %s\n",ffn);
		return 1;
	}
	
	fgets(line,512,f);

	line[strlen(line)-1]=0;

	p = &line[0];
	i = 0;

	fclose(f);

	while (*p)
	{
		if (*p == '|')
		{
			*p = 0;
			data[i] = p+1;
			i++;
		}
		p++;
	}

	sprintf(dfn,"/mnt/uber/leached/%s",data[3]);

	f = fopen(dfn,"w+");

	//printf("Getting %s (%d)\n",data[3],atoi(data[4]));
	
	if (smb_init_download(smb,data[0],data[1],data[2],data[3]))
	{
		printf("Download aborted.\n");
		return 1;
	}

	total = 0;

	gettimeofday(&ts,0);

	while (smb_in = smb_download_read(smb,buf,BUFSIZE))
	{
		total += fwrite(buf,1,smb_in,f);
	}

	fclose(f);
	smb_closepipe(smb);

	gettimeofday(&te,0);

	if (total < atoll(data[4]))
		printf("WARNING: Possible incomplete download, will retry.\n");
	else
		unlink(ffn); // remove .toleach file from batch directory

	tt = te.tv_sec - ts.tv_sec;

	tt *= 1000000;
	tt += te.tv_usec - ts.tv_usec;

	rate = ((double)total/(double)tt)/1.024;

	printf("Time : %5.2fseconds Rate : %8.3fMB/s\n",(double)tt/1000000.0,rate);
	
	return 1;
}

