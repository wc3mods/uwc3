#!/usr/local/bin/perl

use DBI;

# ***************************************************************************
# Program: Conversion script to convert vault files to sql table data
# Author:  K2mia (Andrew Cowan)
# Date:    Sept. 12, 2004
# ***************************************************************************

# Instructions for Use:
# 1. Modify the $configs_dir path for your specific server 
# 2. Modify the $data_dir path for your specific server 
# 3. Locate the UWC3_sql.cfg file that came with your UWC3 zipfile
#    and make sure it is configured for your sql server
# 4. Modify the data_source as required for your server
#    Data sources can be listed with a simple perl script (example below)
#
# use DBI;
# @driver_names = DBI->available_drivers;
# @data_sources = DBI->data_sources($driver_name);
# print @data_sources;


$configs_dir = "/home/cstrike/hlds/cstrike/addons/amxmodx/configs";
$data_dir = "/home/cstrike/hlds/cstrike/addons/amxmodx/data";
$data_source = "DBI:mysql:uwc3";

# ***************************************************************************
# Do NOT Modify Anything Below This Line
# ***************************************************************************

#$DEBUG = 1;

$sqlconf_file = "${configs_dir}/UWC3/UWC3_sql.cfg";
$vault_file = "${data_dir}/vault.ini";

&read_cfg();

print "\nUWC3 Vault to SQL Data Conversion v0.0.1\n";

print "SQL Config Loaded: [$host] user=($user) pass=(*) db=($db)\n";

$dbh = DBI->connect($data_source, $user, $pass );

print "Ensuring SQL tables are created ... \n";
$sth = $dbh->prepare("CREATE TABLE IF NOT EXISTS uwc3_xp ( name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, xp INT, ip VARCHAR(32), mtime VARCHAR(32), PRIMARY KEY ( steamid ) )");
$sth->execute();
$sth = $dbh->prepare("CREATE TABLE IF NOT EXISTS uwc3_skills1 ( name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, skill1 SMALLINT, skill2 SMALLINT, skill3 SMALLINT, skill4 SMALLINT, skill5 SMALLINT, skill6 SMALLINT, skill7 SMALLINT, skill8 SMALLINT, skill9 SMALLINT, skill10 SMALLINT, PRIMARY KEY ( steamid, nset ) )");
$sth->execute();
$sth = $dbh->prepare("CREATE TABLE IF NOT EXISTS uwc3_skills2 ( name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, skill11 SMALLINT, skill12 SMALLINT, skill13 SMALLINT, skill14 SMALLINT, skill15 SMALLINT, skill16 SMALLINT, skill17 SMALLINT, skill18 SMALLINT, skill19 SMALLINT, skill20 SMALLINT, PRIMARY KEY ( steamid, nset ) )");
$sth->execute();
$sth = $dbh->prepare("CREATE TABLE IF NOT EXISTS uwc3_skills3 ( name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, skill21 SMALLINT, skill22 SMALLINT, skill23 SMALLINT, skill24 SMALLINT, skill25 SMALLINT, skill26 SMALLINT, skill27 SMALLINT, skill28 SMALLINT, skill29 SMALLINT, skill30 SMALLINT, PRIMARY KEY ( steamid, nset ) )");
$sth->execute();
$sth = $dbh->prepare("CREATE TABLE IF NOT EXISTS uwc3_skills4 ( name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, skill31 SMALLINT, skill32 SMALLINT, skill33 SMALLINT, skill34 SMALLINT, skill35 SMALLINT, skill36 SMALLINT, skill37 SMALLINT, skill38 SMALLINT, skill39 SMALLINT, skill40 SMALLINT, PRIMARY KEY ( steamid, nset ) )");
$sth->execute();
$sth = $dbh->prepare("CREATE TABLE IF NOT EXISTS uwc3_enh ( name VARCHAR(32) NOT NULL, steamid VARCHAR(32) NOT NULL, nset TINYINT NOT NULL, att1 SMALLINT, att2 SMALLINT, att3 SMALLINT, att4 SMALLINT, att5 SMALLINT, res1 SMALLINT, res2 SMALLINT, res3 SMALLINT, res4 SMALLINT, res5 SMALLINT, PRIMARY KEY ( steamid, nset ) )");
$sth->execute();


