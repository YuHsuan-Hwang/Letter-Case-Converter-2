g++ -L/usr/local/opt/openssl/lib -I/usr/local/opt/openssl/include -std=c++11 -O3 server.cpp -Isrc -o server.out -lpthread -L. -luWS -lssl -lcrypto -lz -luv
