5. Update

ha csak exportot irunk be akkor is ki kell irja az env-t


4. Update

Jelenlegi állapotban egy beépített parancsot elvégez egy parancssorban.


TODO: 
--$ jel kibontás véglegesíteni (var->env-böl) és $?-t megírni
--beépített parancsok hibaüzenete és hibakezelése
	pl. nem létezö directory, unsetelt HOME stb.
--nem beépített parancsok elvégzése


3. Update

# define PROMPT "\001\033[1;32m\002minishell> \001\033[0m\002"

Igy nem ir ra a promptra

MAGYARAZAT:

Ctrl+A: \001 (ASCII Start of Header)
Ctrl+B: \002 (ASCII Start of Text)

Ha vannak szinek akkor jelezni kell neki, hogy hol kezdodik es vegzodik a prompt, ha nincsennek szinek akkor nem is kell foglalkozni ilyesmivel

Stackoverflow: https://unix.stackexchange.com/questions/486789/input-from-readline-owerwrites-the-prompt


2. Update

Implementaltam a signal.c funkciot ahol kezelve van a CTRL + C es CTRL + D.

1. Update.


A Lexer resz hasznalhato. Az iterativ modszert hasznaltam, mivel mar elore definialtad a szamokat. Ha ez a modszer nem lesz eleg hatasos akkor atirom a State Maschine modszerre.

A jelenlegi Lexer-t le lehet tesztelni. Barmit beadsz a minishellnek ki fogja printelni, hogy a tokenizalo, mive alakitotta at az adott szoveget.

Valamint a norminette is hibatlan jeneleg



# VALGRIND
valgrind --leak-check=full --suppressions=readline.supp --trace-children=yes --track-fds=yes ./minishell

******************************************************************
bszilas@c4r1p10:~/m3/gh_msh/Beni/egy/ketto$ rm -rf /home/bszilas/m3/gh_msh/Beni/egy/
bszilas@c4r1p10:~/m3/gh_msh/Beni/egy/ketto$ cd ..
cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory
bszilas@c4r1p10:~/m3/gh_msh/Beni/egy/ketto/..$ echo $?
0
bszilas@c4r1p10:~/m3/gh_msh/Beni/egy/ketto/..$ pwd
/home/bszilas/m3/gh_msh/Beni/egy/ketto/..
bszilas@c4r1p10:~/m3/gh_msh/Beni/egy/ketto/..$ echo $PWD
/home/bszilas/m3/gh_msh/Beni/egy/ketto/..
******************************************************************