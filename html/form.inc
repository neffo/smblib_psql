<title>Samba Search</title>
<img src="filesearch.jpg">
<br>
<table width="100%"><tr><td align=left valign=top width="10%" height="100%" bgcolor="#B2B2B2">
<form action="index.php" method="POST">
Search for<br>
<input type="text" name="query" size=20 value="<? echo $query; ?>"><br>
File type<br>
<select name=ext>
	<option value="">
	<option value="mp3">Mp3s
	<option value="avi">AVI movies
	<option value="m%pg">Mpeg files
	<option value="zip">Zip files
	<option value="exe">Executables
	<option value="rar">Rar files
</select>
  <input type="submit" name="Search" value="Search" size=10>
</form>
</td>
<td>