print "Ensuring SQL tables setup with steamid as primary key ... \n";
$sth = $dbh->prepare("ALTER TABLE uwc3_xp DROP PRIMARY KEY, ADD PRIMARY KEY ( steamid )");
$sth->execute();
print " * Table uwc3_xp [OK]\n";
$sth = $dbh->prepare("ALTER TABLE uwc3_enh DROP PRIMARY KEY, ADD PRIMARY KEY ( steamid, nset )");
$sth->execute();
print " * Table uwc3_enh [OK]\n";
$sth = $dbh->prepare("ALTER TABLE uwc3_skills1 DROP PRIMARY KEY, ADD PRIMARY KEY ( steamid, nset )");
$sth->execute();
print " * Table uwc3_skills1 [OK]\n";
$sth = $dbh->prepare("ALTER TABLE uwc3_skills2 DROP PRIMARY KEY, ADD PRIMARY KEY ( steamid, nset )");
$sth->execute();
print " * Table uwc3_skills2 [OK]\n";
$sth = $dbh->prepare("ALTER TABLE uwc3_skills3 DROP PRIMARY KEY, ADD PRIMARY KEY ( steamid, nset )");
$sth->execute();
print " * Table uwc3_skills3 [OK]\n";
$sth = $dbh->prepare("ALTER TABLE uwc3_skills4 DROP PRIMARY KEY, ADD PRIMARY KEY ( steamid, nset )");
$sth->execute();
print " * Table uwc3_skills4 [OK]\n";

print "Reading / Converting Vault Data Now " . (($DEBUG) ? "\n" : "");

&do_convert();

print "\nUWC3 Vault to SQL Data Conversion Completed\n\n";

exit(0);

