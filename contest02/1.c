int
bitcount(STYPE value)
{
    int num = 0;
    UTYPE val = (UTYPE) value;
    while (val) {
        num += (val & 1);
        val >>= 1;
    }
    return num;
}

