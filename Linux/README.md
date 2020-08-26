### Linux side server program

#### Build

The program uses `xdo` to emulate keyboard events.

To build it you need xdo libraries.

Run `gcc server.c -lxdo -o server.o`

#### Notes

The key mapping configurations and the server port is hard-coded in the source code.

