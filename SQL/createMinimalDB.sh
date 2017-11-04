#!/bin/sh

<<<<<<< HEAD
USER=root
PASS=
=======
USER=username
PASS=yourpass
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
mysql --user="$USER" --password=$PASS -e "CREATE DATABASE IF NOT EXISTS mindb CHARACTER SET utf8 COLLATE utf8_general_ci;"
mysql --user="$USER" --password=$PASS mindb < mysqlDB.sql

