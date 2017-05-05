gcc src/*.c -o build/simpleserver 
if [ $? -eq 0 ]
then
   build/simpleserver
fi

