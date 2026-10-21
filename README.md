# Quiz-Core

**A CLI quizing application for both students and teachers**

#The program is at a very alpha stage of it's life. Only the MCQ engine part is done. Other question types ike fill in the blanks, Short answer and Mathematical question will be added very soon. 

#Login system for every student, tecaher and admin will be added and they will have their separate sections

**How to use**

1. Enter filepath of original question (.txt formats only) and then enter the filepath of the ecnrypted version of the original file(.dat format is recommended).

2. Now you will type and Enter Name, Roll and Subject.

3. Questions will be loaded and you have to type the correct option(A/B/C/D).

4. Result will be shown after the quiz finishes and will be encrypted and stored in progress.dat.

**How to create Question text files**

Follow this format:

    [Question]
    [Option A]
    [Option B]
    [Option C]
    [Option D]
    [Answer option (A/B/C/D)]


Example :

    In a simple graph, what is the maximum number of edges between 5 vertices?
    A) 5
    B) 10
    C) 15
    D) 20
    B
    What is the next term in the arithmetic sequence 3, 7, 11, 15, ...?
    A) 17
    B) 18
    C) 19
    D) 20
    C

**#Any missing component in an individual question will cause undefined behaviour**
