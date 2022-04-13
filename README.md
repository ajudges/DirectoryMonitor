# CAPSTONE PROJECT
Manages the size/number of content of specified system directories. If a directory is above a particular size/content [configurable, known as `control policy`] , execute a `deletion policy`.

This project is motivated by the constrained space resource on embedded systems, whereby files are written to certain directories, but the directories may not be expected to exceed a certain size or number of files. 

## Dependencies
### C++17
Make sure your C++ compiler supports C++17:
Ubuntu + GCC
```bash
g++ --version
```
Output should be something like
```console
g++-8 (Ubuntu 8.4.0-1ubuntu1~18.04) 8.4.0
```

Mac Os + Clang
``` bash
clang --version
```
Output should be something like
```console
Apple clang version 11.0.0 (clang-1100.0.33.8)
```

Windows + MSVC
```
!!! warning ""
    * Visit the [Visual Studio](https://visualstudio.microsoft.com) website
    * Download Git from [https://git-scm.com/download/win](https://git-scm.com/download/win) and install it
```


## Run the Project
Pre-requisite
1. Create one or more test folders 

### Run
1. Clone repo
2. Navigate to the root directory of the project
3. Edit ./directoryConf in a text editor
	See template in section _Sample of configuration file_ below
	OR
	Put absolute paths of test folder(s) created in the pre-requisite
4. Create a folder (`build`) in the root directory
`mkdir ./build`
5. Navigate to the `build` folder and build the project
`cd ./build && cmake .. && make`
6. Run the program
`./DirectoryMonitor`
7. Copy files/folders to the test folders to breach the `controlPolicyType` in `./directoryConf`

## Workflow
Reads `dirPath`, `controlPolicyType` (`maxSizeInMb`/`maxNumOfContent`) `deletePolicyType` from a configuration file -- directoryConf.
Check if directory exist.
If directory does not exist, indicate directory does not exist and don't monitor
If directory exists, instantiate class `DirControlPolicy` to hold the control policy object and class `DeletePolicy` to hold the delete policy object, and start monitoring loop.

### Monitoring loop
Gets `control policy` (`max_size_in_mb` or `max_num_of_content`)
Checks if directory is obeying `monitoring policy`
If deviating from `monitoring policy`
	- enforce `deletion policy`
	- output a policy breached message

### Sample of configuration file -- directoryConf
The file directoryConf is a `list[json]`. To add more directories, just add an additional json. See examples below
#### Sample Conf One directory

The sample below has a monitoring configuration for a directory. 
directory1 has a control policy of maximum number of content - 2. It has a delete policy to delete all files when number of content > 2.

```json
[
    {
        "dirPath": "/absolute/path/to/directory1",
        "controlPolicyType": "maxNumOfContent: 2",
        "deletePolicyType": "ALL"
    }
]
```

#### Sample Conf with Two directories
This adds an additional directory (directory2) to monitor. directory2 has a control policy max size - 3 MB. It has a delete policy to delete all but file1.txt and file2.txt;
```json
[
	{
		"dirPath": "/absolute/path/to/directory1",
		"controlPolicyType": "maxNumOfContent: 2",
		"deletePolicyType": "ALL"
	},
	{
		"dirPath": "/absolute/path/to/directory2",
		"controlPolicyType": "maxSizeInMb: 3",
		"deletePolicyType": "exclude: file1.txt, file2.txt"
	}
]
```
## Rubric points addressed
### Loops, Functions, I/O
- The project demonstrates an understanding of C++ functions and control structures: The program makes use of a variety of control structures and is organized into namespaces, and functions.

- The project reads data from a file: The program reads configurations from `directoryConf` file -- located in the root directory. 

### Object Oriented Programming
- Object Oriented Programming Techniques: The program makes use of classes (`DirControlPlicy` and `DeletePolicy`)to hold data and perform tasks. 

- Classes use appropriate access specifiers for class members: Class members are explicitly specified as private or public.

- Class constructors utilize member initialization lists. Member initialization lists are used in all classes.

- Overloaded functions allow same function to operate on different parameters: `DeletePolicy` class constructor is overloaded to operate on different parameters.

### Memory Management
- The program makes use of references in function declarations: Atleast two variables are defined as references.

- The project uses move semantics to move data, instead of copying it, where possible.

- The project uses smart pointers instead of raw pointers. `unique_ptr` are used extensively in the project. 

### Concurrency
- The project uses multithreading: The project makes use of parrallel execution when enforcing `DeletePolicy`

## NOTE:
If getting the error `filesystem` is not a namespace-name. Kindly ensure your compiler version is up to date with the standard C++17. Kindly see
[C++ Filesystem library](https://en.cppreference.com/w/cpp/filesystem)

