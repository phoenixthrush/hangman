import requests
import random

# secret = "Anime".strip().lower()
letters = {}
wrong = []
lives = 0

# Fetch the word list from the URL
url = "https://raw.githubusercontent.com/MaximilianFreitag/Zufallswort/refs/heads/main/duden.txt"
response = requests.get(url)

if response.status_code == 200:
    words = response.text.splitlines()
    secret = random.choice(words).strip().lower()
else:
    print("Failed to fetch the word list. Using default word.")

HANGMAN = [
    """






    """,
    """
        |
        |
        |
        |
        |
        |
    """,
    """
    ----+
        |
        |
        |
        |
        |
    """,
    """
    +---+
    |   |
        |
        |
        |
        |
    """,
    """
    +---+
    |   |
    O   |
        |
        |
        |
    """,
    """
    +---+
    |   |
    O   |
    |   |
        |
        |
    """,
    """
    +---+
    |   |
    O   |
   /|   |
        |
        |
    """,
    """
    +---+
    |   |
    O   |
   /|\\  |
        |
        |
    """,
    """
    +---+
    |   |
    O   |
   /|\\  |
   /    |
        |
    """,
    """
    +---+
    |   |
    O   |
   /|\\  |
   / \\  |
        |
    """
]


# dict of chars which need to be guessed
for char in secret:

    if char not in letters:
        letters[char] = False

while True:
    # clear screen
    print("\033[H\033[J", end="")

    # break if no lives left
    print('=' * 12)

    if lives == 10:
        # below prints are probably useless?
        print("You lose!")
        print(f"It was: {secret.upper()}")
        break
    else:
        print(HANGMAN[lives])

    print('=' * 12)
    print()

    # print word
    print("Word: ", end='')

    for char in secret:
        if letters[char]:
            print(f"{char.upper()} ", end='')
        else:
            print("_ ", end='')

    print("\n")

    # break if all chars are guessed
    if all(letters[char] for char in letters):
        print("You won!")
        break

    # print(f"Debug: {letters}")
    print(f"Lives: {9 - lives}")

    if wrong:
        print(f"Wrong: {', '.join(wrong).upper()}")

    print()
    if lives != 9:
        guess = input("Make a guess: ").strip().lower()[0]
    else:
        print("You lose!")
        print(f"It was: {secret.upper()}")
        break

    if guess in letters:
        letters[guess] = True
    else:
        if guess not in wrong:
            wrong += guess
            lives += 1
