#!/usr/bin/env bash
test_suite=$1
test_files_dir=$2
files_count=$(ls $test_files_dir | wc -l)
printf "\ntesting\n"
for i in $(seq 0 $(($((files_count/3))-1)))
do
  if eval ./bin/test_suite "${test_files_dir}"/first"${i}" \
    "${test_files_dir}"/second"${i}" "${test_files_dir}"/result"${i}"; then
    printf "test ${i} PASSED\n"
  else
    printf "test ${i} FAILED\n"
  fi
done
