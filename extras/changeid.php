<?php
mysql_connect ( "localhost", "user", "password" );
mysql_select_db ( "database" );

$authpass = 'changeme'; /* not secure at all, must use ssl and htaccess */

if ( ! $_POST['OldSteamID'] || ! $_POST['NewSteamID'] || 
   ( $_POST['AuthPass'] != $authpass ) ) {
  if ( $_POST['from_form'] ) {
    print ( "<b>Missing required fields!</b><br />" );
  }
  showform();
} else {
  changeid();
}

function changeid() {

  /* Find the new steamid in uwc3_xp table */
  $query = sprintf ( "SELECT steamid FROM uwc3_xp WHERE steamid='%s' LIMIT 1", 
                      mysql_real_escape_string ( $_POST['NewSteamID'] ) );
  $result = mysql_query ( $query );
  if ( mysql_fetch_row ( $result ) ) {
    print ( "<b>The New SteamID [" . $_POST['NewSteamID'] . 
            "] already exists</b><br />" );
    showform();
    exit;
  }

  $query = sprintf ( "SELECT steamid FROM uwc3_xp WHERE steamid='%s' LIMIT 1", 
                      mysql_real_escape_string ( $_POST['OldSteamID'] ) );
  $result = mysql_query ( $query );
  if ( ! mysql_fetch_row ( $result ) ) {
    print ( "<b>The Old SteamID [</b>" . $_POST['OldSteamID'] . 
            "<b>] does not exist!</b><br />" );
    showform();
    exit;
  } else {
    $tables = array ( "uwc3_enh", "uwc3_skills1", "uwc3_skills2", 
                      "uwc3_skills3", "uwc3_skills4", "uwc3_xp" );
    $now = date ( 'd/m/Y G:i:s' );
    $logfile = fopen ( 'changeid.log', 'a' ) OR die ( "Cannot open log file" );
    $oldsteamid = $_POST['OldSteamID'];
    $newsteamid = $_POST['NewSteamID'];
    $line = "[$now] Updated: $oldsteamid -> $newsteamid\r\n";
    print ( "<b>LOG</b> " . $line . "<br />" );
    fputs ( $logfile, $line );
    fclose ( $logfile );

    foreach ( $tables as $table ) {
  
     $query = sprintf ( "UPDATE %s SET steamid='%s' WHERE steamid='%s' LIMIT 1", 
                        mysql_real_escape_string ( $table ),
                        mysql_real_escape_string ( $_POST['NewSteamID'] ),
                        mysql_real_escape_string ( $_POST['OldSteamID'] ) );
      
      print ( "<b>QUERY </b>" . $query . "<br />" );
      $result = mysql_query ( $query );

    }

    print ( "The SteamID [" . $_POST['OldSteamID'] . "] has been changed to [" .
            $_POST['OldSteamID'] . "]" );

  }
}

function showform() {
?>
Update a player's SteamID<br />
<form method="post" action="<? echo $_SERVER['PHP_SELF'] ?>">
Old ID: <input type="textbox" name="OldSteamID" style="width: 150" 
               value="<? echo $_POST['OldSteamID'] ?>" /><br />
New ID: <input type="textbox" name="NewSteamID" style="width: 150"
               value="<? echo $_POST['NewSteamID'] ?>" /><br />
Password: <input type="password" name="AuthPass" style="width: 47" 
                 maxlength="8" /><br />
<input type="hidden" name="from_form" value="true" />
<input type="submit" value="Update" />
</form>
<?
}
?>
