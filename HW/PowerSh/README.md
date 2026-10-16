# Project 4 — UNIX Shell and History Feature

## Submission

- **Canvas**: zip the folder containing your project (including simple-shell.c and Makefile) and submit via Canvas

## Overview

In this project, you will design a C program that acts as a simple UNIX shell. Your shell will display a prompt, accept a command from the user, and execute that command in a separate process.

For example:

```text
osh> ls
```

The parent shell process will read the command entered by the user, create a child process with `fork()`, and execute the command in the child using `execvp()`.

Unless otherwise specified, the parent process should wait for the child to finish before displaying the next prompt. Your shell must also support running a process in the background by placing a standalone `&` at the end of the command:

```text
osh> ls &
```

In this case, the parent shell should **not** wait for the child before continuing.

This project has two parts:

1. Create a child process and execute the user's command.
2. Add a command-history feature.

---

# Part I — Creating and Executing a Child Process

Your first task is to complete the shell so that it parses the user's command, creates a child process, and executes the command in that child.

## 1. Parsing the command

The `setup()` function reads the command into `inputBuffer` and separates it into individual arguments stored in `args`.

For example, if the user enters:

```text
osh> ps -ael
```

then `args` should contain:

```c
args[0] = "ps";
args[1] = "-ael";
args[2] = NULL;
```

The final entry in `args` **must be `NULL`** because the array will be passed to `execvp()`.

The provided starter code uses `read()` to get input from standard input:

```c
length = read(STDIN_FILENO, inputBuffer, MAX_LINE - 1);
```

`read()` does **not** automatically add the terminating `\0` required by a C string. The starter code safely adds this terminator for you after checking the return value from `read()`.

When parsing `inputBuffer`:

- A space `' '` or tab `'\t'` separates arguments.
- Replace argument-separating whitespace with `'\0'` so each argument becomes its own null-terminated C string.
- Store the address of the first character of each argument in `args[arg_index]`.
- A newline `'\n'` marks the end of a normal command entered with the Enter key and should also be replaced with `'\0'`.
- After parsing, set the next unused entry in `args` to `NULL`.

You will be modifying `inputBuffer` directly while also building the `args` array. Each element of `args` should point to the first character of one argument stored inside `inputBuffer`. Because `args` only stores pointers into `inputBuffer`, you must modify `inputBuffer` so that each argument is separated from the next by a `'\0'` character.

For example, the buffer for `ps -ael` will conceptually become:

```text
p s \0 - a e l \0
^      ^
|      |
args[0] args[1]
```

### Background commands

Your shell only needs to support `&` when it appears as the final, standalone item in a command. For example:

```text
osh> sleep 5 &
```

If a final `&` is present:

- Set `*background = 1`.
- Do **not** include `&` in the argument array passed to `execvp()`.

You do not need to support forms such as `sleep 5&`.

## 2. `setup()` return values

The starter code defines three possible return values:

```c
SETUP_EXIT     // stop the shell
SETUP_EXECUTE  // process this command
SETUP_SKIP     // ignore this input and display another prompt
```

Use them as follows:

- `SETUP_EXIT`: end-of-file was entered with CTRL-D.
- `SETUP_EXECUTE`: a valid command is ready to be processed.
- `SETUP_SKIP`: an invalid history request was entered; do not terminate the shell and do not attempt to execute the input.

This distinction is important: an invalid history request should return to the prompt, not terminate the shell and not create a child process.

## 3. Built-in `exit` command

If the user enters:

```text
osh> exit
```

the shell should terminate normally.

You may use `strcmp()` or `strncmp()` to check for this command. Make sure you are checking the command itself rather than accidentally matching a longer command that only begins with `exit`.

## 4. Creating the child process

For a normal command, create a child process using `fork()`.

Handle all three possible results:

- `fork() < 0`: an error occurred.
- `fork() == 0`: this is the child process.
- `fork() > 0`: this is the parent process.

In the child, execute the command using:

```c
execvp(args[0], args);
```

If `execvp()` succeeds, it does not return. Therefore, if it *does* return, execution failed. Print an error using `perror()` and terminate the child with a failure status so that the child does not accidentally continue running the shell loop.

In the parent:

- If `background == 0`, wait specifically for the child you just created using `waitpid()`.
- If `background == 1`, do not wait before displaying the next prompt.

For this project, cleanup of completed background processes is **outside the required scope**. You do not need to implement signal handling or nonblocking zombie cleanup.

---

# Part II — Command History

Your shell must maintain the **9 most recent commands**.

The history is displayed from **most recent to least recent**. For example, if the stored commands are:

```text
ps
top
cal
who
date
```

then entering:

```text
osh> history
```

should display:

```text
1 ps
2 top
3 cal
4 who
5 date
```

The number shown by `history` is the number used with `!N`. In other words, **`!1` always means the most recent displayed command**, regardless of where that command happens to be stored in the underlying circular array.

## 1. Maintaining history

The starter code provides:

```c
char history[MAX_COMMANDS][MAX_LINE];
int command_count = 0;
```

Use `history` to store commands in a form that can later be copied back into `inputBuffer` and executed.

You may use strcpy() when copying commands between inputBuffer and history.

Once more than 9 commands have been entered, only the 9 most recent commands should remain available. A circular-array approach is recommended.

`command_count` may be used to track how many commands have been successfully added since the shell started. The physical array index and the number shown by `history` are not necessarily the same once the circular buffer wraps.

### Commands that should be saved

- Normal executable commands should be added to history.
- A command recalled using `!!` or `!N` should be added to history **after it has been replaced by the recalled command**. Store the command that is actually executed, not the literal text `!!` or `!N`.
- The built-in `history` command should be added to history.
- The built-in `exit` command does **not** need to be added to history.
- Invalid history requests must **not** be added to history.
- If a command contains a final `&`, preserve the `&` in the saved history command even though it is removed from the `args` array used by `execvp()`.

