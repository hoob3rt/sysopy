#!/usr/bin/env bash
declare -a commands=("pwd" "ls" "uname -r" "cal" "hostname" "whoami" "w" \
  "df" "du -sh" "lsblk")
for i in $(seq 0 $2); do printf "${commands[$[$RANDOM % ${#commands[@]}]]}\n" \
  >> $1; done
