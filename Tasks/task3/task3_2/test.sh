#! /bin/bash
gcc -g shell_pipe.c -o shell_pipe

echo Testing parralel And Ending. Expected 5 sec
echo "sleep 5 | sleep 3" > test.txt
time ./shell_pipe < test.txt


echo Testing right pipes. Expected abc
echo "echo abc | cat" > test.txt

./shell_pipe < test.txt

echo "Testing fon mode expected abc and < 5 sec"
echo "sleep 5 | sleep 3 &" > test.txt
echo "echo abc | cat" >> test.txt

time ./shell_pipe < test.txt

echo Testing Zombies


echo "ps af > iter1.txt | sleep 5 | ps af  &" > test.txt
echo "ps af | sleep 10" >> test.txt
echo "ps af" >> test.txt


./shell_pipe < test.txt

#ps af

#echo iter1
#cat iter1.txt
#echo iter2
#cat iter2.txt
#echo ls
#cat ls.txt

exit 0
