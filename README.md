# smblib_psql

#### Index and search public Windows shares on a LAN

This was an attempt to generate an index of all the shared files visible on a
LAN. I don't believe at the time there was a simple way to do this via linking
to SAMBA, or if there was I didn't see it. Instead I did it the hard way...

What this indexer did was crawl all the windows shares and throw it into a
PostgreSQL database. A very simple PHP webpage then enabled file searches and
even indirect downloads.

The crawler did this by parsing the output of the smbclient command. To make
this more challenging, this was all written in C. Later iterations even used
multithreading.

The basic functionality of the indexer was this:

1. Connect to master browser server and request list of workgroup and associated
  masters servers.
2. Connect to each workgroup master and request a list of servers in that
  workgroup.
3. Connect to each server and request a list of shares.
4. Connect to each share, and request a recursive list of directories and files.
  (This was actually handled by smbclient, but earlier versions of this indexer
  actually created a list of directories to browse individually. This was
  painfully slow as each time we had to reconnect to the server.)
5. Dump all these indexes into a big stupid database.

The website was just a simple search engine that would allow you to browse by
filetype and keyword. There was some functionality to allow the downloading of
files through the browser and possibly to the server (not sure if I completed
that).

The PHP code for the website is sadly (or not so sadly) lost. All that remains
is the original flat text file version.
