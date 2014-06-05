#!/bin/sh

for file in `find -type f -name "*.c" -or -name "*.h"`;
do
  perl batch-cleanup.pl $file
done
