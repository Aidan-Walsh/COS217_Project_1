# Decommenter
This C code decomments your code, in exactly the same way as a preprocessor! As it runs, it takes text from stdin and outputs the text decommented to stdout. 

# About
When you run C code through a compiler, it goes through several stages: preprocessing, compiling, assembling, and linking. The first stage, preprocessing, 
involves removing comments in the code so that it may be eventually turned into machine readable language. A comment consists of a "/\*" at the beginning and a "\*/" at the end. There must be both of these for it to be a comment (if there is a "/\*" but no "\*/" then there will be an error). All text within the "/\*" and "\*/" is part of the comment and so it is also removed by the preprocessor. 

The only file of use here is decomment.c. All other files are used for testing and were provided by the Princeton University Computer Science Department. 

# To use
1. Download "decomment.c"
2. Compile with the command
    - "gcc decomment.c -o decomment"
3. To run a text file, "text.txt", through the program and save the output to a new text file, "new.txt", do the following: 
    - "./decomment < text.txt > new.txt"
4. If you run into an issue where permission is denied, run this command, then try step 3 again: 
    - "chmod +x decomment"
    - Note that if you are running Linux, then you may have to add "sudo" to the beginning of the command to gain super-user privileges. 

You now have a decommenter!
    

