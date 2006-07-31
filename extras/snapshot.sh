#!/bin/bash

base=/home/uwc3-dev

cd $base

svn checkout http://svn.marticus.org/svn/uwc3ng/branches/idlenexus/uwc3/ > $base/revision.log 2>&1

count=`wc -l $base/revision.log`
if [ "${count%\ *}" == 1 ]; then
  exit 0
fi

revision=`cat $base/revision.log | grep revision`
revision=${revision#*revision\ }
revision=${revision%*.}
export revision

perl -pi -e 's/(.*)\d*(\"\})/$1-$ENV{revision}$2/g' $base/uwc3/uwc3.sma

datestamp=`date +%Y%m%d`

if [ ! -d $base/tmp-svn ]; then
  mkdir -p $base/tmp-svn/uwc3
fi

mv $base/uwc3/.svn $base/tmp-svn/
mv $base/uwc3/uwc3/.svn $base/tmp-svn/uwc3/

file=$base/snapshots/uwc3-snapshot-$datestamp-$revision

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

mv $base/tmp-svn/.svn $base/uwc3
mv $base/tmp-svn/uwc3/.svn $base/uwc3/uwc3/
