# Syntactical-Analysis

## Grammar Rules
https://github.com/vain3219/Syntactical-Analysis/blob/master/grammars.pdf


__________________________________________________________________________________________________________________

## IMPORTANT FOR RUNNING THE PROGRAM
  * Change the file path for srcFile in main.cpp to reflect the location of myFile.txt on **your** system
  
  
## FIRST(α)
 * First(S)  = { type, i }
 * First(D)  = { type }
 * First(A)  = { i }
 * First(E)  = { (, i, num }
 * First(E') = { +, -, ε }
 * First(T)  = First(E)  = { (, i, num }
 * First(T') = { * , /, ε }
 * First(F)  = First(E)  = { (, i, num }


## FOLLOW(α)
 * Follow(S)  = { $ }
 * Follow(D)  = { $ }
 * Follow(A)  = { $ }
 * Follow(E)  = { ), $ }
 * Follow(E') = { (, $ }
 * Follow(T)  = { +, -, ), $ }
 * Follow(T') = { +, -, ), $ }
 * Follow(F)  = { * , /, +, -, ), $ }
