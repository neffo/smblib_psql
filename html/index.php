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

print_header();

if ( $ext != "" )
{
$pgc = pg_connect("dbname=ineffable");
 $cmd = "SELECT * FROM file WHERE file_name LIKE '%.".$ext."'";
if ( $query != "" )
	$cmd = $cmd." AND file_name LIKE '%".$query."%'";
$count = 20;
$res = pg_exec ( $pgc, $cmd);
 $pgt = pg_numrows( $res );
 $start = $skip;
 $end = $start + $count;
 if ( $end > $pgt )
 	$end = $pgt;
if ($pgt > 0 )
{
 if ( $pgt > $count )
         printf("&nbsp; <b>Listing %d to %d out of %d<b><br>\n",($start+1),$end,$pgt);
	print_next($pgt, $start, $count, $ext, $query);
echo "<table width=\"100%\">\n";
//echo "<tr><td><b>HOST</b></td><td><b>FILE</b></td><td align=right><b>SIZE</b></td></tr>\n";
echo "<tr><td width=\"10%\"><b>HOST</b></td><td width=\"90%\"><b>FILE</b></td></tr>\n";
	$grey="bgcolor=\"#D0D0D0\" ";
	$bg="";
 for($i=$start;$i<$end;$i++)
 {
        echo "<tr>\n";
	 $rarr = pg_fetch_array ($res,$i);
//	 echo $rarr["file_name"];
	$sdir = "";
	if ( $rarr["dir_name"] != "" )
		$sdir = $rarr["dir_name"]."/";
	if ($bg == "")
		$bg=$grey;
	else
		$bg="";
	 printf("<tr><td %s width=\"10%%\"><a href=\"file:////%s/%s%s\">%s</a></td>\n",$bg,$rarr["host_name"],$rarr["share_name"],$sdir,$rarr["host_name"]);
	 printf("<td width=\"90%%\" %s ><a href=\"file:////%s/%s/%s%s\">%s</a> <font color=\"red\">%s</font></td></tr>\n",$bg,$rarr["host_name"],$rarr["share_name"],$sdir,$rarr["file_name"],$rarr["file_name"],ffs($rarr["size"]));
         echo "</tr>\n";
 }
echo "</table>\n";
/*
if ($pgt > $start + $count)
{
	echo "<form action=\"index.php\" method = \"POST\">";
	echo "<input type=\"hidden\" name=\"ext\" value=\"".$ext."\">";
	if ($query != "")
		echo "<input type=\"hidden\" name=\"query\" value=\"".$query."\">";
	echo "<input type=\"hidden\" name=\"skip\" value=\"".($start+$count)."\">";
	echo "<input type=\"submit\" name=\"submit\" value=\"Next ".$count."\">";
	echo "</form>";
}*/
	print_next($pgt, $start, $count, $ext, $query);
}
else
{
	echo "NO FILES FOUND.<br>\n";
}
pg_freeresult($res);
 pg_close($pgc);
}
print_footer();

function print_next($pgt, $start, $count, $ext, $query)
{

	echo "<table width=\"100%\"><tr><td align=left width=\"50%\">\n";
	$disabled="";

	if (($start - $count) < 0 )
		$disabled="disabled";

	echo "<form action=\"index.php\" method = \"POST\">";
	echo "<input type=\"hidden\" name=\"ext\" value=\"".$ext."\">";
	if ($query != "")
		echo "<input type=\"hidden\" name=\"query\" value=\"".$query."\">";
	echo "<input type=\"hidden\" name=\"skip\" value=\"".($start-$count)."\">";
	echo "<input type=\"submit\" ".$disabled." name=\"submit\" value=\"&lt; Prev ".$count."\">";
	echo "</form>\n";

	echo "</td><td align=right width=\"50%\">\n";

	$disabled="disabled";

	if ($pgt > $start + $count)
		$disabled="";

	echo "<form action=\"index.php\" method = \"POST\">";
	echo "<input type=\"hidden\" name=\"ext\" value=\"".$ext."\">";
	if ($query != "")
		echo "<input type=\"hidden\" name=\"query\" value=\"".$query."\">";
	echo "<input type=\"hidden\" name=\"skip\" value=\"".($start+$count)."\">";
	echo "<input type=\"submit\" ".$disabled." name=\"submit\" value=\"Next ".$count." &gt;\">";
	echo "</form>\n";
	echo "</td></tr></table>\n";
}


function print_header()
{
	include("form.inc");
}

function print_footer()
{
	echo "</td></tr><br>\n";
}
