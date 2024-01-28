# Description
## What it is:
This is a simple simulation of the IPFS distributed file system coded in C++ as a project of Data Structures course in FAST NUCES 3rd semester.
It is a distributed hash table system using the excellent sha1 hash function.

## How it works:
The type of distributed system is the Ring Distributed Hash Table System with machines placed in a ring topology. Every machine has a unique id assigned to it which helps determine which file it will hold.
internally the files in the machines are stored and indexed with a `B-Tree` which holds the hash value of the file together with its path in the system.

# How to run
All the code is in all these files and the `main.cpp` file has the driver code. Copy all the files in visual studio and execute the `main.cpp` file it will automatically link the other header files.

## License
This project is open source and can be modified and distributed anywhere.
