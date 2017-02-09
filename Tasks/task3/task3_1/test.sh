#!/bin/bash
echo "success" > success.txt
test_cmd="echo success"
suc_cmd="true true"
err_cmd="false false"
suc=0
total=0

echo command $suc_cmd "||" $suc_cmd
./shell_andor $suc_cmd "||" $suc_cmd && $test_cmd && suc=$(($suc + 1)) ; total=$(($total + 1)) 
echo command $suc_cmd "||" $err_cmd 
./shell_andor $suc_cmd "||" $err_cmd && $test_cmd && suc=$(($suc + 1)) ; total=$(($total + 1))
echo command $err_cmd "||" $suc_cmd 
./shell_andor $err_cmd "||" $suc_cmd && $test_cmd && suc=$(($suc + 1)) ; total=$(($total + 1))
echo command $err_cmd "||" $err_cmd 
./shell_andor $err_cmd "||" $err_cmd || $test_cmd && suc=$(($suc + 1)) ; total=$(($total + 1))
echo command $suc_cmd "&&" $suc_cmd 
./shell_andor $suc_cmd "&&" $suc_cmd && $test_cmd && suc=$(($suc + 1)) ; total=$(($total + 1))
echo command $suc_cmd "&&" $err_cmd
./shell_andor $suc_cmd "&&" $err_cmd || $test_cmd && suc=$(($suc + 1)) ; total=$(($total + 1))
echo command $err_cmd "&&" $suc_cmd 
./shell_andor $err_cmd "&&" $suc_cmd || $test_cmd && suc=$(($suc + 1)) ; total=$(($total + 1))
echo command $err_cmd "&&" $err_cmd
./shell_andor $err_cmd "&&" $err_cmd || $test_cmd && suc=$(($suc + 1)) ; total=$(($total + 1))
echo command $suc_cmd ";" $suc_cmd 
./shell_andor $suc_cmd ";" $suc_cmd && $test_cmd && suc=$(($suc + 1)) ; total=$(($total + 1))
echo command $suc_cmd ";" $err_cmd 
./shell_andor $suc_cmd ";" $err_cmd || $test_cmd && suc=$(($suc + 1)) ; total=$(($total + 1))
echo command $err_cmd ";" $suc_cmd 
./shell_andor $err_cmd ";" $suc_cmd && $test_cmd && suc=$(($suc + 1)) ; total=$(($total + 1))
echo command $err_cmd ";" $err_cmd  
./shell_andor $err_cmd ";" $err_cmd || $test_cmd && suc=$(($suc + 1)) ; total=$(($total + 1))

echo "sucsess / total" $suc / $total

echo expect 8 sec
time ./shell_andor sleep 5 "&&" sleep 3

echo expect 5 sec
time ./shell_andor sleep 5 "||" sleep 3

exit 0
