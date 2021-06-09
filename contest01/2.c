#include <stdio.h>
#include <ctype.h>

enum
{
    LEN = 64,
    DIGITS_START = 1,
    LOWER_START = DIGITS_START + '9' - '0' + 1,
    UPPER_START = LOWER_START + 'z' - 'a' + 1,
    BIT3_MASK = 0b00001000,
    BIT2_MASK = 0b11111011
};

int
get_symbol_number(int symbol)
{
    if (isdigit(symbol)) {
        return symbol - '0' + DIGITS_START;
    }

    if (isalpha(symbol)) {
        if (islower(symbol)) {
            return symbol - 'a' + LOWER_START;
        }
        if (isupper(symbol)) {
            return symbol - 'A' + UPPER_START;
        }        
    }
    
    return 0;
}

unsigned char
convert(unsigned char bit)
{        
    bit ^= BIT3_MASK;
    bit &= BIT2_MASK;

    return bit;
}

void
fill_symbols(unsigned char *symbols)
{
    int counter = 0;
    symbols[counter++] = '@';
    for (unsigned char s = '0'; s <= '9'; ++s) {
        symbols[counter++] = s;
    }
    for (unsigned char s = 'a'; s <= 'z'; ++s) {
        symbols[counter++] = s;
    }
    for (unsigned char s = 'A'; s <= 'Z'; ++s) {
        symbols[counter++] = s;
    }
    symbols[counter++] = '#';
}

int
main(void)
{
    unsigned char symbols[LEN];
    fill_symbols(symbols);

    int c;
    while ((c = getchar()) != EOF) {
        c = get_symbol_number(c);
        if (c != 0) {
            putchar(symbols[convert(c)]);
        }
    }
    return 0;
}
