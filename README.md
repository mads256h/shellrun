# shellrun

 Runs binary shell code. Also includes debugging and compatibillity features.

 The goal of this project is to create a simple portable shellcode-running program.

## Building

 Run cmake on this repository to generate a project for your compiler. **Remember you cannot mix bitness e.g. x32 shellcode on x64 shellrun** To run 32-bit shellcode build shellrun as 32-bit and vice versa.

## Running

 ```shellrun [args] file```

 Runs the shellcode located in the file specified by the file argument. Shellrun supports clearing of registers and inserting a breakpoint before the shellcode is run, by specifying the -c and the -d argument respectivly.

 Currently supported arguments:

```txt
 -c OR --clear   Clear all registers before running the shellcode.
 -d OR --debug   Set a breakpoint just before shellcode executes.
 -h OR --help    To print this help message.
```

 Shellrun also supports running shellcode from stdin:

 ```cat shellcode | shellrun -```

## Building the shellcode examples

 First make sure that you have [nasm](https://www.nasm.us) installed and that it is in your global path. _This is easiest with [scoop](https://scoop.sh)_. Then simply for Windows run ```build.bat``` and for Linux run ```build.sh``` (does not exist yet).
