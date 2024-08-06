#include "lfs.h"
#include "W25Q.hpp"

// variables used by the filesystem
lfs_t lfs;
lfs_file_t file;

static uint8_t read_buffer[512] = {};
static uint8_t prog_buffer[512] = {};
static uint8_t lookahead_buffer[512] = {};

static int W25Q_lfs_Read(const struct lfs_config *c [[maybe_unused]], lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    int16_t status = W25Q_ReadSector(static_cast<uint8_t *>(buffer), block, size, off);
    if (status == W25Q_OK)
    {
        return LFS_ERR_OK;
    }
    else
    {
        return -2;
    }
}

static int W25Q_lfs_Program(const struct lfs_config *c [[maybe_unused]], lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    int16_t status = W25Q_ProgramSector((uint8_t *)buffer, block, size, off);
    if (status == W25Q_OK)
    {
        return LFS_ERR_OK;
    }
    else
    {
        return -3;
    }
}

static int W25Q_lfs_EraseSector(const struct lfs_config *c [[maybe_unused]], lfs_block_t block)
{
    int16_t status = W25Q_EraseSector(block);
    if (status == W25Q_OK)
    {
        return LFS_ERR_OK;
    }
    else
    {
        return -4;
    }
}

static int W25Q_lfs_Sync(const struct lfs_config *c [[maybe_unused]])
{
    // maybe flash a light or something
    // this call is a bit useless tbh, because the data is commited immediately
    return 0;
}

// configuration of the filesystem is provided by this struct
// dont change the order of the members, otherwise it doesnt compile :'D
const struct lfs_config cfg = {
    // block device operations
    .read = W25Q_lfs_Read,
    .prog = W25Q_lfs_Program,
    .erase = W25Q_lfs_EraseSector,
    .sync = W25Q_lfs_Sync,

    // block device configuration
    .read_size = 256,
    .prog_size = 256,
    .block_size = 4096,
    .block_count = 256,
    .block_cycles = 500,
    .cache_size = 512,
    .lookahead_size = 512,
    .compact_thresh = -1,

    .read_buffer = read_buffer,
    .prog_buffer = prog_buffer,
    .lookahead_buffer = lookahead_buffer,
};

// entry point
int8_t initLittleFs(void)
{
    // mount the filesystem
    int8_t err = lfs_mount(&lfs, &cfg);

    // read current count
    uint32_t boot_count = 0;
    lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

    // update boot count
    boot_count += 1;
    lfs_file_rewind(&lfs, &file);
    lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&lfs, &file);

    // release any resources we were using
    err = lfs_unmount(&lfs);

    return err;
}
