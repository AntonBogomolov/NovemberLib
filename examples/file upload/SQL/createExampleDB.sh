#!/bin/sh

USER=root
PASS=651065
mysql --user="$USER" --password=$PASS -e "CREATE DATABASE IF NOT EXISTS novemberFileUploadExample CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql --user="$USER" --password=$PASS novemberFileUploadExample < mysqlDB.sql

