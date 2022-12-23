# Servidor-RPC Multithreaded
Tested in Ubuntu 16

Steps to reproduce it:

1) Clone the repository;
2) Open a terminal window in the project directory
3) Install rpc bind with command 'sudo apt install rpcbind'
4) Run command 'make'
5) Run './serverOp'
6) Open another terminal window and run './clientOp <ip_addr>'

You can run step 6 as many times as you'd wish as long as the server is up.
