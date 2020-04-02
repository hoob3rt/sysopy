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

breaker 'fork'
./bin/fork mask fork
./bin/fork ignore fork
./bin/fork pending fork
./bin/fork handler fork
breaker 'exec'
./bin/fork mask exec
./bin/fork ignore exec
./bin/fork pending exec
