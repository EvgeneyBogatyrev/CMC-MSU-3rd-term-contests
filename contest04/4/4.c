const char sample[] = "rwxrwxrwx";
    
int
parse_rwx_permissions(const char *str) 
{
    if (!str) {
        return -1;
    }

    int len = sizeof(sample) - 1;
    int mode = 0;

    for (int i = 0; i < len; ++i) {
        if (*str == sample[i]) {
            mode |= 1 << (len - i - 1);
        } else if (*str != '-') {
            return -1;
        }
        ++str;
    }
    if (*str) {
        return -1;
    }

    return mode;
}
