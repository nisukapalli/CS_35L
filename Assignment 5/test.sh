#!/bin/sh

if [ `./randall 150 | wc -c` -eq 150 ]; \
then echo "PASSED ./randall 150"; \
else echo "FAILED ./randall 150"; \
fi
   
if [ `./randall -i rdrand 100 | wc -c` -eq 100 ]; \
then echo "PASSED ./randall -i rdrand 100"; \
else echo "FAILED ./randall -i rdrand 100"; \
fi
if [ `./randall -i mrand48_r 50 | wc -c` -eq 50 ]; \
then echo "PASSED ./randall -i mrand48_r 50"; \
else echo "FAILED ./randall -i mrand48_r 50"; \
fi
   
if [ `./randall -i /dev/urandom 250 | wc -c` -eq 250 ]; \
then echo "PASSED ./randall -i /dev/urandom 250"; \
else echo "FAILED ./randall -i /dev/urandom 250"; \
fi
   
if [ `./randall -o stdout 100 | wc -c` -eq 100 ]; \
then echo "PASSED ./randall -o stdio 100"; \
else echo "FAILED./randall -o stdio 100"; \
fi
   
if [ `./randall -o 2 10 | wc -c` -eq 10 ]; \
then echo "PASSED ./randall -o 2 10"; \
else echo "FAILED ./randall -o 2 10"; \
fi


echo "./randall 0 | wc -c"
a=`(./randall 0 | wc -c)`
echo $a;

echo "./randall 5 | wc -c"
a=`(./randall 5 | wc -c)`
echo $a

echo "./randall -i rdrand 10 | wc -c"
a=`(./randall -i rdrand 10 | wc -c)`
echo $a

echo "./randall -i rdrand 15 | wc -c"
a=`(./randall -i rdrand 15 | wc -c)`
echo $a

echo "./randall -i mrand48_r 20 | wc -c"
a=`(./randall -i mrand48_r 20 | wc -c)`
echo $a

echo "./randall -i /dev/null 25 | wc -c"
a=`(./randall -i /dev/null 25 | wc -c)`
echo $a

echo "./randall -i /dev/full 25 | wc -c"
a=`(./randall -i /dev/full 30 | wc -c)`
echo $a

echo "./randall -o stdio 35 | wc -c"
a=`(./randall -o stdio 35 | wc -c)`
echo $a

echo "./randall -o 10 10 | wc -c"
a=`(./randall -o 10 10 | wc -c)`
echo $a

echo "./randall -i rdrand -o stdio 20 | wc -c"
a=`(./randall -i rdrand -o stdio 20 | wc -c)`
echo $a

echo "./randall -o stdio -i rdrand 30 | wc -c"
a=`(./randall -o stdio -i rdrand 30 | wc -c)`
echo $a
