for((i = 1; i <= 20; i++));do
echo $i 
./Generator $i
time ./AhMeD_HoSSaM <input.txt> output
echo " ";
done