## 2. `history`

When the user enters:

```text
osh> history
```

print all currently stored commands from most recent to least recent.

`history` is a shell built-in. After printing the history, return to the prompt. Do **not** call `fork()` or `execvp()` for this command.

## 3. `!!`

When the user enters:

```text
osh> !!
```

execute the most recent command in history.

Before executing it:

1. Copy the stored command into `inputBuffer`.
2. Echo the recalled command to the screen.
3. Update `length` so the replacement command is parsed correctly.
4. Add the recalled command to history as the newest command.

If there are no commands in history, print:

```text
No commands in history.
```

Then return to the prompt without creating a child process.

## 4. `!N`

When the user enters `!N`, execute the command currently displayed as number `N` by the `history` command.

For this project, `N` will be a **single digit from 1 through 9**. You do not need to support multi-digit history numbers.

For example:

```text
osh> !3
```

executes the command currently displayed as number 3.

Before executing it:

1. Copy the selected command into `inputBuffer`.
2. Echo the recalled command to the screen.
3. Update `length`.
4. Add the recalled command to history as the newest command.

You may use:

```c
isdigit(inputBuffer[1])
```

to determine whether the character after `!` is a digit.

Treat malformed or unavailable history requests as invalid, including examples such as:

```text
!
!0
!9        # when fewer than 9 commands are currently stored
!x
```

For an invalid `!N` request, print:

```text
No such command in history.
```

Then return `SETUP_SKIP` so that the shell displays another prompt without terminating and without attempting to execute the invalid input.

---

# Suggested Implementation Order

## Part I

1. Complete command parsing in `setup()`.
2. Make `exit` terminate the shell.
3. Use `fork()` to create a child.
4. Use `execvp()` in the child.
5. Use `waitpid()` in the parent for foreground commands.
6. Add support for final standalone `&` background commands.

## Part II

1. Implement `addtohistory()`.
2. Implement `!!` and `!N` replacement in `setup()`.
3. Make invalid history requests return `SETUP_SKIP`.
4. Implement the built-in `history` command in `main()`.
5. Verify that the circular history buffer still displays and recalls commands in the correct order after more than 9 commands have been entered.

---

# Required Testing

Your screenshots should demonstrate that your shell correctly handles at least the following cases:

```text
osh> ls
osh> pwd
osh> ps -ael
osh> sleep 2 &
osh> !!
osh> history
osh> !4
osh> !9          # test when that history entry does not exist
osh> exit
```

Also test enough commands to exceed the 9-command history limit and verify that:

- only the 9 most recent commands remain,
- `history` displays them from most recent to least recent, and
- `!N` executes the command corresponding to the displayed number.

---

# Notes

- Maximum command-line size: `MAX_LINE` characters, including space reserved for the terminating `\0`.
- Maximum history size: `MAX_COMMANDS`, which is 9 in the starter code.
- You may assume commands and arguments are separated by spaces or tabs.
- You do not need to implement pipes, redirection, quoting, wildcard expansion, environment-variable expansion, or job-control commands.
- You do not need to clean up completed background children for this project.

# Bonus — Reaping Background Processes

For bonus credit, update your shell so that completed background processes are cleaned up instead of remaining as zombie processes.

Your shell should:

- Install a signal handler for `SIGCHLD`.
- When `SIGCHLD` is received, use `waitpid()` with the `WNOHANG` option to reap any child processes that have terminated.
- Make sure the shell does not block while checking for completed background children.
- Continue to use the normal blocking `waitpid()` behavior for foreground commands.

Your signal handler should be able to clean up multiple terminated children if more than one background process finishes before the handler runs.

A common pattern is to repeatedly call:

```c
waitpid(-1, NULL, WNOHANG);
```

until there are no more terminated children waiting to be reaped.

---
# Grading Rubric (60 points + 5 bonus)

| Category Criteria |  | Points |
| --- | --- | ---: |
| Command parsing | Correctly parses `inputBuffer` into separate arguments, replaces separators with `'\0'`, stores pointers to each argument in `args`, terminates `args` with `NULL`, handles empty input appropriately, and correctly removes a final standalone `&` from the argument list. | 12 |
| Process creation and execution | Correctly creates a child with `fork()`, executes commands with `execvp()`, implements the built-in `exit` command, and handles `fork()`/`execvp()` errors appropriately. | 10 |
| Foreground and background processes | Correctly waits for foreground processes using `waitpid()`, does not wait for background processes, and correctly uses the background flag to determine the parent’s behavior. | 10 |
| Command history storage | Correctly stores commands in history, maintains only the 9 most recent commands, preserves commands so they can be recalled and parsed again, and correctly handles the history buffer after more than 9 commands have been entered. | 8 |
| `history` command | Correctly implements `history` as a built-in command and displays stored commands from most recent to least recent with the correct numbering. | 4 |
| History recall (`!!` and `!N`) | Correctly recalls, echoes, and executes commands using `!!` and `!N`, maps `!N` to the displayed history number, adds recalled commands back to history, and handles invalid history requests correctly. | 8 |
| Program testing and output | Submitted testing demonstrates normal commands, multi-argument commands, foreground/background execution, history features, invalid history requests, and behavior after exceeding the 9-command history limit. | 5 |
| Code quality and submission | Code compiles without errors, avoids major warnings, uses readable organization and naming, and is submitted in the required `.zip` format with the required files. | 3 |
| **Total** |  | **60** |
| **Bonus — Background child cleanup** | Uses a `SIGCHLD` signal handler and `waitpid(..., WNOHANG)` to reap terminated background processes without blocking the shell. | **+5** |