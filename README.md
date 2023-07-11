# C Linked List Dictionary Challenge
My take on a programmers challenge to write a program in C that takes in a sentence and creates a dictionary of words out of it using a linked list. This challenge demonstrates user input, static data structure, creation and usage, and output to console.

## Specification
The following rules were given for the structure and functionality of the code:
1. The program receivs a sentence from the user and, for each word, stores the word length, the first four letters of the word, and a pointer to the next entry
2. Dynamic memory allocation routines such as ```malloc``` and ```free``` cannot be used; global memory is to be used instead
3. ```strtok``` cannot be used to parse words from the sentence
4. The program will sort the list alphabetically by reassigning pointer links
5. The user will be able to look up a word and the program will return the entry that matches
6. The user can add new entries to the current linked list, which get sorted alphabetically
7. The linked list can be deleted
8. Searching for a word supports case sensitivity

## Summary
The linked list supports nodes (```word```) with three parameters: an integer for the ```length``` of the word, a string containing the first four ```letters```, and a pointer to the ```next``` node in the linked list. As dynamic memory routines are not allowed, I instead used an array called ```wordPool``` in order to store all the nodes that would be used. A maximum cap of 100 nodes can be stored in this pool. To help keep track of the pool, the ```head``` integer is used to determine the index for the head of the linked list, and the ```wordCount``` integer counts how many word nodes have been created.

### main
When the program starts, the user is first prompted to enter a sentence. Upon entering it, the program passes the sentence into the ```parse``` function and gives the user five choices that can be selected with letters: enter a new sentence, search the list, print the list, delete the list, or exit the program. There is also a failsafe to catch invalid choices.

Whenever ```scanf``` is used to read inputs from the user, a while loop is used to consume the newline character from previous input. This prevents unwanted text from being output to the console.

### parse
When a new sentence is entered when starting the program or after selecting the option to do so, the ```parse``` function is the first to run. It takes the sentence as a parameter and iterates through each character, using alphanumeric and non-alphanumeric characters to tell when multiple words are separated. Using a for loop, the length and first four letters of each word are parsed and passed into the ```addWord``` function. As a failsafe, It first checks if the maximum capacity of the linked list has been reached, and if so, it breaks the for loop. Outside the for loop, ```bubbleSort``` is called.

### addWord
The ```addWord``` function takes the data of an inputted word and turns it into a node for the linked list. The first open entry of ```wordPool```, determined by ```wordCount```, is used to store the word and its data, and the node is set to the end of the linked list with its ```next``` pointer linking to the next empty slot in the pool. At the end of the function, ```wordCount``` is incremented by one.

### bubbleSort
The ```bubbleSort``` function is used to sort the linked list alphabetically using a simple bubble sorting algorithm. It first checks if there are less than two entries in the linked list, in which case there is no sorting required, and exits the function if true. Three pointers are then created, with ```headptr``` pointing to the head of the linked list, ```current``` being iterated on, and ```prev``` being for the previous node if required.

In a nested for loop, the function iterates through the linked list from head to tail as many times as there are nodes, with each run through passing over one fewer node. It checks the current node for whether it should be sorted after the node directly after it, and if so, it switches their positions and appropriately redirects their ```next``` pointers.

Two additional helper functions are used for conditionals in this function. If the node being put forward is currently the head of the linked list, then the ```changeHead``` function is used to locate the head's position in ```wordPool``` and change its index accordingly. If it is not the head, then the previous node also needs its ```next``` pointer changed, so the ```getPreviousWord``` function is used to locate and return said node.

### search
When a user searches for a word in the dictionary, then the ```search``` function is called with the option to make the search case-sensitive. This function first checks if the list is empty and, if so, displays this to the user and exits the function. Much of the code here is reused from ```parse```, but because only one word is being read, code relating to separating words from the sentence is removed. Once the first four letters and length of the word are found, they are compared to every entry in the linked list using ```strcmp``` or, if not case-senstive, ```strcasecmp```. If a match is found, then the first four letters and length are displayed to the user. If not, then "No match found" displays instead.

### printWordList
When a user attempts to print the dictionary, the ```printWordList``` function is called. This simple function first checks if the list is empty and, if so, displays this to the user and exits the function. Otherwise, the function iterates through the linked list from head to tail and prints the first four letters and length of each word.

### deleteWordList
When a user attempts to delete the dictionary, the ```deleteWordList``` function is called. This simple function first checks if the list is empty and, if so, displays this to the user and exits the function. Otherwise, the function iterates through the linked list from head to tail and sets data to null values. It then resets the ```wordcount``` and ```head``` integers back to 0.
