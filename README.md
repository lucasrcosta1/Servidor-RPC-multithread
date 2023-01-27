# Multithreaded RPC Server
Tested in Ubuntu 16

Steps to reproduce it:

1)  Install rpc bind with command 'sudo apt install rpcbind'
2)  Clone the repository
3)  Open a terminal window in the project directory
4)  Change to rpc_multithread directory with command 'cd rpc_multithread'
5)  if (Client and server are at the same node) {
        Run command 'make all'
        Run './server'
    } else if(!Client and server are at the same node) {
        On server node: 
            Run command 'make server'
            Run './server'
        On client node:
            Run command 'make client'
            Go to step 6
    }

6)  Open a new terminal window and run './client <ip_addr>'

You can run step 7 as many times as you'd wish as long as the server is up.
