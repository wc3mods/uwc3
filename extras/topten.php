<?php
mysql_connect ("localhost", "user", "password");
mysql_select_db ("database");
$result = mysql_query ("SELECT * FROM uwc3_xp ORDER BY xp DESC LIMIT 0,10");

?>
<html>
<head>
<title>UWC3 - Top Ten</title>
<style type="text/css">
body  {text-align: center; font-family: helvetica, arial, geneva, sans-serif; font-size: small}
table { border: 0; padding: 1px; margin: 0; }
div { margin-left: auto; margin-right; auto; }
tr { border: 0; padding: 1px; margin: 0; }
td { padding: 0; border: 1px; margin: 0; }
th { text-align: left; }
.one { background-color: #dddddd; }
.two { background-color: #cccccc; }
</style>
</head>
<body>
<div>
<table>
<tr>
<th>Name</th><th>Steam ID</th><th>XP</th><th>Last Visit</th>
<?

$class = "one";

while ($row = mysql_fetch_array($result)) {
    print "<tr>\n<td class=\"" . $class . "\">";
    print $row['name'];
    print "</td>\n<td class=\"" . $class . "\">";
    print $row['steamid'];
    print "</td>\n<td class=\"" . $class . "\">";
    print $row['xp'];
    print "</td>\n<td class=\"" . $class . "\">";
    print $row['mtime'];
    print "</td>\n</tr>";
    if ( $class == "one" ) {
      $class = "two";
    } else {
      $class = "one";
    }
}

?>
</table>
</div>
</body>
</html>
