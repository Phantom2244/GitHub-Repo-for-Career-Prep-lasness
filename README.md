# Adventurers-Vs.-Dragon-Version-1

This is a C++ program with an interactive `main.cpp` that simulates
a text-based adventure combat. The goal is for the user's party to beat the
Dragon. The user's party is provided by the simulators `main.cpp`, and contains
one of each of the Adventurer characters `Rogue.h` , `Knight.h` , and `Mage.h` .
The user then battles a Dragon in a turn-based combat system that is managed
by `main.cpp`.

My project demonstrates object-oriented design principles that include inheritance,
polymorphism and encapsulation. The main program is interactive, validates
user input and guides the player through Adventurer combat actions against a Dragon,
including selecting other party members to effect with heals or revives.

My project started with C++, but I eventually added Python. I use Python in this program to take in the information of
which character's turn it is, and display an image of that character during their turn. Python is good at remembering
things efficiently on its own, without the program having to explain how to find something. Python can go find it on
its own pretty easily, as it is intuitively structured. Third Party Libraries in Python are thoroughly built out and
make it simpler to visualize and develop user interface.

In the C++ main, which controls the whole game, I utilize the system command line to load the Python file, so the
Python file is always on in the background. As different turns happen and are tracked in main, the information of which
turn is happening is then written to a text file. The always on Python script then reads and recognizes that the
characters' turn changed in the text file. Then the Python file utilizes openCV to render and show teh adventurer whose
turn it is.

An installations necessary to run this program is openCV.