#include <stdio.h>
#include <math.h>
#include <cs50.h>

float get_grade_level(char text[]);
int get_sentences(char text[]);
int get_words(char text[]);
int get_letters(char text[]);
bool is_a_letter(char c);
int print_grade(float grade);

int main()
{
    string text = get_string("Introduce the text: ");
    float grade = get_grade_level(text);
    print_grade(roundf(grade));
    return 0;
}

int get_sentences(char text[])
{
    int sentences = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}

int get_words(char text[])
{
    int words = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == ' ')
        {
            words++;
        }
    }
    words++;
    return words;
}

int get_letters(char text[])
{
    int letters = 0;
    for (int i = 0; text[i] != '\0'; i++)
    {
        if (is_a_letter(text[i]))
        {
            letters++;
        }
    }
    return letters;
}

bool is_a_letter(char c)
{
    return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) ? true : false;
}

float get_grade_level(char text[])
{
    int sentences = get_sentences(text);
    int words = get_words(text);
    int letters = get_letters(text);
    /* Average letters per 100 words */
    float L = (float) letters / words * 100;
    /* Average sentences per 100 words */
    float S = (float) sentences / words * 100;

    return L * 0.0588 - 0.296 * S - 15.8;
}

int print_grade(float grade)
{
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.f\n", grade);
    }
    return 0;
}
