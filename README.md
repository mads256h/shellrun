# shellrun

 Runs binary shell code. Also includes debugging and compatibillity features.

## Building

 Run cmake on this repository to generate a project for your compiler. **Remember you cannot mix bitness e.g. x32 shellcode on x64 shellrun** To run 32-bit shellcode build shellrun as 32-bit and vice versa.

## Running

 ```shellrun [args] file```

 Runs the shellcode located in the file specified by the file argument. Shellrun supports clearing of registers and inserting a breakpoint before the shellcode is run, by specifying the -c and the -d argument respectivly.

 Currently supported arguments:

```bash
 -c OR --clear   Clear all registers before running the shellcode.
 -d OR --debug   Set a breakpoint just before shellcode executes.
 -h OR --help    To print this help message.
```

 Shellrun also supports running shellcode from stdin:

 ```cat shellcode | shellrun -```
