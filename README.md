# 2048-AI

AI for 2048 game.

Based on a fast bitboard, it can make around 10.000.000 moves per second on my CPU (Intel i3 M370 2.4 GHz).

The AI algorithm is very simple and inspired by the [Monte-Carlo approach](https://en.wikipedia.org/wiki/Monte_Carlo_tree_search#Pure_Monte_Carlo_game_search).

### Compiling

A makefile is provided to compile the demo. It require gcc or clang (which is better).

Also, please note that compiling in 64 bits is recommended.

### Usage

To run the demo  :

`./2048 [nruns]`

(`nruns` is the number of playouts per move, by default `nruns` is set to 500.)
