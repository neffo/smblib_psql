#define SMB_NULL_PIPE 1
#define SMB_PIPE_OPEN 2
#define SMB_PIPE_ERROR 3
#define SMB_BAD_HOST 4 //Connection to <some host> failed
#define SMB_BAD_SHARE 5 //ERRDOS - ERRnosuchshare
#define SMB_BAD_PATH 6 //ERRDOS - ERRbadpath
#define SMB_NO_USER_ACCESS 7 //ERRDOS - ERRnoaccess
#define SMB_SRV_NO_ACCESS 8 //ERRSRV - ERRaccess
#define SMB_BAD_PASSWORD 9 //ERRSRV - ERRbadpw
#define SMB_POPEN_FAILURE 10 // unable to run smbclient command?

typedef struct errcodes_s {
	int errn;
	char *errs;
	char *errl;
}errcodes_t;

static errcodes_t errors[12] = {
/*	0,"Unknown error","Unknown",*/
	SMB_NULL_PIPE, "Null pointer","The program tried to perform an operation on a NULL process pipe.",
	SMB_PIPE_OPEN,"Already open pipe","The process pipe has already been initialised, a procedure expected it to be NULL.",
	SMB_PIPE_ERROR,"Unknown pipe error","Unknown pipe error",
	SMB_BAD_HOST,"Bad host","The specified host was not found, or didn't respond.",
	SMB_BAD_SHARE,"Bad share","The specified sharename was not valid.",
	SMB_BAD_PATH,"Bad directory","The directory specified doesn't exist or is invalid.",
	SMB_NO_USER_ACCESS,"No user access","The server denied access because of an invalid user/password combo.",
	SMB_SRV_NO_ACCESS,"Access denied","The server refused access.",
	SMB_BAD_PASSWORD,"Bad password","The server refused access because of an invalid user/password combo.",
	SMB_POPEN_FAILURE,"popen failed","The process pipe was not opened succesfully, possibly due to a problem locating or running \"smbclient\".",
	NULL,NULL,NULL
};
