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

char *ffmt = "|%s|%s|%s|%s|%d|\n";

FILE *idx;

int main ( int varc, char **var )
{
	smb_workgroup wg[500];
	smb_machine mach[500];

	char bmaster[80];
	char filename[120];
	int i;

	int wgc,machc;

	smb_shiz smb;

	filec=0;
	dirc=0;
	machc=0;


	smb_setup(&smb);

//	smb.debug = 1;

/*
	if ( var[1] )
		smb_setmaster(&smb,var[1]);
	else
	{
		smb_name2ip(&smb,0,bmaster);
		smb_setmaster(&smb,bmaster);
	}
*/

	smb_setmaster(&smb,"UBEROLDMAN");

	wgc = get_wgs(&smb, wg);

	fprintf(stderr,"%d WORKGROUPS FOUND:\n--------\n",wgc);

	for (i=0;i<wgc;i++)
	{
		fprintf(stderr,"%d WG: %s MASTER: %s\n",i+1,wg[i].name,wg[i].master);
	}

//	sleep(5);

	for(i=0;i<wgc;i++)
	{
		machc+=get_machs(&smb, &wg[i], &mach[machc]);
		// add wg to database
	}

	fprintf(stderr,"%d MACHINES FOUND:\n---------\n",machc);
	
//	sleep(5);

	for(i=0;i<machc;i++)
	{
		fprintf(stderr,"%d MACH: %s\n",i+1,mach[i].name);
	}

	for (i=0;i<machc;i++)
	{
		/*strcpy(filename,"/var/lib/sambaidx/");
		strcat(filename,mach[i].name);
		idx = fopen(filename,"w+");
		if (idx == NULL)
			idx = stdout;
		*/
		get_files(&smb, &mach[i]);
		/*if (idx != stdout)
			fclose(idx);*/
	}

	//printf("Finished: Directories: %d  Files: %d\n",dirc,filec);
	
	exit(0);
	return 0;
}

int get_wgs(smb_shiz *smb, smb_workgroup *wgs)
{
	int wgc = 0;
	int ret = 0;
	
	ret = smb_init_workgroups(smb);

	if (ret) fprintf(stderr,"smb_init_wgs() failed. error : %d\n",ret);
	
	ret = smb_get_workgroups(smb);

	if (ret) fprintf(stderr,"smb_get_wgs() failed. error : %d\n",ret);

	while (smb_get_next_workgroup(smb,&wgs[wgc]) == 0 )
	{
//		printf("WG : %s M: %s\n",wgs[wgc].name,wgs[wgc].master);
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
//		printf("MACHINE: %s\n",machs[machc].name);
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
	char odir[1024];
	static char query[2048];
	smb_dent file;

	int i;
	int cd;
	int sharec=0;

	cd = 0;
	cdir[0]=0;

	smb_name2ip(smb,mach->name,smb->ip);

	sharec = get_shares(smb, mach, share);

	fprintf(stderr,"%d shares on %s\n",sharec, mach->name);

	if (sharec == 0)
		return 0;

	for(i=0;i<sharec;i++)
	{
		cdir[0]=0;
		odir[0]=0;
		smb_connect_recurse ( smb, mach->name, share[i].name, 0, 0);
		if (smb_get_files( smb ) == 0)
		while ( smb_get_next_file_r(smb, &file, cdir) == 0)
		{
			if (smb->dir_change == 1)
			{
				strcpy(odir,cdir);
				//escape_badchars(cdir, 1024);
				printf("Entering %s -> %s...\n",mach->name,cdir);
				smb->dir_change = 0;
				dirc++;
				//filec--;
			}
			//else
			if (file.mode == 0)
			{
				filec++;
				/*if (smb->dir_change == 1) //FIXME: this should be outside this if then ( if (file.mode == 0 )
				{
					strcpy(odir,cdir);
					escape_badchars(cdir, 1024);
					printf("Entering %s -> %s...\n",mach->name,cdir);
					smb->dir_change = 0;
					dirc++;
				}
				else*/
				fprintf(idx,ffmt,mach->name,share[i].name,odir,file.filename,file.size);
			}
		}
		else
			printf("smb_get_files( %s -> %s ) failed = 1\n",mach->name,share[i].name);
		smb_closepipe(smb);
	}
	return 0;
}

int get_shares(smb_shiz *smb, smb_machine *mach, smb_share *share )
{
	int sharec=0;

	if (smb_init_shares(smb, mach))
		return 0;

	if (smb_get_shares(smb)) // NULL PIPE?
		return 0;

	while ( smb_get_next_share(smb,&share[sharec]) == 0 )
	{
		if (!strchr(share[sharec].name,'$'))
			sharec++;
	}

	smb_closepipe(smb);

	fprintf(stderr,"%d shares on %s\n",sharec,mach->name);

	return sharec;

	// add shares to database
}
