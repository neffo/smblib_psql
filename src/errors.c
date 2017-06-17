#include <stdio.h>
#include "smblib.h"
#include "errors.h"

void smb_error(smb_shiz *smb, int errnum)
{
	smb->errnum = errnum;
	smb_printerror(smb);
}

void smb_printerror(smb_shiz *smb)
{
	fprintf(stderr,"ERROR: #%d %s (%s)\n",smb->errnum,errors[smb->errnum].errs,errors[smb->errnum].errl);
}
