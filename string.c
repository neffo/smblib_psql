#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smblib.h"
#include "errors.h"

int smb_gettoken_from_url(char *url, char *string, int token)
{
	char *fb=0;

	int i=0;
	int j=0;
	int h=0;

	string[0]=0;

	fb = strstr(url,"//");
	//fb = url;

	//printf("url = %s\n",url);

	if (!fb)
		return 1;

	while ( fb[i]=='/' )
		i++;

	for (h=0;h<token;h++)
	{
		while ( fb[i] && fb[i] != '/')
			i++;

		if (!fb[i])
			return 1;

		i++;
	}

	while ( fb[i] && fb[i] != '/' )
	{
		string[j] = fb[i];
//		printf("%d = %c\n",j,string[j]);
		i++;
		j++;
	}

	string[j] = 0;

	return 0;
}

int smb_gethost_from_url(char *url, char *host)
{
	return smb_gettoken_from_url(url,host,0);
}

int smb_getshare_from_url(char *url, char *share)
{
	return smb_gettoken_from_url(url,share,1);
}

int smb_getdir_from_url(char *url, char *dir)
{
	smb_gettoken_from_url(url,dir,2);

	if (*dir)
	{
		strcpy(dir,strstr(url,dir));
	}
	
	return 0;
}

/*
int smb_gethost_from_url(char *url, char *host)
{
	char *fb;

	int i=0;
	int j=0;

	fb = strstr(url,"//");

	if (!fb)
	{
		return 1;
	}

	while ( fb[i] == '/' )
	{
		i++;
	}
	
	while ( fb[i] != '/' && j < MAX_HOST_LEN )
	{
		host[j] = fb[i];
		i++;
		j++;
	}
	
	fb[j] = 0;

	return 0;
}
*/

int smb_line_to_smbdent(char *line, smb_dent *file)
{
	char *blah=0;
	char *filename;

	int namelen=0;
	int i=0;

	filename = file->filename;
//	"  msos.sys                  "
//	012345678901234567890123456789
//	blah=strstr(line+29,"    "); // not right
	blah=strstr(line+2,"   ");
	
	if (!blah)
		return 1;

	namelen=blah-line;

	for(i=2;i<namelen;i++)
	{
		filename[i-2]=line[i];
	}

	filename[i-2]=0;
	//'  ntdetect.com                     "
	// 12345678901234567890123456789012345
	i=namelen;
	if (i<35)
		i=35;

	while(line[i]==' ')
	{
		i++;
	}
	if (line[i]<59)
		goto size_bit;

	while(line[i]!=' ')
	{
		if (line[i]=='D')
			file->mode+=1;
		i++;
	}
	while(line[i]==' ')
	{
		i++;
	}
	size_bit:
	while (line[i]!=' ')
	{
		file->size*=10;
		file->size+=line[i]-48;
		i++;
	}

	return 0;
}

int smb_check_url( char *url)
{
	int slen=strlen(url);
	if (url[slen-1]=='/' && slen >3)
		url[slen-1]=0;
	return 0;
}

int smb_url_type ( char *url)
{
	int i;
	int slen=strlen(url);
	int count=0;

	for (i=0;i<slen;i++)
	{
		if ( url[i]=='/' )
			count++;
	}
	
	if (count==2 && slen==2)
		return URL_LISTWORKGROUPS;
	else if (count==2 && slen > 2)
		return URL_LISTMACHINES;
	else if (count>3)
		return URL_LISTDIR;
	
	return 0;

}

int getword(char *line, char *word)
{
	int i;

	for (i=0;i<strlen(line) && i<20;i++)
	{
		if (line[i]!=' ')
		{
			word[i]=line[i];
		}
		else
		{
			word[i]=0;
			return 0;
		}
	}
	return 1;
}

int escape_badchars(char *string, int size)
{
	char *bad;
	char string2[size];
	int i,j;
	int slen;
	bad=string-2;

//	strncpy(string2,string,size);

	slen = strlen(string);
	memcpy(string2,string,slen);

	for(i=0;i<slen;i++)
     	{
		if (string2[i]=='\\')
			string2[i]='/';
	}
	
	j=0;
	for (i=0;i<slen;i++)
	{
		if (string2[i] == '\'')
		{
			string[j] = '\\';
			j++;
		}
		string[j]=string2[i];
		j++;
	}
	
	return 0;
}

int get_dir_from_line(char *line, char *dir)
{
	int slen;
	slen=strlen(line);
	//strcpy(dir,line+1);
	memcpy(dir,line+1,slen);
	return 0;
}
	
