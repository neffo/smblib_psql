#include <stdio.h>
#include <string.h>
#include "smblib.h"

int get_files_test(smb_shiz *smb/*, smb_machine *mach*/);

int main ( void )
{
	int i;
	smb_shiz smb;
	smb_setup(&smb);

	smb.debug = 0;

	for (i=0;i<50;i++)
		get_files_test(&smb);
	
	return 0;
}
			
int get_files_test(smb_shiz *smb /*, smb_machine *mach*/)
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

//	smb_name2ip(smb,mach->name,smb->ip);

//	sharec = get_shares(smb, pgc, mach, share);

//	snprintf(query,2047,"DELETE FROM file WHERE host_name = '%s'",mach->name);
//	do_pg_command( pgc, res, query, smb->debug);
//	PQclear(res);

//	do_pg_command( pgc, res, "BEGIN", smb->debug);
//	PQclear(res);

//	for(i=0;i<sharec;i++)
	{
//		smb_connect_recurse ( smb, mach->name, share[i].name, 0, 0);
		smb->ppipe = fopen("dirlist.txt","r");
		if (smb_get_files( smb ) == 0)
		while ( smb_get_next_file_r(smb, &file, cdir) == 0)
		{
			if (file.mode == 0)
			{
//				filec++;
//				printf("-> %s %s\n",cdir,file.filename);
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
					smb->dir_change = 0;
//					dirc++;
				}

//				if (strstr(cdir,"\\"))
//				printf("SDFASDFASDFASDFASDF %s\n",cdir);
				
//				snprintf(query,2047,file_insert,mach->name,share[i].name,cdir,file.filename,file.size);
				//printf("query -> %s\n",query);
//				do_pg_command( pgc, res, query, smb->debug);
//				PQclear(res);
			}
		}
		else
			printf("smb_get_files() = 1\n");
		//smb_closepipe(smb);
		fflush(smb->ppipe);
		fclose(smb->ppipe);
	}
//	do_pg_command( pgc, res, "COMMIT", 1);
//	PQclear(res);
	return 0;
}
