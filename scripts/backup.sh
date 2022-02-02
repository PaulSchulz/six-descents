#@/bin/bash

DB=/home/pi/Documents/git/six-descents-data/events.db

DIR=${DB%/*}
DBFILE=${DB##*/}

cd  $DIR
echo ".dump" | sqlite3 $DBFILE > $DBFILE-backup-`date +%Y%m%d-%H%M`.sql
