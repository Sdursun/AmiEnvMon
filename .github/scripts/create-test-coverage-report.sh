#!/bin/bash

echo '# Test coverage report'
echo -e '| File | Coverage (%) |\n|---|---|'

# TODO: any way we can include all C files in report somehow? there are files not compiled and covered during testing. Even if they just show 0% it shows a more complete picture...
for objfile in $(ls build-dev/*.o | grep --invert-match  _test)
do
    GCOV_RESULT=$(gcov -n $objfile)
    FILENAME=$(echo "$GCOV_RESULT" | sed -n -E 's/File .(src\/.*)./\1/p' | tail -n1)

    # the for-loop also contains test files, which should not be included. They cause empty filenames
    if [ -z $FILENAME ]
    then
        break
    fi
    
    COVERAGE=$(echo "$GCOV_RESULT" | sed -n -E 's/Lines executed:(.*)% of .*/\1/p' | tail -n1)
    echo "| $FILENAME | $COVERAGE |"
done
