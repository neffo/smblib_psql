<?

function ffs ($size) {
	$ffsize="";

	if ($size<1024)
	{
		$ffsize = sprintf("%d b",$size);
//		$ffsize=$size."b";
	}
	else if ($size < (1024*1024))
	{
		$ffsize = sprintf("%3.2f Kb",($size/1024));
//		$ffsize=($size/1024)."Kb";
	}
	else if ($size < (1024*1024*1024))
	{
		$ffsize = sprintf("%3.2f Mb",($size/(1024*1024)));
//		$ffsize=($size/(1024*1024))."Mb";
	}
	else
	{
		$ffsize = sprintf("%1.2f Gb",($size/(1024*1024*1024)));
//		$ffsize=($size/(1024*1024*1024))."Gb";
	}

	return($ffsize);
}

function dlbutton ($h,$s,$d,$f) {

	?>
	<form action="leach.php" method=POST>
	<input type=hidden name=host value="<? echo $h; ?>">
	<input type=hidden name=share value="<? echo $s; ?>">
	<input type=hidden name=dir value="<? echo $d; ?>">
	<input type=hidden name=file value="<? echo $f; ?>">
	<input type=submit value="D">
	</form>
	<?
}

$q=$query;

if(!isset($PHP_AUTH_USER)) {
	Header("WWW-Authenticate: Basic realm=\"Uberoldman\"");
	Header("HTTP/1.0 401 Unauthorized");
	echo "Text to send if user hits Cancel button\n";
	exit;
} else {
//	echo "Hello $PHP_AUTH_USER.<P>";
//	echo "You entered $PHP_AUTH_PW as your password.<P>";

	if ($PHP_AUTH_PW != "hello123")
	{
		Header("HTTP/1.0 401 Unauthorized");
		echo "Invalid password\n";
		exit;
	}
}

include "form.inc";

echo "<hr>\n";

if ($e != "")
{
	$e = ".".$e;
}

if (strlen($q)>2 || $e != "")
{

//	$indexfile=file("/var/lib/sambaindex.dat");
	$maxresults=9999;
	$line=0;
	$rescount=0;
	$bytes=0;

	$sz = $sz * 1000 * 1000;

//	echo "Q = $q $sign E = $e Sz = $sz Sc = $sc<br>\n";

	

	echo "<table>\n";

	if ($sdir = @opendir("/var/lib/sambaidx")) 
	
	while ($file = readdir($sdir))
	{
		if ( $file != "." && $file != "..")
		{
			$indexfile=file("/var/lib/sambaidx/".$file);
			$line = 0;

			while ($rescount < $maxresults && $line < count($indexfile) && $indexfile[$line][0]=="|")
			{
				if ( $q != "" )
				{
					if (stristr($indexfile[$line],$q))
						$qm = 1;
					else
						$qm = 0;

					if (strcmp($sign,"false")==0)
					if ($qm == 1)
						$qm = 0;
					else 
						$qm = 1;
				}
				else // $q == ""
					$qm = 1;
		
				if ( $e != "" ) 
				{
					if (!strncasecmp(strrchr($indexfile[$line],"."),$e,strlen($e)))
						$em = 1;
					else
						$em = 0;
				}
				else // $e = ""
					$em = 1;

				if ( $qm && $em)
				{
					$filedata = explode("|", chop($indexfile[$line]));
					$dir = $filedata[3];
					if ( $e != "" && !strstr($filedata[4],$e))
					{
						$line++;
						continue;
					}

					// don't list bad hosts
					if ($hosts != "" && strstr($hosts,$filedata[1]))
					{
						$line++;
						continue;
					}

					$sm = 1;

					if ( $sz != "" )
					{
						if ($sc == "gt" && ( $filedata[5] < ($sz) ))
						{
							$sm = 0;
						}
						else if ($sc == "lt" && ( $filedata[5] > ($sz) ))
						{
							$sm = 0;
						}
						else if ($sc == "ap" && ($filedata[5] > 0) && ( (abs($filedata[5] - ($sz)))/$filedata[5]) > .25)
						{
							$sm = 0;
						}
					}

					if ($sm == 0 )
					{
						//echo "No match.<br>";
						$line++;
						continue;
					}
					
					if (strlen($dir)>0)
						$dir=$dir."/";
					$link = sprintf("///%s/%s/%s%s",$filedata[1],$filedata[2],$dir,$filedata[4]);
					$link = str_replace("\\","/",$link);
					if ($rescount%2)
						$color="#ffffff";
					else
						$color="#eeeeee";
					printf("<tr bgcolor=\"$color\"><td width=75><b>%s</b></td>",ffs($filedata[5]));
					echo "<td>";
					dlbutton($filedata[1],$filedata[2],$filedata[3],$filedata[4]);
					echo "</td>";
					echo "<td>\\\\ $filedata[1] \\ $filedata[2] \\ $dir <a href=\"file://$link\">";
					printf(" %s</a></td></tr>\n",$filedata[4]);
					$bytes = $bytes + $filedata[5];
					$rescount++;
				}
				$line++;
	
			}
			
			//fclose($indexfile);
		}
	}

	printf("</table><hr><b>%d matches %s</b><br>\n",$rescount,ffs($bytes));
	
}
//else
{
//	printf("Q == NULL<BR>\n");
//	include "form.inc";
}
