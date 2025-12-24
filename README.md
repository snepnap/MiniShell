MiniShell - POSIX Compliant C Shell
A lightweight, high-performance Linux shell built in C. This project demonstrates low-level system programming, process management, and OS kernel interaction.

🚀 Features
Process Management: Uses fork() and execvp() system calls to manage child processes.

Built-in Commands: Native support for cd, exit, and history.

I/O Redirection: Supports output redirection using the > operator via dup2() and file descriptors.

Command History: Implements a Circular Buffer data structure to store and retrieve the last 10 user commands.

Custom Prompt: A color-coded, interactive CLI prompt for better User Experience.

🛠️ Technical Concepts Implemented
Pointers & Memory: Managed string tokenization using strtok and dynamic argument arrays.

File Descriptors: Manipulated the file descriptor table to redirect STDOUT to disk.

Data Structures: Created a custom circular buffer for efficient memory usage in history tracking.

Linux/POSIX API: Interfaced directly with the OS kernel through system calls.

📋 How to Build and Run
Requirement: You must be on a Linux environment (Ubuntu, Debian, or WSL).

Clone the project:

Bash

git clone https://github.com/YOUR_USERNAME/minishell.git
cd minishell
Compile:

Bash

gcc minishell.c -o minishell
Launch:

Bash

./minishell
📸 Usage Examples
Standard Commands: ls -la, pwd, mkdir test

Redirection: ls > output.txt (Saves directory list to a file)

History: Type history to see your previous command logs.