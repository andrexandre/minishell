# Minishell

A "mini" shell implementation inspired by bash. This project aims to recreate a basic command-line interpreter

## Features

- Command execution with `PATH` handling
- Built-in commands (`cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`)
- Environment variable expansion
- Signal handling `Ctrl+C`, `Ctrl+D`, `Ctrl+\`
- Redirections `>`, `<`, `>>`, `<<`
- Pipes `|`
- Quote handling `'` and `"`
- Error handling with appropriate error messages

## Usage

1. Clone the repository
```sh
git clone https://github.com/andrexandre/minishell.git ; cd minishell
```

2. Compile the program
> The curses and readline libraries are required\
> `sudo apt libncurses-dev libreadline-dev`
```sh
make
```

3. Run the prorgam
```sh
./minishell
```

### Commands

- Use the shell like any regular bash shell
- Type commands and press Enter to execute
- Use Ctrl+D to exit the shell
- Use Ctrl+C to interrupt a running process

> [!NOTE]
> This project is part of the 42 School curriculum
