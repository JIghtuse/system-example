#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <mntent.h>

#define MTAB_PATH "/etc/mtab"

bool is_mounted(const char *path)
{
    FILE *fp = setmntent(MTAB_PATH, "r");
    struct mntent *entry;

    while ((entry = getmntent(fp)) != NULL)
        if (!strcmp(entry->mnt_fsname, path))
            return true;
    return false;
}

/* Parses mounted filesystem description file /etc/mtab to find if specific
 * device path is mounted currently */
int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s dev_path\n", argv[0]);
        fprintf(stderr, "For example, %s /dev/sda1\n", argv[0]);
        return 1;
    }

    const char *dev_path = argv[1];

    printf("%s is ", dev_path);
    printf("%s\n", is_mounted(dev_path) ? "mounted" : "not mounted");

    return 0;
}
