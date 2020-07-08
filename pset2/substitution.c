#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h>

#define KEY_MAX_LENGTH 26

bool is_valid(char arg[]);
int print_ciphertext(char plaintext[], char key[]);

int main(int argc, string argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s key\n", argv[0]);
    return 1;
  }
  if (!is_valid(argv[1]))
  {
    return 1;
  }

  /* Lower case the key */
  char key[KEY_MAX_LENGTH];
  for (int i = 0; i < KEY_MAX_LENGTH; i++)
  {
    key[i] = tolower(argv[1][i]);
  }
  key[KEY_MAX_LENGTH] = '\0';

  string plaintext = get_string("plaintext: ");
  print_ciphertext(plaintext, key);

  return 0;
}

/*
 * Validate the key:
 *    - Check the key length is 26 characters
 *    - Check there isn't non-alphabetic characters
 *    - Check there isn't any repeated character
 */
bool is_valid(char arg[])
{
  if (arg[KEY_MAX_LENGTH] != '\0')
  {
    printf("Key must contain 26 characters.\n");
    return false;
  }
  int aux = 1;
  for (int i = 0; i < KEY_MAX_LENGTH; i++)
  {
    if (isalpha(arg[i]) == 0)
    {
      printf("Key must be alphabetic.\n");
      return false;
    }
    for (int j = aux; j < KEY_MAX_LENGTH; j++)
    {
      if (toupper(arg[j]) == toupper(arg[i]))
      {
        printf("Key can't contain any repeated characters.\n");
        return false;
      }
    }
    aux++;
  }

  return true;
}

/*  Prints the ciphertext of the text given by the user */
int print_ciphertext(char plaintext[], char key[])
{
  printf("ciphertext: ");
  for (int i = 0, len = strlen(plaintext); i < len; i++)
  {
    /* If is not an alphabetic character prints it */
    if (isalpha(plaintext[i]) == 0)
    {
      printf("%c", plaintext[i]);
    }
    /* Substitutes the characters */
    for (int aux = 97; aux < 97 + KEY_MAX_LENGTH; aux++)
    {
      if (plaintext[i] == aux || plaintext[i] == aux - 32)
      {
        if (islower(plaintext[i]) != 0)
        {
          printf("%c", key[aux - 97]);
        }
        else
        {
          printf("%c", toupper(key[aux - 97]));
        }
      }
    }
  }
  printf("\n");
 
  return 0;
}

