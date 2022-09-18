# Concordance
Info: Repository which contains source code of the concordance challenge
Fire-Up: In order to create a CMake solution, run 'python boot.py' 
Structure: Concordance project consists of the following structure:

-Concordance
 |
 |_exec
 |     |_main.cpp
 |     
 |_src
 |     |_<CONCORDANCE_SOURCES>
 |     |_include
 |              |_<CONCORDANCE_HEADERS>
 | 
 |_test
 |     |_<UNIT_TESTS(gtest)>

 src subdirectory creates a 'concordance library' to be used both by 'exec' executable and unit testings
 Some auxiliary files are also included in this repo (.gitignore and boot.py)

 #Specifications
 -> Application should run by providing a -f command line argument and a text file that the concordance will be generated from
 -> Result will be a console print of the generated concordance, meaning
       a. The index of the word
       b. The word in lower case, appearing sorted alphabetically
       c. The number of the sentences each word is found.

#Remarks
1. In order for a word to be accepted in concordance, it must be an english word. Any words that contain special symbols like @#$%%^^&&* will not be
   accepted in the concordance and will be dropped during parsing. Special handling is done for abbreviations and acronyms, like a.k.a and Back2U.
   The above will be considered a word and will be included in the concordance
2. A sentence is defined as a series of words, terminated with one of the following characters: 
   - a period mark ('.')
   - a question mark ('?')
   - an exclamation mark ('!')
   - a greek question mark (';')  <<< (for this one it is not always considered to change a sentence, but in this example I made the assumption that it does)
3. A change of sentence is done when
   - a sentence termination character (of the above) is found
   - next word starts with a capital letter
   This method was chosen since it typo errors like 'this sentence terminates here.A new sentence started withoout space' are typical
4. Hyphenation is not currently handled in this first iteration. Any word that terminates with - and continues in next line will be considered as two words
5. A word is not valid if it does not contain at least one alphabetical character. AK47 is considered to be a valid word but 210628569 is not
6. E-mails like mymail@outlook.com will not be considered as 2 seperate valid words and will be dropped
7. File paths like %USER%/my/path will not be splitted to words. They are considered invalid words due to the above rules and will be dropped
8. Character "'" receives special handling and is allowed to participate in words, due to the possession role that a word gets with it. 
   Angelo's car is assumed to contain two valid words: angelo's & car
9. An abbreviation is defined as a word which contains a mixture of letters and dots, where the next letter after a dot is lowercase. 
   a.k.a is considered an abbreviation, while A.K.A is not for the moment, since it would violate rule #3.