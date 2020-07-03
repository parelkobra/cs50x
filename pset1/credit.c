#include <stdio.h>
#include <cs50.h>

#define AMEX "AMEX\n"
#define MASTERCARD "MASTERCARD\n"
#define VISA "VISA\n"
#define INVALID "INVALID\n"
#define MAX_LENGTH 16

// 378282246310005      // AMEX
// 371449635398431      // AMEX
// 5555555555554444     // MASTERCARD
// 5105105105105100     // MASTERCARD
// 4111111111111111     // VISA
// 4012888888881881     // VISA
// 1234567890           // INVALID
// 369421438430814      // INVALID
// 4062901840           // INVALID
// 5673598276138003     // INVALID
// 4111111111111113     // INVALID

int validate_cc(long cc_number);

int main(void)
{
    long cc_number = get_long("Introduce the card number: ");
    validate_cc(cc_number);
    return 0;
}

int validate_cc(long cc_number) {
    int first_digit, second_digit, third_digit, fourth_digit;
    long aux = 10;
    long aux_b = 1;
    int sum = 0;
    for (int i = 0; i < MAX_LENGTH; i++)
    {
        long remainder = cc_number % aux;
        int digit = remainder / aux_b;

        /* I get the first, second and third digits of the credit card
         * number. That way I will be able to tell the length (?) of
         * the credit card number, and whether it's American Express,
         * MasterCard or Visa.
         */
        switch (i)
        {
          case 12:
            fourth_digit = digit;
            break;
          case 13:
            third_digit = digit;
            break;
          case 14:
            second_digit = digit;
            break;
          case 15:
            first_digit = digit;
            break;
        }

        if (i % 2 != 0)
        {
            digit = digit * 2;
            if (digit > 9)
            {
              digit = digit / 10 + digit % 10;
            }
        }
        sum += digit;
        aux *= 10;
        aux_b *= 10;
    }
    if (sum % 10 == 0)
    {
      // Doesn't have the appropiate length
      if (first_digit == 0 && second_digit == 0 && third_digit == 0 && fourth_digit == 0)
      {
        printf(INVALID);
      }
      // It has 13 digits and the first one is 4
      else if (first_digit == 0 && second_digit == 0 && third_digit == 0 && fourth_digit == 4)
      {
        printf(VISA);
      }
      // It has 15 digits and starts by 34 or 37
      else if (first_digit == 0 && second_digit == 3 && (third_digit == 4 || third_digit == 7))
      {
        printf(AMEX);
      }
      // It has 16 digits and starts between 51 and 55
      else if (first_digit == 5 && second_digit >= 1 && second_digit <= 5)
      {
        printf(MASTERCARD);
      }
      // It has 16 digits and starts by 4
      else if (first_digit == 4)
      {
        printf(VISA);
      }
      else
      {
        printf(INVALID);
      }
    }
    else
    {
      printf(INVALID);
    }
}
