#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "smblib.h"

#define CURDIR ".smbcd"

char *password=0;

void listworkgroups()
{
	printf("%c[01;36m%s%c[00m:\n",27,"//",27);
	workgroup *workgroups=0;
	workgroup *cur=0;

	workgroups=get_workgroups(workgroups,MASTER);

	cur=workgroups;

	while (cur)
	{
		//printf("//%s/\n",cur->name);
		printf("  %c[01;34m(DIR)%c[00m //%s\n",27,27,cur->name);
		cur=cur->next;
	}

	delete_workgroup_list(workgroups);
}

void listmachines(char *wg)
{
	if (wg)
		printf("%c[01;36m//%s/%c[00m:\n",27,wg,27);
	else
		printf("%c[01;36m//%s/%c[00m:\n",27,"*",27);
	workgroup *workgroups=0;
	workgroup *curw=0;

	machine *machines=0;
	machine *curm=0;

	workgroups=get_workgroups(workgroups,MASTER);

	curw=workgroups;

	while (curw)
	{
		if (wg==0 || strcmp(curw->name,wg)==0)
		{
			machines=get_machines(curw,machines);
		}
		curw=curw->next;
	}

	if (machines)
	{
		curm=machines;
	
		while (curm)
		{
			printf("  %c[01;34m(DIR)%c[00m //%s\n",27,27,curm->host);
			//printf("//%s/\n",curm->host);
			curm=curm->next;
		}
	}

	delete_workgroup_list(workgroups);
	delete_machine_list(machines);
}

void listmachines(void)
{
	listmachines(0);
}

void listdir_alt(const char *service, const char *match)
{
	//printf("ALT listdir()\n");
	printf("%c[01;36m%s%c[00m:\n",27,service,27);
	sdiritem *list=0;
	sdiritem *cur=0;
	sdiritem *prev=0;

	char *fsize=0;
	
	list=get_dir((char *)service,0);

	cur=list;

	while (cur)
	{
		if (cur->mode&S_IS_DIR)
		{
			if (chkmatch(cur->filename,match)==1)
			{
				printf("   %c[01;34m(DIR)%c[00m %s\n",27,27,cur->filename);
			}
		}
		else
		{
			if (chkmatch(cur->filename,match)==1)
			{
				fsize=ffs(cur->size);
				printf("%c[01;32m%8s%c[00m %s\n",27,fsize,27,cur->filename);
				delete fsize;
			}
		}
		cur=cur->next;
	}
	delete_sdirlist(list);
}

void listdir(const char *service, const char *match) {

	printf("%c[01;36m%s%c[00m:\n",27,service,27);

	SMBIO io(getFunc);
	io.setDefaultBrowseServer(MASTER); // it should always be run on my machine anyway
	//io.setDefaultUser("nobody");
	
	//char *tmp;
	char *size;
	//long int total=0;
	
	
	// code to grab password
	if (password)
		delete password;
	password=getpassword(service);
	
	// process command-line argument as a URL
	struct stat statbuf;

	if (io.stat(service,&statbuf)==-1)
		{fprintf(stderr,"ERROR: unable to open file/dir %s\n",service);return;}

	int dd=io.opendir(service);
	if (dd==-1) 
		{fprintf(stderr,"ERROR: unable to open file/dir %s\n",service);return;} // just give the error and move on
	SMBdirent *dent;
	while ((dent=io.readdir(dd))) {
		/*while ( ( (dent->d_name[strlen(dent->d_name)-1]=='$') ) ) // this ignores the printer$ and ipc$ services
		{
			dent=io.readdir(dd);
		}*/
		if ((dent->st_mode&040000) /*&& (dent->d_name[0]!='.')*/) // it does some wierd shit like continually recursing 
		{                                                     // into the same directory because of the "." being considered as a directory (now fixed)
			/*tmp=new char[1024];
			if (service[strlen(service)-1]!='/')
			{
				sprintf(tmp,"%s/%s",service,dent->d_name);
			} 
			else
			{
				sprintf(tmp,"%s%s",service,dent->d_name);
			}					
			listdir(tmp, level+1); // list the directory and then add the size of it to the total
			free(tmp);*/
			if (chkmatch(dent->d_name,match)==1)
				printf("   %c[01;34m(DIR)%c[00m %s\n",27,27,dent->d_name);
		}
		else if (!(dent->st_mode&040000))
		{
			if (chkmatch(dent->d_name,match)==1)
			{
				size=ffs(dent->st_size);
				printf("%c[01;32m%8s%c[00m %s\n",27,size,27,dent->d_name);
				delete size;
			}
		}
	}
	io.closedir(dd);
	//free(dent);
}

int main (int varc, char **var)
{
	char *cd;
	int i=0;
	int optcount=0;

	cd=getcd();
	//printf("%c[01;36m%s%c[00m:\n",27,cd,27);

	for(i=0;i<varc;i++)
	{
		if (var[i][0]=='-')
			optcount++;
		if (strcmp(var[i],"-a")==0)
			options += BIT_ALT_LIST;
		if (strcmp(var[i],"-m")==0)
			options += BIT_MACH_LIST;
		if (strcmp(var[i],"-w")==0)
			options += BIT_WG_LIST;
	}
	
	if ((varc-optcount)>1 && varc==2)
	{
		listdir(cd,var[varc-1]);
	}
	else
	{
		if (!options)
		{	if (options&BIT_ALT_LIST)
				listdir_alt(cd,"*");
			else
				listdir(cd,"*");
		}
		else
		{
			if (options&BIT_WG_LIST)
				listworkgroups();
			else if (options&BIT_MACH_LIST)
				if (varc-optcount==1)
					listmachines();
				else
					listmachines(var[varc-1]);
			else if (options&BIT_ALT_LIST)
				listdir_alt(cd,"*");
		}
		
	}

	delete cd;
}
