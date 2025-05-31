# Shell Demo 

## Description  

This project implements a mini UNIX-like shell, written in C, that provides an interactive environment for executing commands. The shell includes user authentication and manually implemented system commands without relying on external system calls like `system()`.  

## Key Features  

- **Custom Shell Environment** – Provides an interactive terminal for executing supported commands.  
- **User Authentication** – Requires users to log in with valid credentials stored in `passwords.txt` before executing commands.  
- **Custom Command Implementations:**  
  - **`first`** – Displays the first N lines of a file.  
    - **`-n [number]`** – Specifies how many lines to display from the beginning of the file.  
  - **`last`** – Displays the last N lines of a file.  
    - **`-n [number]`** – Specifies how many lines to display from the end of the file.  
  - **`cat`** – Outputs file contents with extra functionality.  
    - **`-b`** – Numbers non-empty lines.  
    - **`-c [number]`** – Displays only the first specified number of characters.  
  - **`ls`** – Lists directory contents with various options.  
    - **`-i`** – Displays the inode number of each file.  
    - **`-l`** – Displays detailed file information (permissions, size, owner, etc.).  
    - **`-a`** – Includes hidden files in the listing.  
- **Manual System Execution** – Implements a custom command execution mechanism instead of relying on `system()`, promoting a deeper understanding of shell behavior.  
- **File Handling** – Enables reading, writing, and displaying content from files through implemented commands.  
- **Process Control** – Ensures efficient execution and error handling while interacting with the underlying system.  

## 📂 Project Structure  

- **`main.c`** – Main shell logic, handling authentication and user interaction.  
- **`login.c`** – Login function to validate user credentials.  
- **`system.c`** – Custom implementation of `system()` for command execution.  
- **`ls.c`**, **`cat.c`**, **`first.c`**, **`last.c`** – Custom implementations of UNIX commands.  
- **`passwords.txt`** – Stores usernames and passwords.  
