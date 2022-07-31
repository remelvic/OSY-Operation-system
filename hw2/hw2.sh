#!/bin/bash
i_for_path=0
count_error=0
number=0
NEW_ARRAY=()
file_for_zip=()
zip_on=0
  
#switch-----------------
while getopts ":hz" opt; do
    case ${opt} in
     h ) 
        echo "the program briefly writes out info about the files"
        exit 0
        ;;
     z ) 
        zip_on=1
        ;;
     \? ) 
        exit 2
        ;;
    esac
done
#-----------------------
  
# read stdin -----------
while read -r path; do
    if [ -n "${path}" ]; then
        NEW_ARRAY+=( "${path}" )
    fi
done
#-----------------------
  
# split file in array
while [ $i_for_path -lt ${#NEW_ARRAY[*]} ]
do     
    FILE=${NEW_ARRAY[$i_for_path]}
    VAR=${FILE##*/}
    if [[ "${FILE}" != *"PATH"* ]] || [ -n "${FILE%%PATH*}" ] || [[ "${FILE#PATH }" == *"PATH"* ]]; then
        if [ -f "${FILE#PATH }" ]; then
            number=$(wc -l < "${FILE#PATH }")
            line=$(head "${FILE#PATH }" -n 1)
            echo "FILE '${FILE#PATH }' $number '$line'"
            file_for_zip+=( "${FILE#PATH }")
        else
            echo "ERROR '${FILE}'" >&2
        fi
    else
        if [ -r "${FILE#PATH }" ] && [ -f "${FILE#PATH }" ] && [ ! -L "${FILE#PATH }" ]; then # FILE
            number=$(wc -l < "${FILE#PATH }") 
            line=$(head "${FILE#PATH }" -n 1)
          
            echo "FILE '${FILE#PATH }' $number '$line'"
            file_for_zip+=( "${FILE#PATH }" )
       
        elif [ -L "${FILE#PATH }" ]; then # LINK
            LINK=$(readlink "${FILE#PATH }")
            echo "LINK '${FILE#PATH }' '${LINK##*/}'"
          
        elif [ -d "${FILE#PATH }" ]; then # DIR
            echo "DIR '${FILE#PATH }'"
      
        elif [ -f "${FILE#PATH }" ] && [ ! -r "${FILE#PATH }" ]; then # unreadable
            echo "ERROR '${FILE#PATH }'" >&2
            # count_error=$(( count_error+2 ))
            exit 2
      
        elif [ ! -f "${FILE#PATH }" ]; then # not exist, have PATH OK
            echo "ERROR '${FILE#PATH }'" >&2
            count_error=$(( count_error+1 ))  
  
        else
            DIR=${FILE%/}
            echo "ERROR '${DIR#PATH }'" >&2
            count_error=$(( count_error+1 ))
        fi
    fi
    i_for_path=$(( i_for_path+1 )) 
done
#-----------------------
  
if [ $zip_on -eq 1 ]; then
    tar czf output.tgz "${file_for_zip[@]}" 
fi
  
if [ $count_error -eq 0 ]; then
    exit 0
else
    exit 1
fi
