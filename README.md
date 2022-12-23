# Servidor-RPC Multithreaded
Tested in Ubuntu 16

Steps to reproduce it:

1) Install rpc bind with command 'sudo apt install rpcbind'
2) Clone the repository
3) Open a terminal window in the project directory
4) Change to operation_multithread directory with command 'cd operation_multithread'
5) Run command 'make all'
6) Run './Server'
7) Open another terminal window and run './Client <ip_addr>'

You can run step 7 as many times as you'd wish as long as the server is up.
