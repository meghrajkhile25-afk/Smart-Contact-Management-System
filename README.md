# Smart Contact Management System (C++17)

A console-based minor project that manages contacts with a singly linked list.

## Features
- Add, display, search by ID/name/phone, update and delete contacts
- Alphabetical sorting by name using insertion sort
- File persistence (`data/contacts.txt`)
- Positive, unique IDs and phone-number validation

## Requirements
- GCC/G++ supporting C++17
- Run commands from the project root directory

## Build and run (Windows / VS Code terminal)
```powershell
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o contact.exe
.\contact.exe
```

## Build and run (Linux/macOS)
```bash
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp -o contact
./contact
```

## Data structure and algorithms
- Singly linked list: contact storage; add at head O(1), display/search/update/delete O(n)
- Linear search: O(n)
- Insertion sort by name: O(n^2) worst case
- File load/save: O(n) contacts

## Notes
Run from the project root so the relative path `data/contacts.txt` works. The app saves on exit and with option 7.
