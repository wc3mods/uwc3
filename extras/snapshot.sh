#!/bin/bash
# Take snapshots from svn, build UWC3, make packages of binary and source
# IdleNexus Gaming, Derek Evan Mart, Sun Jul 30 13:56:36 EDT 2006

base=/home/uwc3-dev

cd $base

rm -rf $base/uwc3/

svn checkout http://svn.marticus.org/svn/uwc3ng/branches/idlenexus/uwc3/ > $base/revision.log 2>&1

revision=`cat $base/revision.log | grep revision`
revision=${revision#*revision\ }
revision=${revision%*.}
export revision

perl -pi -e 's/(.*)\d*(\"\})/$1-$ENV{revision}$2/g' $base/uwc3/uwc3.sma

datestamp=`date +%Y%m%d`

find $base/uwc3/ -name '.svn' -print0 | xargs -0 rm -rf

file=$base/snapshots/uwc3-snapshot-$datestamp-$revision

rm -f $file* 

cp -a $base/uwc3/* $base
$base/amxxpc $base/uwc3/uwc3.sma -o$base/uwc3-snapshot-$datestamp-$revision.amxx > /dev/null 2>&1
rm -rf $base/uwc3/*.inl
rm -f $base/uwc3.sma

wget -q -O $base/snapshots/changelog.txt http://svn.marticus.org/viewvc/viewvc.cgi/*checkout*/uwc3ng/branches/idlenexus/changelog.txt

tar -cf ${file}.tar $base/uwc3-snapshot-$datestamp-$revision.amxx
bzip2 -zkfq9 ${file}.tar
gzip -frq9 ${file}.tar
zip -q -9 ${file}.zip $base/uwc3-snapshot-$datestamp-$revision.amxx

tar -cf ${file}-src.tar uwc3/
bzip2 -zkfq9 ${file}-src.tar
gzip -frq9 ${file}-src.tar
zip -q -9 ${file}-src.zip uwc3/uwc3.sma uwc3/uwc3/*

rm -f $file.amxx
