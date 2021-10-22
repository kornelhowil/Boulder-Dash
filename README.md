## Boulder-Dash
Simple version of classic Boulder Dash written in C

## How to use
#Compilation
Compile with 
``` sh
gcc -std=c17 -DTUI -lncurses boulder_dash.c -o boulder_dash
```
And run with
``` sh
./boulder_dash
```
First you need to input to ints n and m. These are the dinmensions of the board.
Then you need to input a map. 

# Meaning of characters
'@' - Rockford
' ' - empty space
'+' - dirt
'#' - bedrock
'O' - rock
'$' - diamond
'X' - exit

An example of input is in test.txt.
If you want to play on the map from a txt file you need to run with
And run with
``` sh
cat test.txt - | ./boulder_dash 
```
