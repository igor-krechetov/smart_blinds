#!/bin/bash

IP="wilfred.lair"
SUBJECT_CA="/C=KR/ST=Suwon/L=Suwon/O=Home_IoT/OU=CA/CN=iotroot"
SUBJECT_SERVER="/CN=$IP"
SUBJECT_CLIENT="/CN=localhost"

function generate_CA () {
   echo "$2"
   openssl req -x509 -nodes -sha256 -newkey rsa:2048 -subj "$2"  -days 7305 -keyout ./$1.key -out ./$1.crt
   openssl x509 -in ./$1.crt -out ./$1.pem
   openssl rsa -in ./$1.key -text > ./$1_key.pem
}

function generate_server () {
   echo "$SUBJECT_SERVER"
   openssl req -nodes -sha256 -new -subj "$SUBJECT_SERVER" -keyout ./server.key -out ./server.csr
   openssl x509 -req -sha256 -in ./server.csr -CA ./ca.crt -CAkey ./ca.key -CAcreateserial -out ./server.crt -days 7305
   openssl x509 -in ./server.crt -out ./server.pem
   openssl rsa -in ./server.key -text > ./server_key.pem
}

function generate_client () {
   echo "$3"
   openssl req -new -nodes -sha256 -subj "$3" -out ./$1.csr -keyout ./$1.key 
   openssl x509 -req -sha256 -in ./$1.csr -CA ./$2.crt -CAkey ./$2.key -CAcreateserial -out ./$1.crt -days 7305 -addtrust clientAuth
   openssl x509 -in ./$1.crt -out ./$1.pem
   openssl rsa -in ./$1.key -text > ./$1_key.pem
}

cd ./certs
rm -v ./*
generate_CA ca $SUBJECT_CA
generate_server
generate_client client ca "/CN=smartblinds"
generate_client client_ha ca "/CN=homeassistant"

openssl x509 -in ./server.crt -text -noout
