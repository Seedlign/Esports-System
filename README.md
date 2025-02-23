# eSports Player Tracking System

## Overview
The **eSports Player Tracking System** is a C++ program designed to manage and analyze player statistics for eSports teams. It provides tools for sorting, searching, and managing player data, enabling coaches and managers to make informed decisions about player performance and team composition. This also allows supposed players to search and sort their fellow players in a tournament fashion. Another real-life application for this is the utilization for an organization to easily rank and determine the top players in a tournament.

---

## Features
- **Read Player Data**: Load player statistics from a file.
- **Display Players**: Show player data in a formatted table.
- **Sort Players**:
  - **Quick Sort**: Rank players by Kill/Death/Assist (KDA) ratio.
  - **Merge Sort**: Organize players by region and city.
- **Search Players**: Find players by username using Binary Search.
- **Add Players**: Add new players to the system.
- **Delete Players**: Remove players from the system by username.

---

## How It Works

### Key Functions
- **`ReadPlayersFromFile`**: Reads player data from a file and stores it in a vector.
- **`DisplaySortedPlayers`**: Displays player statistics in a clean, formatted table.
- **`QuickSort`**: Sorts players by KDA ratio using the Quick Sort algorithm.
- **`MergeSortByRegionAndCity`**: Sorts players by region and city using Merge Sort.
- **`BinarySearchByUsername`**: Searches for a player by username using Binary Search.
- **`AddPlayerToFile`**: Adds a new player to the file.
- **`DeletePlayerFromFile`**: Deletes a player from the file by username.
- **`DisplayMenu`**: Provides a user-friendly menu for interacting with the system.

---

## Getting Started

### Prerequisites
- A C++ compiler (e.g., GCC, Clang, or MSVC).
- A text file (`Players_200.txt`) containing player data in the following format:
