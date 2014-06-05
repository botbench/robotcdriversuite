#!/bin/sh

# Absolute path to this script. /home/user/bin/foo.sh
SCRIPT=$(readlink -f $0)
# Absolute path this script is in. /home/user/bin
SCRIPTPATH=`dirname $SCRIPT`

for file in `find -type f -name "*.c" -or -name "*.h"`;
do
  perl $SCRIPTPATH/batch-cleanup.pl $file
done
