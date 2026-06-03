# LRU Cache Manager
A command-line LRU (Least Recently Used) Cache simulator built in C++ from scratch.

## What it does
Simulates a fixed-capacity cache that evicts the least recently used item 
when full. Supports SET, GET, and EXIT commands via an interactive CLI.

## Technical highlights
- Doubly linked list with manual pointer manipulation
- Fixed-size lookup array (no STL containers used)
- Manual memory management with new/delete (zero memory leaks)
- Built without std::list, std::map, or unordered_map

## How to run
g++ lru_cache.cpp -o lru_cache
./lru_cache

## Example
Enter maximum cache capacity: 3
SET A 10  →  [Head] -> (A: 10) -> [Tail]
SET B 20  →  [Head] -> (B: 20) <-> (A: 10) -> [Tail]
GET A     →  Cache Hit! Value: 10
SET D 40  →  Eviction Warning: Evicting LRU Key (B).
GET B     →  Cache Miss! ERR: Key 'B' is not present in cache.