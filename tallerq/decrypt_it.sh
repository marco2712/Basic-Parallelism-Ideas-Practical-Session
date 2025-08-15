#!/bin/bash
#decript.sh
if [ $# -lt 2 ]; then
    echo "Uso: $0 <rotacion> \"texto\""
    exit 1
fi

k=$1
text="$2"

result=""
for (( i=0; i<${#text}; i++ )); do
    c="${text:$i:1}"
    if [[ "$c" =~ [a-z] ]]; then
        result+=$(printf "\\$(printf '%03o' $(( ( ( $(printf '%d' "'$c") - 97 - k ) % 26 + 26 ) % 26 + 97 )) )")
    elif [[ "$c" =~ [A-Z] ]]; then
        result+=$(printf "\\$(printf '%03o' $(( ( ( $(printf '%d' "'$c") - 65 - k ) % 26 + 26 ) % 26 + 65 )) )")
    else
        result+="$c"
    fi
done

echo "$result"
