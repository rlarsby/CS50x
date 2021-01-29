from cs50 import get_string

def main():

    text = get_string("Text: ")

    #count sentences
    sentences = 0
    for i in range(len(text)):
        if text[i] == "!" or text[i] == "?" or text[i] == ".":
            sentences = sentences + 1

    #count letters
    letters = 0
    for i in range(len(text)):
        if text[i] >= "a" and text[i] <= "z":
            letters = letters + 1
        if text[i] >= "A" and text[i] <= "Z":
            letters = letters + 1

    #count words
    words = 0
    for i in range(len(text)):
        if text[i] == " ":
            words = words + 1
    if letters > 0:
        words = words + 1

    #calculate index
    L = (letters / words) * 100
    S = (sentences / words) * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)

    #print correct grade
    if index >= 16:
        print("Grade 16+")

    elif index < 1:
        print("Before Grade 1")

    else:
        print(f"Grade {index}")

main()