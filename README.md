A Go A.I. based on *MCTS* (Monte Carlo Tree Search).

It shows the beginner level on 9x9 board currently, for it does not use neural networks, but only simple *UCT* (*UCB* for Tree) algorithm and monte carlo simulation.

The underlying data structure and algorithms are quite efficient. Specifically, it runs 40 thousands monte carlo game per second on my MacBook Air 2014.

The key ideas of the Go game's implementation are as follows：
* We implement a string of connected pieces using the disjoint set, allowing to efficiently update piece strings during the game process.
* We use bit operations to efficiently update liberty point number of piece strings. For example, we use *or* when merging several piece strings into a single one.
* For tree search, we do not really implement a tree structure, but use a hash table to keep game states. Especially, we use *Zobrist Hashing* to incrementally compute the hash value of a game state.

This project is highly readable and extensible, due to its well-designed OOP design. Some important classes are listed as follows:

* **FullBoard** A game board class providing efficient methods such as *PlayMove* to update the game state.
* **Player** An interface providing inheritable methods such as *NextMove*. For example, *RandomPlayer*, a subclass of *Player*, will return a random legal position of the next move when being called its *NextMove*.
* **Game** An interface containing two *Player* objects. For example, *MonteCarloGame* contains two *RandomPlayer* objects.

## Make
* boost is required
* mkdir build && cd build
* cmake ..
* make

## Contact
* email : chncwang@gmail.com
* QQ Group：823648893
