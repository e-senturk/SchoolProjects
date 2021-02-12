# Spell Checker with Dynamic Programing
- Design an algorithm for correcting misspelled words by using Levenshtein Edit Distance

## Process Steps:
- To prepare the dictionary to be used in the homework, the use smalldictionary.txt file
for dictionary and put the words in the dictionary table (hash table). This process will be done once.
- Search each word in the given sentence in the dictionary table. If the word is in the dictionary table
a. word is correct if there is one. The process is complete for that word.
b. otherwise it is searched in the incorrect word table
- If word is in the incorrect table, correct word recommended and accepted by the user is suggested to the user.
- If word isn't in the incorrect table, calculate Levenshtein Edit Distance with dictionary words 
- if there are any word with distance 1 print them to the user as a recommendation.
- otherwise if there are any words with distance 2 print them as a recommendation. 
- ignore 3 or higher distances.
- You must use openaddress and double hashing method for that problem.

## Bonus:
- Dont calculate Levenshtein Edit Distance to the end stop calculation if distance is greater than 2.

## Rules:
- Program must be implemented in C language
- Put screenshots and algoritm information into the report file.