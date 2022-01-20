#!/bin/bash
DB=/home/pi/Documents/git/six-descents-data/events.db

echo "Six Descents - Status"
echo "---------------------"
date
echo
echo "Database: $DB"
echo -n "Database size: "
ls -alh $DB \
    | awk '{print $5}' \
    | perl -pe 'chomp'
echo -n " ("
ls -al $DB \
    | awk '{print $5}' \
    | perl -pe 'chomp'
echo ")"
