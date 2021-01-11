# Incremenator

Incremenator is a basic shell-script used to create a reflecting amount of folders for each file in the specified directory. <br>
\* Files are only counted if they have the specified extension.

This shell-script was made to help myself personally upon downloading episodes of a season from some TV show. 

***

### Usage

`increment --directory ~/tests/ --extension mkv`

### How to install (Linux -- Ubuntu)

**Prerequisite:** [Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)

1. `cd ~/Downloads`
2. `git clone https://github.com/ObscureNightingale/incremenator`
3. `sudo mv ~/Downloads/incremenator/increment /usr/local/bin`
4. `increment --help`

Afterwards, a help page should echo to the terminal.

---

# Help

__Values__:
* FILEPATH  - directory from which to read the files
* EXTENSION - extension which is required in order to count the file as valid
* NULL      - blank; no value necessary

|     Flag      |    Aliases    |   Value   |                             Description                             |
|:-------------:|:-------------:|:---------:|:-------------------------------------------------------------------:|
|   `--help`    |     `-h`      |   NULL    |                       Displays the help page                        |
| `--directory` | `-dir`, `-d`  | FILEPATH  |               Directory where the files are located.                |
| `--extension` | `-ext`, `-ex` | EXTENSION | Extension which the file must have in order to be counted as valid. |