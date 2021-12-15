# Passcrack
Created by Ryan Murphy

Passcrack is a brute-force password cracking program written in C.

Usage:
      ./passcrack <password file> [flag(s)]
  <password file> : The text file containing passwords which the programs guesses will be compared to.

  [flag(s)] : Optional flags, described below.

  FLAGS:
    -p : Prints output to command line (NOTE: Program will still print to output file as well).
    -o <output file> : Prints output to <output file>. A default pcout.txt is provided if this flag is not used.
    -s <start length> : Initial password length to check is <start length>.
    -e <end length> : Program will cease checking if passwords being checked reach <end length>.
