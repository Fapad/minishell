5. Update

ha csak exportot irunk be akkor is ki kell irja az env-t


4. Update

Jelenlegi állapotban egy beépített parancsot elvégez egy parancssorban.


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
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --show-error-list=yes --suppressions=readline.supp --trace-children-skip="$(echo /bin/* /usr/bin/* /usr/sbin/* /home/bszilas/.capt/* $(which norminette) | tr ' ' ',')" --trace-children=yes --track-fds=yes ./minishell

To scroll within a tmux pane, you need to enter copy mode. Here’s how you can do it:

    Enter copy mode:
        Press Ctrl+b followed by [ (the square bracket).

    Scroll through the content:
        Use the arrow keys (Up and Down) to scroll line by line.
        Use PgUp and PgDn to scroll by pages.
        You can also use Ctrl+u and Ctrl+d to scroll up and down by half a page, respectively.

    Exit copy mode:
        Press q to exit copy mode and return to normal mode.