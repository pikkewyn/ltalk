#!/usr/bin/env bash
#random checks for:
#  * memory leaks
#  * crashes
#  * missing messages
#  * messed up order of messages from one client
#TODO: probably some C code would be more efficient and precise
#      for futhure testing

{
    sleep 1
    echo "open localhost 8888" 
    sleep 1 
    echo "/nick jan"
    echo "hej"
    echo "ala"
    echo "kota"
    sleep 1
} | telnet &

{
    sleep 1
    echo "open localhost 8888" 
    sleep 1 
    echo "/nick foo"
    echo "hej"
    echo "ma"
    sleep 1
} | telnet &

{
    sleep 1
    echo "open localhost 8888" 
    sleep 1 
    echo "/nick foo"
    sleep 1 
    echo "hej"
    echo "ma"
    sleep 1
} | telnet &

{
    sleep 1
    echo "open localhost 8888" 
    sleep 1 
    echo "/nick bar"
    sleep 1 
    echo "hej"
    echo "ma1"
    echo "ma2"
    echo "ma3"
    echo "ma4"
    echo "ma5"
    sleep 1
} | telnet &

{
    sleep 1
    echo "open localhost 8888" 
    sleep 1 
    echo "/nick johny"
    sleep 1 
    echo "hej"
    echo "ma6"
    echo "ma7"
    echo "ma8"
    echo "ma9"
    echo "ma10"
    sleep 1
} | telnet &

