#include <boot.h>
#include <inode.h>

char     *image;
volume_t *volume;
block_t  *blocks;
inode_t  *inodes;

_declspec(dllexport)
int initfs(int magic, uint32_t *info[], MBHDR *mbhdr) {
    char *imagebase = (char*)mbhdr->initrd;
    image  = 0;
    volume = (volume_t*)imagebase;
    blocks = (block_t*)&imagebase[sizeof(volume_t)];
    inodes = (inode_t*)&imagebase[sizeof(volume_t) + sizeof(block_t)*MAX_BLOCKS];
    return mbhdr->initrd;
}

// used by build tools to manipulate initrd image
_declspec(dllexport)
int fsstart(char *ram, int disk) {
    image  = ram;
    volume = (volume_t*)&image[disk];
    blocks = (block_t*)&image[disk + sizeof(volume_t)];
    inodes = (inode_t*)&image[disk + sizeof(volume_t) + sizeof(block_t)*MAX_BLOCKS];
    return disk;
}

int __stdcall main(int a, int b, int c) {
    return 1;
}