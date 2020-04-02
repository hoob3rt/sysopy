#!/usr/bin/env bash

function breaker(){
  printf "\n"
  charlen=${#1}
  cols=$(tput cols)
  len="$(($(($cols-$charlen))/2))"
  for i in $(seq 1 $len);do printf "-";done;
  printf $1
  for i in $(seq 1 $len);do printf "-";done;
  printf "\n"
}

pkill catcher && pkill sender
breaker sigrt
echo $COLUMNS
./bin/catcher sigrt&
./bin/sender `pidof catcher` 100 sigrt
breaker kill
./bin/catcher kill&
./bin/sender `pidof catcher` 100 kill
breaker sigqueue
./bin/catcher sigqueue&
./bin/sender `pidof catcher` 100 sigqueue
