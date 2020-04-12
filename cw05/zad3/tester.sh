#!/usr/bin/env bash
function exec_bin(){
  if [ "$#" -eq 4 ]; then
    # path fifo file len
    ./$1 $2 $3 $4;
  else printf "wrong number of arguments\n" exit 1; fi
}
printf "testing\n"
if [ ! -f $3 ]; then
  mkfifo $3
  for x in $(seq 0 4); do
    for y in $(seq 0 5); do
      printf "$(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w 5 | \
        head -n 1)\n" >> "file$x"
    done
  done
  exec_bin $1 $3 file0 5 & exec_bin $1 $3 file1 5 & exec_bin $1 $3 file2 5 & 
  exec_bin $1 $3 file3 5 & exec_bin $1 $3 file4 5 & exec_bin $2 $3 output 5
  wait
  diff -w <(cat file* | grep -o . | sort | tr -d "\n") <(cat output | \
    grep -o .  | sort | tr -d "\n") && printf "test passed\n" \
    ||  printf "test failed\n"
  rm -f $3 output
  for x in $(seq 0 4); do rm -f "file$x"; done
else printf"cannot create fifo, file exists\n" exit 1; fi
