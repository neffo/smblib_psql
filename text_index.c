#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "smblib.h"

int get_wgs(smb_shiz *smb, smb_workgroup wgs[]);
int get_machs(smb_shiz *smb, smb_workgroup *wg, smb_machine machs[]);
int get_files(smb_shiz *smb, smb_machine *);
int get_shares(smb_shiz *smb, smb_machine *, smb_share *share);

int filec;
int dirc;

FILE *idx;

char *ffmt = "|%s|%s|%s|%s|%d|";

int main ( void )
{
	smb_workgroup wg[500];
	smb_machine mach[500];

	char bmaster[80];
	int i;

	int wgc,machc;

	smb_shiz smb;

	filec=0;
	dirc=0;
	machc=0;


	smb_setup(&smb);

//	smb.debug = 1;

	idx = fopen("/tmp/sambaindex.dat","w");

	if (idx == NULL)
	{
		printf("ERROR: unable to open index file for output\n");
		exit(1);
	}


	/*if ( var[1] )
		smb_setmaster(&smb,var[1]);
	else*/
	{
		smb_name2ip(&smb,0,bmaster);
		smb_setmaster(&smb,bmaster);
	}

	wgc = get_wgs(&smb, wg);

	for(i=0;i<wgc;i++)
	{
		machc+=get_machs(&smb, &wg[i], &mach[0]);
		// add wg to database
	}

	for (i=0;i<machc;i++)
	{
//		fprintf(stderr,"--> %d %s\n",i,mach[i].name);
		get_files(&smb, &mach[i]);
	}

	printf("Finished: Directories: %d  Files: %d\n",dirc,filec);
	
	exit(0);
}

int get_wgs(smb_shiz *smb, smb_workgroup *wgs)
{
	int wgc = 0;
	
	smb_init_workgroups(smb);

	smb_get_workgroups(smb);

	while (smb_get_next_workgroup(smb,&wgs[wgc]) == 0 )
	{
		wgc++;
	}
	smb_closepipe(smb);

	return wgc;
}

int get_machs(smb_shiz *smb, smb_workgroup *wg, smb_machine *machs)
{
	int machc=0;
	int smb_err=0;

	smb_init_machines(smb,wg->master);

	smb_err = smb_get_machines(smb);

	if (smb_err)
		goto stopit;

	while (smb_get_next_machine(smb, &machs[machc]) == 0 )
	{
		machc++;
	}

	stopit:

	smb_closepipe(smb);

	return machc;

	// FIXME: add machines to database
}

int get_files(smb_shiz *smb, smb_machine *mach)
{
	smb_share share[100];
	char cdir[1024];
	static char query[2048];
	smb_dent file;

	int i;
	int cd;
	int sharec=0;

	cd = 0;
	cdir[0]=0;

	smb_name2ip(smb,mach->name,smb->ip);

	sharec = get_shares(smb, mach, share);

	for(i=0;i<sharec;i++)
	{
		cdir[0]=0;
		smb_connect_recurse ( smb, mach->name, share[i].name, 0, 0);
		if (smb_get_files( smb ) == 0)
		while ( smb_get_next_file_r(smb, &file, cdir) == 0)
		{
			if (file.mode == 0)
			{
				filec++;
			//	printf("-> %s %s\n",cdir,file.filename);
				escape_badchars(file.filename, 1024);
				if (smb->dir_change == 1)
				{
				/*
					do_pg_command( pgc, res, "COMMIT", 1);
					PQclear(res);
					do_pg_command( pgc, res, "BEGIN", 1);
					PQclear(res);
				*/
					escape_badchars(cdir, 1024);
					printf("Entering %s...\n",cdir);
					smb->dir_change = 0;
					dirc++;
				}

//				if (strstr(cdir,"\\"))
//				printf("SDFASDFASDFASDFASDF %s\n",cdir);
				
				fprintf(idx,ffmt,mach->name,share[i].name,cdir,file.filename,file.size);
			}
		}
		else
			printf("smb_get_files() = 1\n");
		smb_closepipe(smb);
	}
	return 0;
}

int get_shares(smb_shiz *smb, smb_machine *mach, smb_share *share )
{
	int sharec=0;

	smb_init_shares(smb, mach);

	if (smb_get_shares(smb)) // NULL PIPE?
		return 0;

	while ( smb_get_next_share(smb,&share[sharec]) == 0 )
	{
		if (!strchr(share[sharec].name,'$'))
			sharec++;
	}

	smb_closepipe(smb);

	fprintf(stderr,"SHARE COUNT = %d\n",sharec);

	return sharec;

	// add shares to database
}
