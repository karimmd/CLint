# CLint
Static Analysis Checking Application for C and C++

A new system named CLINT has been designed and implemented as a part of my Graduate Final Project, which can be used to detect security vulnerabilities like buffer overflow, memory leak on the source code of the C and C++ programming language. The results from some several experiments have showed that this system can find possible security vulnerabilities in the software and also can establish the capability and the effectiveness to help the developers creating software with less security flaws in the upcoming days. 


The principle contents of this research are as follows:

(1)	Based on the background of software security testing, analyzed on the current problems of the buffer overflow vulnerability testing and to identify the research requirements.

(2)	Analyzed on the key concepts such as memory allocation knowledge on the stack and heap area and also the overflow detection methods like static analysis technologies and related systems which are available at present.  

(3)	Designed a vulnerability detection prototype on Windows platform, discussed the design objective, overall design method, core function design and the detail design of the system to understand and realize the class functionality and the main detection methods of the system. 

(4)	Test the system in different units and overall the experiment results that can show the design and implementation are effective, stable and robust.


For CLINT system there is no difference but when it came to detect the vulnerabilities, two major software security flaws have been centralized such as buffer overflow and memory leak. After a wide range of research works it was possible to generate a general algorithm program for the main system and then two different algorithm based on the two different vulnerabilities have been put together. Those algorithms will be described later in this chapter.
  
The analyzer comes with many different checkers. These checkers are categorized by their severity. Program options are available for enabling or disabling checks by their severities. These severities are

(1)	error for more severe issues like syntax errors

(2)	warning for suggestions about possible problems

(3)	style for dead code and other stylistic issues

(4)	performance for some common performance related suggestions

(5)	general platform portability issues and

(6)	information for informational messages about problems with the analysis

  
This happens when a user executes CLINT system from the command line:

(1)	CLintExecutor::check this function executes the Clint	

(2)	Clint::parseFromArgs parse command line arguments

     1)	The Settings class is used to maintain settings
     2)	Use FileLister and command line arguments to get files to check

(3)	ThreadExecutor create more instances of Clint if needed

(4)	CLint::check is called for each file. It checks a single file

(5)	Preprocess the file (through Preprocessor)
    
    1)	Comments are removed
    2)	Macros are expanded

(6)	Tokenize the file (see Tokenizer)

(7)	Run the runChecks of all check classes.

(8)	Simplify the tokenlist (Tokenizer::simplifyTokenList)

(9)	Run the runSimplifiedChecks of all check classes
  
When errors are found, they are reported back to the CLintExecutor through the ErrorLogger interface
