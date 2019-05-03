#!/usr/bin/env bash

make

for name in hd01 hd02 hd07 hd08 hd09 hd12 nk01
do

echo "${name}:"
echo "  encode"
time ./huff_codec -w 512 -c -h static -i "./data/${name}.raw" -o "./data/encoded/${name}.huff"
echo "  dencode"
time ./huff_codec  -d -h static -i "./data/encoded/${name}.huff" -o "./data/out/${name}.raw"
echo "  diff"
diff "./data/out/${name}.raw" "./data/${name}.raw"


echo "  encode m"
time ./huff_codec -w 512 -c -h static -i "./data/${name}.raw" -o "./data/encoded/${name}m.huff" -m
echo "  dencode m"
time ./huff_codec -w 512 -d -h static -i "./data/encoded/${name}m.huff" -o "./data/out/${name}m.raw" -m
echo "  diff"
diff "./data/out/${name}m.raw" "./data/${name}.raw"



echo "  encode a"
time ./huff_codec -w 512 -c -h adaptive -i "./data/${name}.raw" -o "./data/encoded/${name}a.huff"
echo "  dencode a"
time ./huff_codec -w 512 -d -h adaptive -i "./data/encoded/${name}a.huff" -o "./data/out/${name}a.raw"
echo "  diff"
diff "./data/out/${name}a.raw" "./data/${name}.raw"



echo "  encode am"
time ./huff_codec -w 512 -c -h adaptive -i "./data/${name}.raw" -o "./data/encoded/${name}am.huff" -m
echo "  dencode am"
time ./huff_codec -w 512 -d -h adaptive -i "./data/encoded/${name}am.huff" -o "./data/out/${name}am.raw" -m
echo "  diff"
diff "./data/out/${name}am.raw" "./data/${name}.raw"


echo "  sizes"
printf "    Original: "
ls -alh ./data/ | grep "${name}.raw" | awk '{ print $5 }'
printf "    Huff: "
ls -alh ./data/encoded/ | grep "${name}.huff" | awk '{ print $5 }'
printf "    Huff m: "
ls -alh ./data/encoded/ | grep "${name}m.huff" | awk '{ print $5 }'
printf "    Huff a: "
ls -alh ./data/encoded/ | grep "${name}a.huff" | awk '{ print $5 }'
printf "    Huff am: "
ls -alh ./data/encoded/ | grep "${name}am.huff" | awk '{ print $5 }'

done
make clean