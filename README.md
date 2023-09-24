# Git redone
## A re-implementation of git primitives

A version control software is a tool that enables the storage, tracking, and management of multiple versions of a project (or a set of files). These tools provide easy access to the history of all changes made to the files, allowing for the retrieval of a previous version in case of issues. Additionally, they are highly valuable for collaborative work, intelligently merging different versions of the same project. For instance, these tools are commonly used in software development to facilitate teamwork and maintain the source code for various versions of a software.

The objective of this project is to study how version control software functions by detailing various data structures involved in its implementation. Specifically, we will focus on the following functionalities:

    How to enable a user to create snapshots of their project?
    How to allow them to navigate freely through different snapshots?
    How to build and maintain a hierarchy of different project versions?
    How to save changes that are not part of a snapshot?

The current version of this project proposes the following instruction list:
    
    ./myGit init
    ./myGit list-refs 
    ./myGit create-ref <name> <hash> 
    ./myGit delete-ref <name> 
    ./myGit add <elem> [<elem2> <elem3> ...] 
    ./myGit list-add 
    ./myGit clear-add 
    ./myGit commit [-m \"<message>\"] 
    ./myGit get-current-branch 
    ./myGit branch <branch-name> 
    ./myGit branch-print <branch-name> 
    ./myGit checkout-branch <branch-name> 
    ./myGit checkout-commit <pattern> 
    ./myGit merge <branch> <message> 