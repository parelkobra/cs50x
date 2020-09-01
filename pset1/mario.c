#include <stdio.h>
#include <cs50.h>

#define PYRAMIDS_GAP "  "

int get_pyramids_height();
void draw_pyramid(int height);

int main(void)
{
  int height = get_pyramids_height();
  draw_pyramid(height);
}

void draw_pyramid(int height)
{
    int offset1 = height - 1;
    int offset2 = 1;
    
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < offset1; column++)
        {
            printf(" ");
        }
        for (int column = 0; column < height - offset1; column++)
        {
            printf("#");
        }
        printf(PYRAMIDS_GAP);
        for (int column = 0; column < offset2; column++)
        {
            printf("#");
        }
        printf("\n");
        
        offset1--;
        offset2++;
    }
}

int get_pyramids_height()
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    return height;
}
