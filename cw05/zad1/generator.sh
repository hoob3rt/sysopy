#!/usr/bin/env bash
for i in $(seq 0 $2); do printf "$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' |\
  fold -w 10 | head -n 1)\n" >> $1; done
