## UID: 206111331

## Pipe Up

Kernel C program to mimic the bash pipe operator by manipulating file descriptors. 

## Building

Use the make command to build the program.
```bash
make 
```

## Running

Run the program by calling the executable along with a sequence of at least 1-2 bash commands.
```bash
./pipe who sort uniq
```

The abobe command will mimic the behavior of the typical bash pipes:
```bash
who | sort | uniq
```

## Cleaning up

Clean up all binary files by executing the make clean command.
```bash
make clean
```
