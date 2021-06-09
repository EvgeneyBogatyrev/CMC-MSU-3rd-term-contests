#include <stdio.h> 
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>

enum 
{ 
    ARG_NUM = 3,
    FILE_SIZE = 64 * 1024,
    OFFSET_ARG = 2,
    BIT_NUM = 9,
    STR_NUM = 0x1ff
};

int
main(int argc, char *argv[])
{
    if (argc < ARG_NUM) {
        fprintf(stderr, "Wrong number of arguments\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);

    if (fd == -1) {
        fprintf(stderr, "Cannot open\n");
        return 1;
    }

    unsigned int table_offset; 
    if (sscanf(argv[OFFSET_ARG], "%x", &table_offset) != 1) {
        fprintf(stderr, "sscanf failed\n");
        return 1;
    }


    uint16_t *p_start = mmap(NULL, FILE_SIZE, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    
    uint16_t *table = (p_start + table_offset / sizeof(uint16_t));


    unsigned int address;
    while (scanf("%x", &address) == 1) {

        uint16_t address_16 = (uint16_t) address;

        uint16_t table_data = *(table + (address_16 >> BIT_NUM));
        
        address_16 &= STR_NUM;
        address_16 |= table_data;

        uint16_t data = *(p_start + address_16 / sizeof(uint16_t));
        printf("%" PRIu16 "\n", data);
    }

    munmap(p_start, FILE_SIZE);

    return 0;
}
