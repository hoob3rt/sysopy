#!/bin/bash
function mkdir_cd ()
{
    if [[ $# = 0 ]]; then
        echo 'enter directory name'
        read directory
    else
        directory=$1
    fi
    mkdir -p "$directory" && cd "$directory"
}

function create_file_symlink(){
  filename=$(cat /dev/urandom | tr -cd 'a-f0-9' | head -c 5) 
  dd if=/dev/urandom bs=1024 count=1 status=none> $filename 
  ln -s $filename "${filename}"symlink
}

echo "creating test directories"
main_dir="test"
mkdir_cd $main_dir
for x in {0..3}
do
  mkdir_cd dir"${x}"
  for y in {0..3}
  do
    mkdir_cd dir"${x}"_"${y}"
    for z in {0..3}
    do
      create_file_symlink
    done
    create_file_symlink
    cd ../
  done
  create_file_symlink
  cd ../
  create_file_symlink
done
