#include <stdio.h>
#include "smblib.h"
#include "errors.h"

void smb_error(smb_shiz *smb, int errnum)
{
	smb->errnum = errnum;
}

void smb_printerror(smb_shiz *smb)
{
}
