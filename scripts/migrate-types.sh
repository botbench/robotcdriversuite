#!/bin/sh

for file in `find -type f -name "*.c" -or -name "*.h"`;
do
  echo "$file"
  sed -i 's/\bint\b/short/' $file
  sed -i 's/\bint\b/short/' $file
  sed -i 's/ int / short /' $file
done
