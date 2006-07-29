<?php
mysql_connect ( "localhost", "user", "password" );
mysql_select_db ( "database" );

$authpass = 'password'; /* not secure at all, use ssl and htaccess */

if ( ! $_POST['SteamID'] || ( $_POST['AuthPass'] != $authpass ) ) {
  if ( $_POST['from_form'] ) {
    print ( "<b>Missing required fields!</b><br />" );
  }
  showform();
} else {
  if ( ! $_POST['do_it_now'] ) {
    confirm();
  } else {
    deleteid();
  }
}

function confirm() {

  $query = sprintf ( "SELECT * FROM uwc3_xp WHERE steamid='%s' LIMIT 1", 
                      mysql_real_escape_string ( $_POST['SteamID'] ) );
  $result = mysql_query ( $query ) OR die ( mysql_error() );
  $row = mysql_fetch_assoc ( $result );
  if ( ! $row ) {
    print ( "<b>The SteamID [</b>" . $_POST['SteamID'] . 
            "<b>] does not exist!</b><br />" );
    showform();
    exit;
  } else {
?>
<style>
tr { padding: 0px; margin: 0px; border: 0px; line-height: 12px; font-size: 14px }
td { padding: 0px; margin: 0px; border: 0px }
table { padding: 0px; margin: 0px; border: 0px }
span { text-align: right }
</style>
The following user is about to be deleted:<br />
<table style="border: 1px solid black; padding: 1px; margin: 1px">
<?
    foreach ( $row as $key => $value ) {
      print ( "<tr><td><span>$key: </span></td><td>$value</td></tr>" );
    }
?>
</table>
Are you sure this is what you want to do?<br />

<form method="post" action="<? echo $_SERVER['PHP_SELF'] ?>">
<input type="hidden" name="SteamID" value="" />
<input type="hidden" name="AuthPass" value="" />
<input type="hidden" name="from_form" value="" />
<input type="submit" value="No - Do Not Delete This Player" />
</form>

<form method="post" action="<? echo $_SERVER['PHP_SELF'] ?>">
<input type="hidden" name="SteamID" value="<? echo $_POST['SteamID'] ?>" />
<input type="hidden" name="AuthPass" value="<? echo $_POST['AuthPass'] ?>" />
<input type="hidden" name="from_form" value="true" />
<input type="hidden" name="do_it_now" value="true" />
<input type="submit" value="Yes - Delete This Player" />
</form>
<?
  }
}

function deleteid() {
  $tables = "uwc3_enh, uwc3_skills1, uwc3_skills2, uwc3_skills3, uwc3_skills4, uwc3_xp";

  $query = sprintf ( "SELECT * FROM uwc3_xp WHERE steamid='%s' LIMIT 1", 
                      mysql_real_escape_string ( $_POST['SteamID'] ) );
  $result = mysql_query ( $query ) OR die ( mysql_error() );
  $row = mysql_fetch_assoc ( $result );
  $now = date ( 'd/m/Y G:i:s' );
  $logfile = fopen ( 'deleteid.log', 'a' ) OR die ( "Cannot open log file" );
  $line = "[$now] ( ";
  $i = 0;
  foreach ( $row as $key => $value ) {
    $line .= "'$value'";
    if ( $i < 4 ) {
      $i++;
      $line .= ", ";
    }
  }
  $line .= " )\r\n";
  print ( "<b>LOG</b> " . $line . "<br />" );
  fputs ( $logfile, $line );
  fclose ( $logfile );

  $tables = array ( "uwc3_enh", "uwc3_skills1", "uwc3_skills2", 
                    "uwc3_skills3", "uwc3_skills4", "uwc3_xp" );

  foreach ( $tables as $table ) {
    $query = sprintf ( "DELETE FROM %s WHERE steamid='%s' LIMIT 1",
                        mysql_real_escape_string ( $table ),
                        mysql_real_escape_string ( $_POST['SteamID'] ) );
    $result = mysql_query ( $query ) OR die ( mysql_error() );
    print ( "<b>QUERY </b>" . $query . "<br />" );
  }

  print ( "The player with SteamID: [" . $_POST['SteamID'] . 
          "] has been removed." );
}

function showform() {
?>
Delete a player<br />
<form method="post" action="<? echo $_SERVER['PHP_SELF'] ?>">
SteamID: <input type="textbox" name="SteamID" style="width: 150" 
                value="<? echo $_POST['SteamID'] ?>" /><br />
Password: <input type="password" name="AuthPass" style="width: 47" 
                 maxlength="8" /><br />
<input type="hidden" name="from_form" value="true" />
<input type="submit" value="Delete" />
<?
}
?>
