<?

function option($v,$n,$e)
{
	echo "<option value=\"$v\"";
	if ( $v != "" && $v == $e)
		echo " selected";
	echo ">$n\n";
}
?>
<form action="smbsearch.php" method="POST">
<b>Search for</b><br>
<input type="text" name="query" size=20 value="<? echo $q; ?>" ><select name=sign value="<? echo $sign; ?>">
<?
	option("true","Normal",$sign);
	option("false","Negative",$sign);
?>
</select>
<br>
<b>File type</b><br>
<select name="e" value="<? echo $e; ?>" >
<?
	option("","",$e);
	option("","---- Audio ----",$e);
	option("mp3","MP3",$e);
	option("ogg","OGG",$e);
	option("wav","WAV",$e);
	option("","---- Video ----",$e);
	option("avi","AVI",$e);
	option("mpg","MPG",$e);
	option("mpeg","MPEG",$e);
	option("rm","RM",$e);
	option("","---- Images ----",$e);
	option("jpg","JPG",$e);
	option("jpeg","JPEG",$e);
	option("gif","GIF",$e);
	option("png","PNG",$e);
	option("bmp","BMP",$e);
	option("","---- Archives ----",$e);
	option("zip","ZIP",$e);
	option("gz","GZIP",$e);
	option("bz2","BZ2",$e);
	option("rar","RAR",$e);
	option("iso","ISO",$e);
	option("bin","BIN",$e);
	option("img","IMG",$e);
	option("cue","CUE",$e);
	option("ccd","CCD",$e);
	option("","---- Documents ----",$e);
	option("doc","DOC",$e);
	option("txt","TXT",$e);
	option("htm","HTM",$e);
	option("html","HTML",$e);
	option("pdf","PDF",$e);
	option("ppt","PPT",$e);
	option("","---- Misc ----",$e);
	option("exe","EXE",$e);
/*	<option value="">
	<option value="">---- Media Files ----
	<option value="mp3">MP3
	<option value="ogg">OGG
	<option value="avi">AVI
	<option value="mpg">MPG
	<option value="rm">RM
	<option value="">---- Image Files ----
	<option value="jpg">JPG
	<option value="jpeg">JPEG
	<option value="gif">GIF
	<option value="png">PNG
	<option value="bmp">BMP
	<option value="">---- Archives ----
	<option value="zip">ZIP
	<option value="gz">GZIP
	<option value="bz2">BZ2
	<option value="rar">RAR
	<option value="iso">ISO
	<option value="bin">BIN
	<option value="">---- Documents ----
	<option value="doc">DOC
	<option value="txt">TXT
	<option value="htm">HTM
	<option value="html">HTML
	<option value="pdf">PDF
	<option value="">---- Misc ----
	<option value="exe">EXE*/
?>
</select><br>
<b>Size (MB)</b><br>
<input type="text" name="sz" size=20 value="<? echo $sz; ?>" >
<select name="sc" value="<? echo $sc; ?>" >
<?
	option("gt","or greater",$sc);
	option("lt","or less",$sc);
	option("ap","or close too",$sc);
?>
</select><br>
<b>Ignore hosts</b><br>
<input type="text" name="hosts" size=50 value="<? echo $hosts; ?>" ><br>
	
  <input type="submit" name="Search" value="Search" size=10>
</form>
