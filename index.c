#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "smblib.h"
#include "list.h"

#define MAX_DIRS 2048

/* THIS PROGRAM REALLY NEEDS A COMPLETE REWRITE IT WAS ONLY
 * WRITTEN THIS WAY BECAUSE I WANTED TO TEST SOMETHING
 * QUICKLY, IT OBVIOUSLY WORKED :)
 */

int do_pg_command( PGconn *, PGresult *, char *, int);

int main ( int varc, char **var )
{
	smb_workgroup wg[500];
	smb_machine mach[500];
	smb_share share[100];
	smb_dent file;
	char dir[MAX_DIRS][1024];
	char bmaster[80];
	char query[2048];
	char command[2048];

	int wgc,machc,sharec,dirc;
	int wgi,machi,sharei,diri;	
	int smb_err;
	int cfiles;
	int cdirs;
	
	PGconn *pgc;
	PGresult *res;

	smb_shiz smb;

	wgc=0;
	machc=0;
	sharec=0;
	dirc=0;
	cfiles=0;
	cdirs=0;

	smb_setup(&smb);

	pgc = PQconnectdb("dbname = 'ineffable'");

	if (pgc == 0)
	{
		if (smb.debug) fprintf(stderr,"ERROR: psql startup failed\n");
		PQfinish(pgc);
		exit(1);
	}

	if ( var[1] && var[1][0]!='-' )
		smb_setmaster(&smb,var[1]);
	else
	{
		smb_name2ip(&smb,0,bmaster);
		smb_setmaster(&smb,bmaster);
	}

//	smb_setpassword(&smb,"guest");

//	smb.debug = 1;

	smb_init_workgroups(&smb);

	smb_get_workgroups(&smb);

	wgc=0;

	while (smb_get_next_workgroup(&smb,&wg[wgc]) == 0 )
	{
//		printf("NAME: %d %s %s\n",wgc,wg[wgc].master,wg[wgc].name);
		wgc++;
	}
	smb_closepipe(&smb);

	for (wgi=0;wgi<wgc;wgi++)
	{
		machc=0;

//		printf("WG: %d %s\n",wgi,wg[wgi].master);

		smb_err = smb_init_machines(&smb,wg[wgi].master);
		if (smb_err)
			goto stopit;

		smb_err = smb_get_machines(&smb);

		if (smb_err)
//			printf("stupid gay error\n");
			goto stopit;

		while (smb_get_next_machine(&smb,&mach[machc]) == 0 )
		{
//			printf("MACHINE: %d %s\n",machc,mach[machc].name);
			machc++;
		}

		stopit:

		smb_closepipe(&smb);

		for (machi=0;machi<machc;machi++)
		{
//			putc('M',stdout);
//			fflush(stdout);

			// check if host exists in database

			snprintf(query,2047,"SELECT * FROM hosts WHERE host_name = '%s'",mach[machi].name);

			if (smb.debug) fprintf(stderr,"DEBUG : query = %s\n",query);

			res = PQexec(pgc,query);

			switch (PQresultStatus(res))
			{
				case PGRES_TUPLES_OK:
					if (smb.debug) fprintf(stderr,"DEBUG : PGRES_TUPLES_OK\n");
					break;
				case PGRES_COMMAND_OK:
					if (smb.debug) fprintf(stderr,"DEBUG : PGRES_COMMAND_OK\n");
					break;
				default:
					if (smb.debug) fprintf(stderr,"DEBUG: some error occured\n");
			}

			// if it does then -> update access time
			// else
			// -> add it

			if (PQntuples(res))
			{
				// update it
				PQclear(res);
				if (smb.debug) fprintf(stderr,"DEBUG : already in DB\n");
				// update code can go here
			}
			else
			{
				// add it
				PQclear(res);

				if (smb.debug) fprintf(stderr,"DEBUG : not in DB\n");

				snprintf(query,2047,"INSERT INTO hosts (host_name) VALUES ('%s')",mach[machi].name);

				if (smb.debug) fprintf(stderr,"DEBUG : query = %s\n",query);

				res = PQexec(pgc,query);

				switch (PQresultStatus(res))
				{
					case PGRES_TUPLES_OK:
						if (smb.debug) fprintf(stderr,"DEBUG : PGRES_TUPLES_OK\n");
						break;

					case PGRES_COMMAND_OK:
						if (smb.debug) fprintf(stderr,"DEBUG : PGRES_COMMAND_OK\n");
						break;

					default:
						if (smb.debug) fprintf(stderr,"DEBUG: some error occured\n");

				}

				PQclear(res);

			}

			//

			snprintf(query,2047,"DELETE FROM file WHERE host_name = '%s'",mach[machi].name);

			if (smb.debug) fprintf(stderr,"DEBUG : query = %s\n",query);

			res = PQexec(pgc,query);

			switch (PQresultStatus(res))
			{
				case PGRES_TUPLES_OK:
					if (smb.debug) fprintf(stderr,"DEBUG : PGRES_TUPLES_OK\n");
					break;

				case PGRES_COMMAND_OK:
					if (smb.debug) fprintf(stderr,"DEBUG : PGRES_COMMAND_OK\n");
					break;

				default:
					if (smb.debug) fprintf(stderr,"DEBUG: some error occured\n");
			}

			PQclear(res);

//
			smb_name2ip(&smb,mach[machi].name,smb.ip);

			smb_init_shares(&smb,&mach[machi]);

			smb_get_shares(&smb);

			sharec=0;

			while (smb_get_next_share(&smb,&share[sharec]) == 0)
			{
//				printf("SHARE %s\n",share[sharec].name);
				//if (*share[sharec].name)
					sharec++;
			}

			smb_closepipe(&smb);

			for (sharei=0;sharei<sharec;sharei++)
			{
				if (!strchr(share[sharei].name,'$'))
				{
//					putc('S',stdout);
//					fflush(stdout);
					snprintf(query,2047,"SELECT * FROM shares WHERE host_name = '%s' AND share_name = '%s'",mach[machi].name,share[sharei].name);

					if (smb.debug) fprintf(stderr,"DEBUG : query = %s\n",query);

					res = PQexec(pgc,query);

					switch (PQresultStatus(res))
					{
						case PGRES_TUPLES_OK:
							if (smb.debug) fprintf(stderr,"DEBUG : PGRES_TUPLES_OK\n");
							break;
						case PGRES_COMMAND_OK:
							if (smb.debug) fprintf(stderr,"DEBUG : PGRES_COMMAND_OK\n");
							break;
						default:
							if (smb.debug) fprintf(stderr,"DEBUG: some error occured\n");
					}

					if (PQntuples(res))
					{
						// already there
						PQclear(res);
					}
					else
					{
						PQclear(res);

						snprintf(query,2047,"INSERT INTO shares (host_name, share_name) VALUES ('%s','%s')",mach[machi].name,share[sharei].name);
						if (smb.debug) fprintf(stderr,"DEBUG : query = %s\n",query);
						res = PQexec(pgc,query);
						switch (PQresultStatus(res))
						{
							case PGRES_TUPLES_OK:
								if (smb.debug) fprintf(stderr,"DEBUG : PGRES_TUPLES_OK\n");
								break;
							case PGRES_COMMAND_OK:
								if (smb.debug) fprintf(stderr,"DEBUG : PGRES_COMMAND_OK\n");
								break;
							default:
								if (smb.debug) fprintf(stderr,"DEBUG: some error occured\n");
						}

						PQclear(res);

					}

					dirc=0;
					//(dir[dirc],"");
					dir[0][0]=0;
					dirc++;

					for (diri=0;diri<dirc;diri++)
					{
//						printf("enterubg dir : %d %s\n",diri,dir[diri]);
//						putc('D',stdout);
//						fflush(stdout);

/*						if (*dir[diri])
							snprintf(command,2047,"//%s/%s/%s",mach[machi].name,share[sharei].name,dir[diri]);
						else
							snprintf(command,2047,"//%s/%s",mach[machi].name,share[sharei].name);
*/
//						if (smb.debug) fprintf(stderr,"DEBUG : //%s/%s\n",mach[machi].name,share[sharei].name);

						if (smb.debug) fprintf(stderr,"--> DIRC %d FILEC %d \n",cdirs,cfiles);
						smb_connect(&smb,mach[machi].name,share[sharei].name,dir[diri],"guest",0);

						do_pg_command(pgc,res,"BEGIN",0);
						PQclear(res);
						
						while ( smb_get_next_file(&smb,&file) == 0 )
				                {
							if (*file.filename /* && !strchr(file.filename,'\'')*/)
							{
								escape_badchars(file.filename,1024);
								// FIXME: need to do a proper fix for ' characters in filenames (replace with \')
								// 	can't be fucked right now
								if (!file.mode)
								{
									//printf("%s/%s\n",dir[diri],file.filename);
									// start
									snprintf(query,2047,"INSERT INTO file (host_name, share_name, dir_name, file_name, size) VALUES ('%s','%s','%s','%s',%d)",mach[machi].name,share[sharei].name,dir[diri],file.filename,file.size);

									if (smb.debug) fprintf(stderr,"DEBUG : query = %s\n",query);
									res = PQexec(pgc,query);
									switch (PQresultStatus(res))
									{
										case PGRES_TUPLES_OK:
											if (smb.debug) fprintf(stderr,"DEBUG : PGRES_TUPLES_OK\n");
											break;
										case PGRES_COMMAND_OK:
											if (smb.debug) fprintf(stderr,"DEBUG : PGRES_COMMAND_OK\n");
											break;
										default:
											if (smb.debug) fprintf(stderr,"DEBUG: some error occured\n");
									}
									PQclear(res);
									
									// end 
									cfiles++;
								}
								else if (dirc < MAX_DIRS)
								{
									snprintf(dir[dirc],1023,"%s/%s",dir[diri],file.filename);
									dirc++;
									cdirs++;
								}
							}
						
//		                        printf("%s: %s %d \n",file.mode ? "DIR ":"FILE",file.filename,file.size);
				                }
						do_pg_command(pgc,res,"COMMIT",0);
						PQclear(res);
						smb_closepipe(&smb);
					}
				}
	
			}
			
		}

	}
	printf("dirs: %d files: %d\n",cdirs,cfiles);
	PQfinish(pgc);
	exit(0);
}

int do_pg_command( PGconn *pgc, PGresult *res, char *query, int debug)
{
	int errorc=-1;
	res = PQexec(pgc,query);
	switch (PQresultStatus(res))
	{
        	case PGRES_TUPLES_OK:
	                if (debug) fprintf(stderr,"DEBUG : PGRES_TUPLES_OK\n");
			errorc=0;
			errorc=PQntuples(res);
        	        break;
		case PGRES_COMMAND_OK:
			if (debug) fprintf(stderr,"DEBUG : PGRES_COMMAND_OK\n");
			errorc=0;
			break;
		default:
			if (debug) fprintf(stderr,"DEBUG: some error occured\n");
	}

	return errorc;
}
