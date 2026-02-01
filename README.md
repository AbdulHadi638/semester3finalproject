# Graphing Calculator - C++ OOP Project

## Project Overview
This is a console-based graphing calculator developed in C++ using object-oriented programming concepts. 
It allows users to plot linear, quadratic, and exponential functions, view function history, and save/load functions to files. 
The project demonstrates abstraction, inheritance, polymorphism, and file handling using C++ classes and linked lists.

## Features
- Plot linear functions (y = mx + c)
- Plot quadratic functions (y = ax^2 + bx + c)
- Plot exponential functions (y = A*e^(Bx))
- Console-based graph plotting with ASCII characters
- Function history using linked list
- Save and load functions to/from text files
- Range adjustment for graphs
- Simple menu-driven interface

## Technologies Used
- C++
- Standard libraries: `<iostream>`, `<cmath>`, `<fstream>`, `<sstream>`, `<iomanip>`, `<vector>`, `<string>`
- File handling for saving functions and graph data
- Console-based graphics

## Object-Oriented Concepts Used
- **Abstraction**: `Expression` abstract base class
- **Inheritance**: `LinearFunction`, `QuadraticFunction`, `ExponentialFunction` derived from `Expression`
- **Polymorphism**: Virtual functions for evaluation and display
- **Linked List**: `FunctionHistory` stores the history of functions
- **Encapsulation**: Data members are protected/private and accessed via functions

## Usage
1. Compile the project in a C++ compiler or Visual Studio.
2. Run the program. A menu will appear with options to plot functions, view history, load saved functions, or exit.
3. Enter coefficients for the chosen function type to plot the graph.
4. Optionally save the function and graph data to files.

## Purpose of the Project
This project is designed as an academic assignment to practice:
- C++ programming and object-oriented concepts
- Console-based interface design
- File handling and data persistence
- Graphical representation using ASCII characters
- Team collaboration (if developed with multiple contributors)

## Future Improvements
- Add more function types (e.g., sine, cosine)
- Improve ASCII graph visuals
- Implement zoom and pan functionality
- Cross-platform compatibility
