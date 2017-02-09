#!/bin/bash

#mkdir books
cd books

#wget `cat ../books_urls`

echo > ../log 
for i in 3 4 5
do 

#echo bubble $((10 ** $i)) >> ../log		
#{ time cat *txt | head -n $((10 ** $i))  | ../linesort -b > ../bubble.txt; } 2>> ../log


#echo >> ../log		

#echo merge $((10 ** $i)) strings >> ../log		
#{ time cat *txt *txt *txt *txt *txt *txt *txt | head -n $((10 ** $i)) | ../linesort -m > ../merge.txt; } 2>> ../log

#echo >> ../log		

echo tree $((10 ** $i)) strings >> ../log
{ time cat *txt *txt *txt *txt *txt *txt *txt | head -n $((10 ** $i)) | ../linesort_tree > ../tree.txt; } 2>> ../log

echo >> ../log		
done
cd ..

#rm -r books

exit 0