sub do_convert(){

   open(VF, $vault_file);
   while(<VF>){
      print " ." unless $DEBUG;

      my $id, $xp, $ip, $time;
      my $att1, $att2, $att3, $att4, $att5;
      my $res1, $res2, $res3, $res4, $res5;
      my $s1, $s2, $s3, $s4, $s5, $s6, $s7, $s8, $s9, $s10;
      my $s11, $s12, $s13, $s14, $s15, $s16, $s17, $s18, $s19, $s20;
      my $s21, $s22, $s23, $s24, $s25, $s26, $s27, $s28, $s29, $s30;
      my $s31, $s32, $s33, $s34, $s35, $s36, $s37, $s38, $s39, $s40;

      chomp;
      
      if (/^(STEAM_\d+:\d+:\d+)_enh\t(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)/){
         $id = $1;
         ($att1, $att2, $att3, $att4, $att5) = ($2, $3, $4, $5, $6);
         ($res1, $res2, $res3, $res4, $res5) = ($7, $8, $9, $10, $11);

         $squery = "REPLACE INTO uwc3_enh (steamid, nset, att1, att2, att3, att4, att5, res1, res2, res3, res4, res5) VALUES ('$id', 1, $att1, $att2, $att3, $att4, $att5, $res1, $res2, $res3, $res4, $res5)";
         $res = &do_query($squery);
         if ($res){
            print "Imported Attrib/Resist Data for $id :: attribs=($att1 $att2 $att3 $att4 $att5) resists=($res1 $res2 $res3 $res4 $res5)\n" if $DEBUG;
         }else{
            print "SQL Error Importing Attrib/Resist Data for $id [ $SQL_ERR ] !!!\n";
         }

      }elsif (/^(STEAM_\d+:\d+:\d+)_\d+\t(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)/){
         $id = $1;

         ($s1, $s2, $s3, $s4, $s5, $s6, $s7, $s8, $s9, $s10) = 
            ($2, $3, $4, $5, $6, $7, $8, $9, $10, $11);         
         ($s11, $s12, $s13, $s14, $s15, $s16, $s17, $s18, $s19, $s20) = 
            ($12, $13, $14, $15, $16, $17, $18, $19, $20, $21);         
         ($s21, $s22, $s23, $s24, $s25, $s26, $s27, $s28, $s29, $s30) = 
            ($22, $23, $24, $25, $26, $27, $28, $29, $30, $31);         
         ($s31, $s32, $s33, $s34, $s35, $s36, $s37, $s38, $s39, $s40) = 
            ($32, $33, $34, $35, $36, $37, $38, $39, $40, $41); 

         $squery = "REPLACE INTO uwc3_skills1 (steamid, nset, skill1, skill2, skill3, skill4, skill5, skill6, skill7, skill8, skill9, skill10) VALUES ('$id', 1, $s1, $s2, $s3, $s4, $s5, $s6, $s7, $s8, $s9, $s10)";
         $res1 = &do_query($squery);
         $squery = "REPLACE INTO uwc3_skills2 (steamid, nset, skill11, skill12, skill13, skill14, skill15, skill16, skill17, skill18, skill19, skill20) VALUES ('$id', 1, $s11, $s12, $s13, $s14, $s15, $s16, $s17, $s18, $s19, $s20)";
         $res2 = &do_query($squery);
         $squery = "REPLACE INTO uwc3_skills3 (steamid, nset, skill21, skill22, skill23, skill24, skill25, skill26, skill27, skill28, skill29, skill30) VALUES ('$id', 1, $s21, $s22, $s23, $s24, $s25, $s26, $s27, $s28, $s29, $s30)";
         $res3 = &do_query($squery);
         $squery = "REPLACE INTO uwc3_skills4 (steamid, nset, skill31, skill32, skill33, skill34, skill35, skill36, skill37, skill38, skill39, skill40) VALUES ('$id', 1, $s31, $s32, $s33, $s34, $s35, $s36, $s37, $s38, $s39, $s40)";
         $res4 = &do_query($squery);

         if ($res1 && $res2 && $res3 && $res4){
            print "Imported Skills Data for $id :: skills=($s1 $s2 $s3 $s4 $s5 $s6 $s7 $s8 $s9 $s10 $s11 $s12 $s13 $s14 $s15 $s16 $s17 $s18 $s19 $s20 $s21 $s22 $s23 $s24 $s25 $s26 $s27 $s28 $s29 $s30 $s31 $s32 $s33 $s34 $s35 $s36 $s37 $s38 $s39 $s40)\n" if $DEBUG;
         }else{
            print "SQL Error Importing Skills Data for $id [ $SQL_ERR ] !!!\n";
         }

      }elsif (/^(STEAM_\d+:\d+:\d+)\tSTEAM_\d+:\d+:\d+\s(\d+)\s([0-9\.:]+)\s(\d+ \d+ \d+ \d+)/){
         $id = $1;
         ($xp, $ip, $time) = ($2, $3, $4);

         $squery = "REPLACE INTO uwc3_xp (steamid, xp, ip, mtime) 
                    VALUES ('$id', $xp, '$ip', '$time')";
         $res = &do_query($squery);
         if ($res){
            print "Imported XP Data for $id :: xp=($xp) ip=($ip) time=($time)\n" if $DEBUG;
         }else{
            print "SQL Error Importing XP Data for $id [ $SQL_ERR ] !!!\n";
         }
      }
 
   }
   close(VF);

}

sub do_query(){
   my $sq = shift;

   #print "sq=($sq)\n";

   $sth = $dbh->prepare($sq);
   $sth->execute();

   if ($dbh->err()){
      $SQL_ERR = $dbh->errstr();
      return 0;
   }
   return 1;
}

sub read_cfg(){

   open(CF, $sqlconf_file);
   while(<CF>){
      chomp;
      next if /^#/;

      if (/UW_sql_host\s+"(.*)"/){ $host = $1; }
      if (/UW_sql_user\s+"(.*)"/){ $user = $1; }
      if (/UW_sql_pass\s+"(.*)"/){ $pass = $1; }
      if (/UW_sql_db\s+"(.*)"/){ $db = $1; }
 
   }
   close(CF);

   if (!($host && $user && $pass && $db)){
      die "\nvault_to_dbi.pl :: Missing SQL config data\n\n";
   }
}

