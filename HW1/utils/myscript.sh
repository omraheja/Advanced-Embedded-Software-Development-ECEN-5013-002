#!bin/bash

clear
echo "User Information                  \n\r>>$(whoami)\n\r" > myscript.txt                     #Command to find Username
echo "Operating System Type/Brand       \n\r>>$(lsb_release -d)\n\r" >> myscript.txt            #Command to find the OS Brand/Type
echo "OS Distribution                   \n\r>>$(lsb_release -i)\n\r" >> myscript.txt            #Command to find OS Distributor
echo "OS Version                        \n\r>>$(lsb_release -r)\n\r" >> myscript.txt            #Command to find OS Version
echo "Kernel Version                    \n\r>>$(uname -r)\n\r" >> myscript.txt                  #Command to find Linux Kernel Version
echo "Kernel gcc version build          \n\r>>$(gcc --version)\n\r" >> myscript.txt             #Command to find gcc version used to build the linux
echo "Kernel build time                 \n\r>>$(uname -v)\n\r" >> myscript.txt                  #-v gives the kernel build time
echo "System Architecture information   \n\r>>$(uname -i)\n\r" >> myscript.txt                  #-i gives the system architecture information
echo "File System Memory                \n\r$(free -m)\n\r" >> myscript.txt                     #-m displays memory in MegaBytes
