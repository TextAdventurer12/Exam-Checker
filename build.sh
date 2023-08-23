VAL1=$(stat -c '%Y' main.c | tr -d '\n')
VAL2=$(cat .last_built | tr -d '\n')

if [ $VAL1 -gt $VAL2 ]
then
echo "Re-compiling"
gcc main.c -o proj
echo $VAL1 > .last_built
fi
./proj $1 $2 $3
if [[ $? -ne 0 ]]
then
    echo $?
fi