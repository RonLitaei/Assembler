The Assembler is written in C language and its purpose is to translate an imaginary "Assembly" language into a made up 32 based number system.

The Assembler works its magic on the .as file which contains the Assembly language code.
When activating the Assembler from the terminal: 

if the Assembly file was written correctly, there are 4 possible key files that are created:
1. The main file: .ob file - this file contains the translated file: the 32 based number system file.
2. The macro file: .am file - like its name suggests, if macros were written in the Assembly file, the .am file will only contain their content, meaning, the macros will be deleted and their content will be laid out in the file.
3. .ent file - will open if entries were written in the Assembly files. Contains the entry name and its address(in the 32 based number system).
4. .ext file - the same as .ent file just for externals.
Note: the original file is left untouched.

if the Assembly file was not written correctly, there are 2 possible outcomes:
1. Warning message, meaning that the program will compile but the user should take note of the issue.
2. Error message, this is a critical error and the program cannot compile, this issue has to be fixed.

This project only handles the translation process of the assembler, the linkage part is not done here.
More information about the small details can be found at the project description file.

This is a project I wrote for one of my courses in my computer science degree (Systems programming - 20465).
Final grade - 96.
