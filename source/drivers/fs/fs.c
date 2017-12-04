#include <boot.h>
#include <inode.h>

char     *image  = 0;
volume_t *volume = (volume_t*)(0x00101000);
block_t  *blocks = (block_t*)(0x00101000 + sizeof(volume_t));
inode_t  *inodes = (inode_t*)(0x00101000 + sizeof(volume_t) + sizeof(block_t)*MAX_BLOCKS);

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