#!/bin/bash

fileName="./admin/user.txt"
#admin_password=$2


while true
do
	read -p "ADMIN> " name
	case $name in
		"CREATE USER")
			read -p "Enter a New username : " username
			if [ $(cat $fileName | grep "$username,"|wc -l) -ne 0 ];
			then
				echo "User already exists"
				break
			fi

			read -sp "Enter a New password : " new_password
			echo ""
			read -sp "Confirm password : " conf_password
			echo ""

			if [ $new_password != $conf_password ]
			then
				echo "User Creation Failed, Confirm password doesn't match"
				break
			fi

			read -p "Enter a database name : " dataBaseName

			if [ $(cut -d',' -f3 "$fileName" | grep "$dataBaseName" | wc -l) -eq 0 ]
			then
				mkdir "./admin/$database" # && chmod "./admin/$dataBaseName" 766
			fi
			echo "$username,$conf_password,$dataBaseName" >> $fileName
			;;
		"CREATE DATABASE")
			read -p "Enter a Database Name : " dataBaseName
			if [ $(cut -d',' -f3 fileName | grep $dataBaseName | wc -l) -eq 0 ]
			then
				mkdir "./admin/$dataBaseName" -m 766
			else
				echo "Database Already Exists"
			fi

			echo "-,-,$dataBaseName" >> $fileName
			;;
		"VIEW LOG")
			echo ""
			cat $fileName
			;;
		"LOGOUT")
			break
	esac
done
