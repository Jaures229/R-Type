#!/bin/bash


./build/r-type_server 127.0.0.1 8080 > log.txt

./build/r-type_client 127.0.0.1 8080

./build/r-type_client 127.0.0.1 8080

./build/r-type_client 127.0.0.1 8080
