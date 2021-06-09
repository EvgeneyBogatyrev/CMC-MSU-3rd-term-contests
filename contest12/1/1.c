STYPE
bit_reverse(STYPE value)
{
    UTYPE num = (UTYPE) value;
    UTYPE res = 0;

    UTYPE counter = (UTYPE) (-1);
    while (counter > 0) {
        res <<= 1;
        res |= num & 1;
        num >>= 1;

        counter >>= 1;
    }

    return (STYPE) res;
}
