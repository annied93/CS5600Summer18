import os

os.system("clang tcpserver.c -o tcpserver")
os.system("clang tcpclient.c -o tcpclient")
os.system("clang http_server.c -o http_server")
os.system("clang http_client.c -o http_client")

