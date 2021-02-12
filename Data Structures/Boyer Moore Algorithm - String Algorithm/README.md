# Find and Replace Application
- In this assignment, using the Boyer-Moore Horspool algorithm, we search a word in given text.
- You need to design an algorithm that searches a text finds a smaller text in it and replaces it with some other word by making the necessary changes in the text.
- Program must be written in C language.
- The specified application is "Find and Replace" used in an editor.
- The algorithm you will develop should offer the user a "Case Sensitive" option.
- The difference between small capital letters should be activated by selecting the feature. 
- If this feature is not selected, the algorithm It should search regardless of small capital letters.

## Program must have 4 parts:
### Reading Data:
- User must enter file name, a text to search and its replacement, also case sensistive option.

### Boyer-Moore Horspool Algoritması:
- The file content must be read into an array.
- Each instance of the word must be replaced in that array.
- After processing the array it must be overwriten the original file.

### Processing Time:
- Processing time must be calculated and printed to user.

### Analysis:
- You must make a analysis for different cases.(Different text size, word to search size etc.)

#### Example 1:

Find: believe

Replace: think

Option: Not case sensitive

Text: I Belive I can fly. We BELIEVE we can pass this exam.

New Text : I think I can fly. We think we can pass this exam.

Found and Replaced: 2

Running Time: xx miliseconds or xx seconds.

#### Example 2:

Find: do

Replace: think about

Option: case sensitive

Text: Just do it or I will Do it.

New Text : Just think about it or I will Do it.

Found and Replaced: 1

Running Time: xx miliseconds or xx second
