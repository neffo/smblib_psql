#include <stdio.h>
#include <stdlib.h>
#include "smblib.h"
#include "errors.h"

#define BUFSIZE 32768

int main (void)
{
	FILE *bleh;
	int size;
	size_t total;
	smb_shiz smb;

	char buf[BUFSIZE];

	smb_setup(&smb);

	smb.debug = 1;

	smb_init_download(&smb,"GOTWANG","Movies","","Southpark -  A Ladder to Heaven.rm");

	bleh = fopen("SP.rm","w+");

	total = 0;

	size = 1;
	
	while ( size > 0 )
	{
		size = smb_download_read( &smb, buf, BUFSIZE);
		if (size)
			total += fwrite(buf,1,size,bleh);
	}
	
	fclose(bleh);

	smb_closepipe(&smb);

//	printf("TOTAL WRITTEN = %d\n",total);

	exit(0);
}
