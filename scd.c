#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smblib.h"
#include "smbcd.h"

void print_cd( smbcd_t *);

int set_cd_rel(smbcd_t *cd, char *);
int set_cd_abs(smbcd_t *cd, char *);

int main ( int varc, char **var)
{
	smbcd_t cd;
	//smb_shiz smb;

	getcd(&cd);

	if (!var[1])
	{
		print_cd(&cd);
		exit(0);
	}

	if (strcmp(var[1],"..")==0)
	{
//		if (set_cd_parent(&cd,var[1]))
//			setcd(&cd);
	}
	
	if (strchr(var[1],'/'))
	{
		// absolute
//		printf("absolute\n");
		if (set_cd_abs(&cd,var[1]))
			setcd(&cd);
	} 
	else
	{
		// relative
//		printf("relative\n");
		if (set_cd_rel(&cd,var[1]))
			setcd(&cd);
	}
		
	exit(0);
}

int set_cd_abs(smbcd_t *cd, char *url)
{
//	printf("url : %s\n",url);
	cd->host[0]=0;
	cd->share[0]=0;
	cd->dir[0]=0;
//	printf("host: -%s- share: -%s- dir: -%s-\n",cd->host,cd->share,cd->dir);
	if (smb_gethost_from_url(url,cd->host)==0)
	{
//		printf("gethost(): host: -%s- share: -%s- dir: -%s-\n",cd->host,cd->share,cd->dir);
		if (smb_getshare_from_url(url,cd->share)==0)
		{
//			printf("getshare(): host: -%s- share: -%s- dir: -%s-\n",cd->host,cd->share,cd->dir);
			smb_getdir_from_url(url,cd->dir);
//			printf("getdir(): host: -%s- share: -%s- dir: -%s-\n",cd->host,cd->share,cd->dir);
		}
		else
		{
			cd->dir[0]=0;
		}
		
//		printf("host: -%s- share: -%s- dir: -%s-\n",cd->host,cd->share,cd->dir);
		return 1;
	}
	else
	{
		return 0;
	}
}

int set_cd_rel(smbcd_t *cd, char *cmd)
{
	char dir[1024];

	sprintf(dir,"//%s",cd->host);
	if (cd->share[0])
	{
		strcat(dir,"/");
		strcat(dir,cd->share);
		if (cd->dir[0])
		{
			strcat(dir,"/");
			strcat(dir,cd->dir);
		}
		
	}

	strcat(dir,"/");
	strcat(dir,cmd);

	printf("%s\n",dir);

	return set_cd_abs(cd,dir);

	//return 0;
}

void print_cd(smbcd_t *cd)
{
	printf("//");
	if (cd->host[0])
	{
		printf("%s",cd->host);		
		printf("/");
	
		if (cd->share[0])
		{
			printf("%s",cd->share);
			printf("/");

			if (cd->dir[0])
			{
				printf("%s",cd->dir);
			}
		}
	}
	printf("\n");
}
	
