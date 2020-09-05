#!/bin/python

block = "#"
space = " "

def main():
    height = get_height()
    draw_pyramid(height)

def get_height():
    while True:
        try:
            height = int(input("Height: "))
        except ValueError:
            continue
        if height > 0 and height < 9:
            break

    return height

def draw_pyramid(height):
    height = int(height)
    lside, rside = height - 1, 1

    for _ in range(height):
        # Draw left side white spaces
        for _ in range(lside):
            print(space, end="")

        # Draw left side's pyramid
        for _ in range(height - lside):
            print(block, end="")

        # Draw gap
        for _ in range(2):
            print(space, end="")

        # Draw right side's pyramid
        for _ in range(rside):
            print(block, end="")

        print()
        lside -= 1
        rside += 1

main()
