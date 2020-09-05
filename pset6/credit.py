#!/bin/python

numbers = [
    "378282246310005",      # AMEX
    "371449635398431",      # AMEX
    "5555555555554444",     # MASTERCARD
    "5105105105105100",     # MASTERCARD
    "4111111111111111",     # VISA   
    "4012888888881881",     # VISA   
    "1234567890",           # INVALID
    "369421438430814",      # INVALID
    "4062901840",           # INVALID
    "5673598276138003",     # INVALID
    "4111111111111113"      # INVALID
    ]

def main():
    number = input("Number: ")
    validate(list(number))

# Validate credit card number
def validate(n):
    check_sum = checksum(n)
    n = list(map(int, n))

    if check_sum % 10 == 0:
        if len(n) == 13 or len(n) == 16 and n[0] == 4:
            print("VISA")
        elif len(n) == 15 and n[0] == 3 and n[1] == 4 or n[1] == 7:
            print("AMEX")
        elif len(n) == 16 and n[0] == 5 and n[1] >= 0 and n[1] <= 5:
            print("MASTERCARD")
        else:
            print("INVALID")
    else:
        print("INVALID")

# Luhn's algorithm to get checksum of credit card
def checksum(n):
    sum_list = []
    check_sum_2 = 0
    n.reverse()

    for i in range(len(n)):
        digit = int(n[i])
        if i % 2 != 0:
            digit *= 2
            if digit > 9:
                digit = str(digit)
                sum_list.extend([digit[0], digit[1]])
            else:
                sum_list.append(str(digit))    
        else:
            check_sum_2 += digit

    check_sum_1 = sum(list(map(int, sum_list)))
    n.reverse()     # Reverse back to it's original order

    return check_sum_1 + check_sum_2

main()
