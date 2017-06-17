#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smblib.h"
#include "errors.h"

/*
int smb_dirlist_init(smbshiz *smb)
{
	char line[1024];
	while (fgets(line,1023,smb->ppipe) && (!strstr(line,"  ..")))
	{
	}
}*/

int smb_get_next_file(smb_shiz *smb, smb_dent *file)
{
	static char line[1024];

	file->filename[0]=0;
	file->size=0;
	file->mode=0;

	getline:
	file->mode=0;

//	if (smb->debug)
//		fprintf(stderr,"&LINE = %d\n",line);
	
	fgets(line,1023,smb->ppipe);

	if (smb->debug)
		fprintf(stderr,"LINE: %s",line);

	if (!line[0]) 
		return 1;
	
	if (feof(smb->ppipe))
		return 1;

	if (strstr(line,"  .."))
		goto getline;
	if (strstr(line," ."))
		goto getline;
	if (strstr(line,"ERRDOS"))
		{smb_error(smb,SMB_NO_USER_ACCESS);return 1;}
	if (strstr(line,"ERRSRV"))
		{smb_error(smb,SMB_SRV_NO_ACCESS);return 1;}
	if (strlen(line)<3)
		goto getline;

	line[strlen(line)-1]=0; // strip \n
	smb_line_to_smbdent(line,file);
	if (strcmp(file->filename,".")==0)
		goto getline;
	return 0;
}

int smb_get_next_file_r(smb_shiz *smb, smb_dent *file, char *dir)
{
	static char line[1024];

	file->filename[0] = 0;
	file->size = 0;
	file->mode = 0;

	getline_r:

	file->mode = 0;

	fgets(line,1023,smb->ppipe);

	if (smb->debug)
		fprintf(stderr,"smb_get_next_file_r() : LINE: |%s|\n",line);
//	more <filename>
//	123456789012345
	if ( strncmp(line,"more <filename>",15)==0 )
		return 1;

	if (!*line)
		return 1;

//	if (feof(smb->ppipe))
//		return 1;
	
	if (strncmp(line,"  .",3)==0)
		goto getline_r;

	line[strlen(line)-1] = 0; // strip linefeed

	/*
	if (strstr(line,"Baldur"))
		 fprintf(stderr,"---> -|%s|-\n",line);
	if (strstr(line,"Diablo"))
		fprintf(stderr,"---> -|%s|-\n",line);
	*/
	
	if ( line[0] == '\\' )
	{
		get_dir_from_line(line,dir);
		if (smb->debug)
			fprintf(stderr,"DEBUG: cd -> %s\n",dir);
		smb->dir_change = 1;
		goto getline_r;
		
	}

	if (smb_line_to_smbdent(line,file))
		goto getline_r;

	if ( !*file->filename )
		goto getline_r;

	if (smb->debug)
		fprintf(stderr,"DEBUG: filename = -|%s|-\n",file->filename);

	return 0;
}
