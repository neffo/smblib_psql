#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "smblib.h"
#include "string.h"

int main ( void )
{
//	smbshiz smb;
	smb_dent file;

	char url[4][1024];
	char master[35];
	char ip[20];
//	workgroup wg;
	smb_machine machine[40];
	smb_workgroup group[40];
	smb_share share[40];
	int i;
	int wgcount;
	int machcount;
	int sharecount;
	int err;
	smb_shiz smb;

	//machine = machine2;

//	url = (char *)malloc(1024);
//	group = malloc(sizeof(workgroup)*40);

	

	strcpy(url[0],"//STUPID/test/Files/amiga/games/ADFs");
	strcpy(url[1],"//Little one/DOWNLOAD/d2 exp/Diablo2 Asia Charms_files");
	strcpy(url[2],"//stupid/movies/jackass/bleh ' shiz/");
	strcpy(url[3],"//bleh/bleh");

	escape_badchars(url[2],0);

	printf("FIXED? %s\n",url[2]);

	smb_setup(&smb);
	smb.debug = 1;
	
	smb_setmaster(&smb,"serverbox");
//	smb_setpassword(&smb,"guest");

	smb_name2ip(&smb,0,ip);

	printf("MASTER = %s\n",ip);
	
	if (0) // don't do
	for (i=0;i<4;i++)
	{	

		printf("URL # %d: %s\n",i,url[i]);	

		err = smb_connect_url(&smb,url[i]);

		printf("ERROR: %d\n",err);
		//free(url);

		while ( smb_get_next_file(&smb,&file) == 0 )
		{
			printf("%s: %s %d \n",file.mode ? "DIR ":"FILE",file.filename,file.size);
		}

		smb_closepipe(&smb);
	}

	smb_get_wgmaster(&smb,"EARTH",master);
	
	printf("EARTH MASTER = %s\n",master);
	
	smb_init_workgroups(&smb);
	smb_get_workgroups(&smb);
	printf("\n");
	i=0;
	wgcount=0;
	while ( smb_get_next_workgroup(&smb,&group[i]) == 0 )
	{
//		printf("NAME: %s -> MASTER: %s\n",group[i].name,group[i].master);
		i++;
		wgcount++;
	}
	smb_closepipe(&smb);
	machcount=0;
	sharecount=0;
	for(i=0;i<wgcount;i++)
	{
		printf("WORKGROUP: %s %s\n",group[i].name,group[i].master);
		smb_init_machines(&smb,group[i].master);
		smb_get_machines(&smb);
		while ( smb_get_next_machine(&smb,&machine[machcount]) == 0 )
		{
			printf("|- - |%s|\n",machine[machcount].name);
			machcount++;
		}
		smb_closepipe(&smb);
	}
	sharecount++;
/*
	for(i=0;i<machcount;i++)
	{
		printf("SHARES ON %s\n",machine[i].name);
		smb_init_shares(&smb,&machine[i]);
		smb_get_shares(&smb);
		while ( smb_get_next_share(&smb,&share[0]) == 0 )
		{
			printf("share = %s\n",share[0].name);
			sharecount++;
		}
		smb_closepipe(&smb);
	}
*/
	printf("host-type: %d\n",smb_check_hostname(&smb,"little one"));
	printf("host-type: %d\n",smb_check_hostname(&smb,"EArTH"));
	printf("host-type: %d\n",smb_check_hostname(&smb,"BLEH"));

/*
	smb_init_shares(&smb,&machine[machcount-1]);
	smb_get_shares(&smb);
	i=0;

	while ( smb_get_next_share(&smb,&share[i]) == 0 )
	{
		printf("share = %s\n",share[i].name);
		sharecount++;
		i++;
	}
	smb_closepipe(&smb);

	sharecount=i;

	for (i=0;i<sharecount;i++)
	{
		sprintf(url[0],"//%s/%s","dolphin",share[i].name);

		printf("URL:%s\n",url[0]);

		err = smb_connect_url(&smb,url[0]);

		printf("ERROR: %d\n",err);

                while ( smb_get_next_file(&smb,&file) == 0 )
               	{
			printf("%s: %s %d \n",file.mode ? "DIR ":"FILE",file.filename,file.size);
		}

		smb_closepipe(&smb);
	}
*/
	smb_name2ip(&smb,"UBEROLDMAN",ip);
	// getipfromnmbname(

	printf("IP = %s\n",ip);		

	exit(0);	
	return 0;
}

