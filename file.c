#include <sys/stat.h>
#include <stdio.h>
/*
int filetype(const char *filename)
{
	if (chkext(filename,".mpg") || chkext(filename,".mpeg") || chkext(filename,".MPEG") || chkext(filename,".MPG")) 
	{
		return FT_MPEG;
	}
	else if (chkext(filename,".txt") || chkext(filename,".TXT") || chkext(filename,".NFO") || chkext(filename,".DIZ"))
	{
		return FT_TEXT;
	}
	else if (chkext(filename,".html") || chkext(filename,".htm") || chkext(filename,"HTML") || chkext(filename,"HTM"))
	{
		return FT_HTML;
	}
	else if (chkext(filename,".mp3") || chkext(filename,".MP3"))
	{
		return FT_MP3;
	}
	else if (chkext(filename,".jpg") || chkext(filename,".jpeg") || chkext(filename,"JPEG") || chkext(filename,"JPG"))
	{
		return FT_JPEG;
	}
	else if (chkext(filename,".gif") || chkext(filename,".GIF"))
	{
		return FT_GIF;
	}
	else if (chkext(filename,".zip") || chkext(filename,".ZIP"))
	{
		return FT_ZIP;
	}
	else if (chkext(filename,".wav") || chkext(filename,".WAV"))
	{
		return FT_WAV;
	}
	else if (chkext(filename,".exe") || chkext(filename,".EXE"))
	{
		return FT_EXE;
	}
	else if (chkext(filename,".c") || chkext(filename,".C")
			|| chkext(filename,"C++") || chkext(filename,"c++")
			|| chkext(filename,"cc") || chkext(filename,"CC"))
	{
		return FT_SOURCE;
	}
	else if (chkext(filename,".gz") || chkext(filename,".GZ")
			|| chkext(filename,".tgz") || chkext(filename,".TGZ"))
	{
		return FT_TAR;
	}
	else if (chkext(filename,".avi") || chkext(filename,".AVI"))
	{
		return FT_AVI;
	}
	else if (chkext(filename,".png") || chkext(filename,".PNG"))
	{
		return FT_PNG;
	}
	else
	{
		return FT_OTHER;
	}
}

char *mimetype(const char *filename)
{
	switch (filetype(filename))
	{
		case FT_MPEG:
			return("video/mpeg");
			break;
		case FT_TEXT:
		case FT_SOURCE:
			return("text/plain");
			break;
		case FT_HTML:
			return("text/html");
			break;
		case FT_MP3:
			return("audio/mpeg");
			break;
		case FT_JPEG:
			return("image/jpeg");
			break;
		case FT_GIF:
			return("image/gif");
			break;
		case FT_ZIP:
			return("application/zip");
			break;
		case FT_WAV:
			return("audio/x-wav");
			break;
		case FT_AVI:
			return("video/x-msvideo");
			break;
		case FT_PNG:
			return("image/png");
			break;
		default:
			return("application/octet-stream");
	}
}
*/
void smb_ffs(char *nice, size_t size)
{
	if (size<1000)
	{
		sprintf(nice,"%db",size);
	}
	else if (size < 1000000)
	{
		sprintf(nice,"%dKb",size/1024);
	}
	else if (size < 1000*1024*1024)
	{
		sprintf(nice,"%dMb",(size/1024)/1024);
	}
	else
	{
		sprintf(nice,"%dGb",(size/1024)/1024);
	}
}
