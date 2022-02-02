# wordle_helper

Simple C++ program to help with wordle.
Word list must contain only five letter words. Five letter words and six guesses is hard coded into this shit. I'm not making it more general

## Usage

### Ubuntu 20.04 (Any Ubuntu? Debian based? Any Linux? idfk, go nuts)

```
$> wget https://github.com/mbremyk/wordle_helper/raw/main/wordle_helper
$> wget https://github.com/mbremyk/wordle_helper/raw/main/words    # Or provide your own words file
$> ./wordle_helper
```

### Linux if previous didn't work

Install your favourite c++ compiler

#### Ubuntu(/Debian? Anything that uses apt really):

```
$> sudo apt install g++
```
If you're missing any packages your package manager will tell you, probably. They're getting pretty smart these days

#### Others:

idk, something about pacman or snap or whatever. You probably know what you're doing

```
$> wget https://github.com/mbremyk/wordle_helper/raw/main/main.cpp
$> wget https://github.com/mbremyk/wordle_helper/raw/main/words    # Or provide your own words file
$> g++ main.cpp
$> ./a.out
```

### Windows

Download [wordle_helper.exe](wordle_helper.exe)  
Download [the list of words](words) or provide your own list of words  
Double-click wordle_helper.exe