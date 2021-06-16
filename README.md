FoolGo is a Go A.I. based on *MCTS* (Monte Carlo Tree Search).

It shows the beginner level on the 9x9 board currently, for it does not use neural networks, but only the simple *UCT* (*UCB* for Tree) algorithm and Monte Carlo simulation.

The underlying data structure and algorithms are quite efficient. Specifically, it runs 40 thousand Monte Carlo games per second on my MacBook Air 2014.

The key ideas of the Go game's implementation are as follows：
* We implement a string of connected pieces using the disjoint set to update piece strings during the game process efficiently.
* We use bit operations to update the liberty point number of piece strings efficiently. For example, we use or when merging several piece strings into a single one.
* We do not implement a tree structure for tree search, but use a hash table to keep game states. Especially, we use Zobrist Hashing to compute the hash value of a game state incrementally.

This project is highly readable and extensible due to its well-designed OOP design. Some important classes are listed as follows:

* **FullBoard** A game board class providing efficient methods such as *PlayMove* to update the game state.
* **Player** An interface providing inheritable methods such as *NextMove*. For example, *RandomPlayer*, a *Player* subclass, will return a random legal position of the next move when called its *NextMove*, while *UCTPlayer* performs the UCT algorithm to find the next move.
* **Game** An interface containing two *Player* objects. For example, *MonteCarloGame* contains two *RandomPlayer* objects.

## Make
* boost is required
* mkdir build && cd build
* cmake ..
* make

## Contact
* email : chncwang@gmail.com
* QQ Group：823648893
