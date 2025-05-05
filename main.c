#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

const char *HANGMAN[] = {
    "\n\n\n\n\n\n",
    "        |\n        |\n        |\n        |\n        |\n        |\n",
    "    ----+\n        |\n        |\n        |\n        |\n        |\n",
    "    +---+\n    |   |\n        |\n        |\n        |\n        |\n",
    "    +---+\n    |   |\n    O   |\n        |\n        |\n        |\n",
    "    +---+\n    |   |\n    O   |\n    |   |\n        |\n        |\n",
    "    +---+\n    |   |\n    O   |\n   /|   |\n        |\n        |\n",
    "    +---+\n    |   |\n    O   |\n   /|\\  |\n        |\n        |\n",
    "    +---+\n    |   |\n    O   |\n   /|\\  |\n   /    |\n        |\n",
    "    +---+\n    |   |\n    O   |\n   /|\\  |\n   / \\  |\n        |\n"
};

void clear_screen() {
    printf("\033[H\033[J");
}

int main() {
    FILE *fp = fopen("duden.txt", "r");
    if (!fp) {
        printf("File 'duden.txt' not found.\n");
        return 1;
    }

    char **words = NULL;
    char buffer[1024];
    int word_count = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        for (char *p = buffer; *p; ++p)
            *p = tolower((unsigned char)*p);

        char *word = malloc(strlen(buffer) + 1);
        if (!word) break;
        strcpy(word, buffer);

        char **tmp = realloc(words, (word_count + 1) * sizeof(char *));
        if (!tmp) break;
        words = tmp;
        words[word_count++] = word;
    }
    fclose(fp);

    if (word_count == 0) {
        printf("No words found.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));
    char *secret = words[rand() % word_count];
    int secret_len = strlen(secret);

    int letters[256] = {0};
    int guessed[256] = {0};
    char wrong[27] = {0};
    int lives = 0;

    for (int i = 0; i < secret_len; i++)
        letters[(unsigned char)secret[i]] = 1;

    while (1) {
        clear_screen();
        printf("============\n");

        if (lives == 10) {
            printf("You lose!\n");
            printf("It was: ");
            for (int i = 0; i < secret_len; i++)
                printf("%c", toupper((unsigned char)secret[i]));
            printf("\n");
            break;
        } else {
            printf("%s", HANGMAN[lives]);
        }

        printf("============\n\n");
        printf("Word: ");
        for (int i = 0; i < secret_len; i++) {
            if (guessed[(unsigned char)secret[i]])
                printf("%c ", toupper((unsigned char)secret[i]));
            else
                printf("_ ");
        }
        printf("\n\n");

        int won = 1;
        for (int i = 0; i < 256; i++) {
            if (letters[i] && !guessed[i]) {
                won = 0;
                break;
            }
        }
        if (won) {
            printf("You won!\n");
            break;
        }

        printf("Lives: %d\n", 9 - lives);
        if (wrong[0])
            printf("Wrong: %s\n", wrong);
        printf("\n");

        if (lives == 9) {
            printf("You lose!\n");
            printf("It was: ");
            for (int i = 0; i < secret_len; i++)
                printf("%c", toupper((unsigned char)secret[i]));
            printf("\n");
            break;
        }

        printf("Make a guess: ");
        char guess_input[100];
        if (!fgets(guess_input, sizeof(guess_input), stdin) || guess_input[0] == '\n')
            continue;

        char c = tolower((unsigned char)guess_input[0]);

        if (letters[(unsigned char)c]) {
            guessed[(unsigned char)c] = 1;
        } else {
            if (!strchr(wrong, toupper((unsigned char)c))) {
                size_t len = strlen(wrong);
                if (len < 26) {
                    wrong[len] = toupper((unsigned char)c);
                    wrong[len + 1] = '\0';
                    lives++;
                }
            }
        }
    }

    for (int i = 0; i < word_count; i++)
        free(words[i]);
    free(words);

    return 0;
}
