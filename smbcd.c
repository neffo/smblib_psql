#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smbcd.h"

int getcd(smbcd_t *cd)
{
	char filename[80];
	FILE *cdfile;

	snprintf(filename,80,"%s/%s",getenv("HOME"),SMBCD);
	
	cdfile = fopen(filename,"r");

	if (cdfile == NULL)
		return(1);

	fread(cd,sizeof(smbcd_t),1,cdfile);
	
	fclose(cdfile);

	return 0;
}

int setcd(smbcd_t *cd)
{
	char filename[80];
	FILE *cdfile;

	snprintf(filename,80,"%s/%s",getenv("HOME"),SMBCD);
	
	cdfile = fopen(filename,"w+");

	if (cdfile == NULL)
		return(1);

	fwrite(cd,sizeof(smbcd_t),1,cdfile);

	fclose(cdfile);
	return 0;
}
