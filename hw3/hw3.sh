#!/bin/bash
i=0
while getopts ":habcd:" opt; do
    case ${opt} in
    h) echo "Using regular expressions, the script works with text."
        exit 0 
    ;;
    a) input=$( ls -a )
        out=$(grep '.*\.[pP][dD][fF]$' <<< "$input")
        if [ -z "$out" ]; then
            exit 0;
        else
            grep '.*\.[pP][dD][fF]$' <<< "$input"
        fi
    ;;
    b)  
        while read -r path; do
            if [ -n "${path}" ]; then
                ARRAY+=( "${path}" )
            fi
        done
         
        while [ "$i" -lt ${#ARRAY[*]} ]
        do
            if [[ ${ARRAY[$i]} =~ ^[0-9]  ]]; then
                echo "${ARRAY[$i]}" | grep -E '^[0-9]' | sed 's/^[0-9]*//g'
            elif [[ ${ARRAY[$i]} =~ ^[+-][0-9] ]]; then
                text=${ARRAY[$i]}
                echo "$text" | grep -E '^[+-][0-9]*' | sed 's/^[+-][0-9]*//g'
            fi
            i=$(( i+1 ))
        done
    ;;
    c)  input=$(cat /dev/stdin )       
        result=$(echo "$input" | tr '\n' ' ' | grep -E -o '[[:upper:]][^.?!]*[.!?]') 
        echo "$result"
    ;;
    d)
    ;;
    \?)
        echo "Write only $(basename "$0") [-a] [-b] [-c] [-d <argument>] [-h]"
        exit 1
    ;;
    esac
done
    
