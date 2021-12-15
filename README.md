# Passcrack
Created by Ryan Murphy

Passcrack is a brute-force password cracking program written in C.

Usage:
      ./passcrack <password file> [output file] [flag(s)]
  <password file> : The text file containing passwords which the programs guesses will be compared to.
  
  [output file] : File for results to be written to. A default, "pcout.txt", is used if this argument is not provided.
  
  [flag(s)] : Optional flags, described below.
    
  FLAGS:
    -p : Prints output to command line (NOTE: Program will still print to output file as well).
