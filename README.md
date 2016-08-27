
## Multi-Stream-File-Transfer-Agent

* This is a multistream file transfer program written in cpp using socket interface.
* It was written by me in Nov 2015 as a part of mini-networks project.
* The purpose of this program is to tranfer a file in multiple fragments (all in parallel) from one local computer to another connected over internet or intranet.
* This project was successfully tested working on Ubuntu 14.04.
* For better understanding checkout [Blog on this project](http://www.manuchandel.com/2015/11/04/multistreamFileTransfer.html)

## Abstract

* File can be transfered from one computer to another over the network using socket interface.
* At the senders end file is divided into equal fragments and all fragments are sent in parallel.
* At the receivers end every received fragment is stored in a separate file and after the file transfer is complete all the fragments are merged into single file.
* All the fragments have .temp extension and can be found in hidden directory " .filetransfer ". 
* Fragments are not deleted for demonstration purpose.

## Installation

1. Make sure latest version of g++ is installed on your LINUX computer.
2. If not installed run " sudo apt-get install g++ ".
3. To transfer file make sure both computers are connected through LAN wire or to a router on a intranet or connected via internet.


To transfer a file from LINUX to another LINUX computer follow these steps <br>

1. Download all the files in this repository into one directory on your local LINUX computer. Make sure this directory has write and execute permissions.
2. Know ipV4 (or inet) address of the sender by " ifconfig " or another unix command. 
2. Open " HEADERS_AND_CONSTANTS_H.h " file and edit the HOST to sender's ip address and PORT to any pre-decided port number. POST and HOST values for both sender and receiver should be same. If you are testing it on same computer set HOST to "127.0.0.1" .
3. At sender's end compile sender file by following command g++ -o sender.out sender.cpp -std=c++11 -pthread .
4. At receivers end compile receiver file by following command g++ -o receiver.out receiver.cpp -std=c++11 -pthread .
5. At senders end pass an argument which denotes file path eg " sender.out  file.pdf " .
6. After running command at senders end run command at recivers end " receiver.out " .
7. After file tranfer is complete program will terminate automatically and transfered file will be saved with the name "new" and extension will depend on the file transfered.
8. Fragments will be in " .filetransfer/ " directory they are not deleted for demonstration purpose. you can delete them manually.


## Constants & Variables

* __PORT__              denotes port number for server and client both.
* __HOST__              denotes ipV4 address of server (sender).
* __NEW_FILE__          denotes name by which file will be downloaded at recivers end.
* __TEMP_DIR__          denotes directory where file fragments are downloaded (do not change recommended).
* __TEMP_FILE__         denotes names of file fragments sequentially (do not change recommended).
* __MAX_STREAMS__       denotes maximum parallel connections to be made to the sender to download file. Also denotes number of fragments in which file will be downloaded.
* __SECONDS_TO_WAIT__   denotes integer seconds receiver should wait before starting to receive download header for slower connections typically less than 20kB/s value should be greater than 5.
* __FILE_NAME_LENGTH__  maximum characters in file name.
* __MAX_BYTES__         maximum size of buffer used for reading bytes from sockets.

## LICENSE

##### [MIT LICENSE](https://github.com/manuchandel/Multi-Stream-File-Transfer/blob/master/LICENSE.md)
