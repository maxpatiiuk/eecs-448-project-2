# Documentation

This is a general documentation for EECS 448 Battleship game.

This guide contains information on getting the development and production server
up, as well as information about tools used in the process of development and
works cited.

## Time Estimate

[Time estimate](time-estimate.md) and
[actual accounting of time taken](time-accounting.md).

## Components

User interaction

- Setup (board)
- Choice (gameplay)
- View boards (view attacks) (view my board) - Quit

Board

- Grid
- Number of ships
- Ship placement
- View board (view attacks) (view my board)

Ship

- Size
- Placement
- Been hit
- Coordinates (array)
- Orientation (horizontal or vertical)

## Development

Clone this repository

```zsh
git clone https://github.com/maxxxxxdlp/eecs-448-project-2
```

A makefile must be used to make the program.

```zsh
# Navigate to the source directory
cd eecs-448-project-1

# Build the program
make

# Run the executable
./Battleship

# Clean up
make clean
```

## Documentation Generation

```zsh
cd ./documentation

doxygen ./documentation/Doxyfile
```

Documentation will then be found in the `./documentation/docs-gen` directory.

## Works Cited

- Bloomfield, Aaron. “PDR: Doxygen Tutorial.” PDR: Doxygen Tutorial,
  www.aaronbloomfield.github.io/pdr/tutorials/11-doxygen/index.html.
- “Fine-Tuning the Output.” Doxygen Manual: Doxygen Usage,
  www.doxygen.nl/manual/doxygen_usage.html.

## Tech stack

- C++
- Makefile

## Tools Used

Tools used in the process of development

- Git
- pre-commit.com
- GitHub
- Vim
- PyCharm
- Visual Studio CODE

## Licence

This code is available under MIT Licence
