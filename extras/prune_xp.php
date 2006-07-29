<?php
mysql_connect ( "localhost", "user", "password" );
mysql_select_db ( "database" );

/* WARNING!!! use htaccess to protect this script */

//$result = mysql_query ( "SELECT * FROM uwc3_xp WHERE xp < 2000 ORDER BY xp DESC LIMIT 0,50" );

//$now = date ( 'm d G i' );
//list ( $mon, $day, $hour, $min ) = explode ( " ", $now );

$now = time();

/* Now - Last Seen = Difference
   One Month: 2679824
   Two Months: 5357384
   Three Months: 7955504 */

$purgelog = fopen ( '../purge_xp_log.html', 'a' );

$line = date( 'D M d H:i:s T Y' ) . '<br /><br />' . "\r\n";
print ( $line );
//fputs ( $purgelog, $line );

$line = '<br /><b>Working on uwc3_xp ...</b>' . "\r\n";
print ( $line );
//fputs ( $purgelog, $line );

/* Process the uwc3_xp table and find stale data */
$query = "SELECT steamid, xp, mtime FROM uwc3_xp";
$result = mysql_query ( $query );
while ( $row = mysql_fetch_row ( $result ) ) {

  $steamid = $row[0];
  $xp = $row[1];
  $mtime = $row[2];

  /* Change uwc3 time stamp into unix time */
  list ( $umon, $uday, $uhour, $umin ) = explode ( " ", $mtime );
  $lastjoin = mktime ( $uhour, $umin, 0, $umon, $uday );

  /* Away for longer than a month and less than level 5 (3022XP)*/
  if ( ( $now - $lastjoin > 2679824 ) && $xp < 3022 ) {

    $line = "Processing $steamid [XP: $xp] [Seen: $mtime]... ";
    print ( $line );
    //fputs ( $purgelog, $line );

    $query2 = sprintf ( "DELETE FROM uwc3_xp WHERE steamid='%s' LIMIT 1", 
      mysql_real_escape_string ( $steamid ) );
    //mysql_query ( $query2 );

    $line = '&gt; 1M &amp; &lt; L5 <font color=red>deleted</font>.<br />' . "\r\n";
    print ( $line );
    //fputs ( $purgelog, $line );

  }

  /* Away for longer than three months and xp won't matter */
  if ( $now - $lastjoin > 7955504 ) {

    $line = "Processing $steamid [XP: $xp] [Seen: $mtime]... ";
    print ( $line );
    //fputs ( $purgelog, $line );

    $query2 = sprintf ( "DELETE FROM uwc3_xp WHERE steamid='%s' LIMIT 1", 
      mysql_real_escape_string ( $steamid ) );
    //mysql_query ( $query2 );

    $line = '&gt; 3M <font color=red>deleted</font>.<br />' . "\r\n";
    print ( $line );
    //fputs ( $purgelog, $line );

  } 

  /* print ( 'mtime: ' . $mtime . ' lastjoin: ' . $lastjoin .  ' now: ' . $now . '<br />' ); */

}

/* Remove data from other tables if it has been removed from the xp table */
$tables = array ( "uwc3_skills1", "uwc3_skills2", "uwc3_skills3", "uwc3_skills4", "uwc3_enh" );

foreach ( $tables as $table ) {
  $line = "<br /><b>Working on $table ...</b>\r\n";
  print ( $line );
  //fputs ( $purgelog, $line );

  $query = sprintf ( "SELECT steamid FROM %s", 
    mysql_real_escape_string ( $table ) );
  $result = mysql_query ( $query );

  while ( $row = mysql_fetch_row ( $result ) ) {
    $steamid = $row[0];
    $query2 = sprintf ( "SELECT steamid FROM uwc3_xp WHERE steamid='%s'",
      mysql_real_escape_string ( $steamid ) );
    $result2 = mysql_query ( $query2 );
    $row2 = mysql_fetch_row ( $result2 );

    if ( ! $row2 ) {

      $line = "<br />Processing $steamid... \r\n";
      print ( $line );
      //fputs ( $purgelog, $line );

      $query3 = sprintf ( "DELETE FROM %s WHERE steamid='%s' LIMIT 1", 
        mysql_real_escape_string ( $table ),
        mysql_real_escape_string ( $steamid ) );
      //mysql_query ( $query3 );
      
      $line = "<font color=red>deleted</font>.\r\n";
      print ( $line );
      //fputs ( $purgelog, $line );
    }
  }
}
?>
