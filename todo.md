# TODO

## 1 Built-ins

cd
exit
maybe pwd
maybe export / unset

cd and exit especially matter because they must affect the shell process itself, not a child.

## 2 Better parsing

quoted strings: "hello world"
single quotes: 'hello world'
escaped characters: \|
making sure operators inside quotes aren't treated as syntax

This is probably the biggest thing separating a toy parser from something shell-like.

## 3 Multiple pipes

support:

cat file | grep foo | sort | uniq
this is a natural generalization of what you just built.

## 4 More redirection forms

append:

>>

stderr:

2>

possibly descriptor duplication:

2>&1

## 5 Environment variables

$HOME
$PATH
$USER
$?
command environment handling

You already benefit from PATH indirectly through execvp(), but expanding $VAR yourself is shell behavior.

## 6 Command sequencing

;
&&
||

These introduce execution based on exit status:

command1 && command2

means run command2 only if command1 succeeds.

## 7 Background execution

&

example:

sleep 20 &

This means your shell should not wait immediately for that job.

## 8 Signal handling

Ctrl+C
Ctrl+Z
making sure Ctrl+C kills the foreground command rather than your shell

This is a very important milestone.

## 9 Process groups and job control

jobs
fg
bg
foreground/background process groups

This is where shell implementation gets significantly more advanced.

## 10 Quality-of-life features
command history
arrow-key navigation
prompt customization
tab completion



If your goal is "I built a legitimate small Unix shell," I would say the minimum convincing feature set is:

external commands
built-ins
redirection
pipes
quoted arguments
environment-variable expansion
&& / || / ;
Ctrl+C handling
background jobs with &

Once you have those, I would comfortably call it a real shell, just a small one.

Given where you are now, I would do built-ins properly next, then quoting/parser cleanup, then multiple pipes, then &&/||. After that, move into signals and job control. That order keeps the complexity increasing gradually rather than throwing process groups at you immediately.
