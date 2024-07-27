# Recorder

## Function

This module is used to record and save data to the flash drive for later retriever.
? everytime the chip boots up, we create a file and we can start recording some data for later?
? way to save error logs asw ?
i mean we can save anything to the flash we want i guess, we can think about it later

## Topology (top to bottom)

1. recorder - all the top level function
2. lfs_adapter - middle ware
3. w25q - low lever driver

## Driver - W25Q

The terms page, sector, and block are fundamental to understanding flash memory architecture, particularly for devices like the Winbond W25Q128. These terms describe different granularities at which data can be read, written, and erased. Here's a breakdown:

1. Page
Definition: A page is the smallest unit of data that can be programmed (written to) in flash memory.
Size: For the W25Q128, a page typically consists of 256 bytes.
Operations: Pages can be written to in a single operation, but only from 1s to 0s (you cannot write 0s to 1s without erasing first). Partial page programming is allowed, but each time you program, you can only change bits from 1 to 0. To change bits from 0 to 1, the entire page must first be erased as part of a sector erase.
2. Sector
Definition: A sector is a larger unit that consists of multiple pages. It is the smallest unit that can be erased.
Size: The W25Q128 has sectors of 4 KB, meaning each sector contains 16 pages (16 pages x 256 bytes/page = 4096 bytes or 4 KB).
Operations: Sectors can be erased, setting all bits to 1 (or FF in hexadecimal), allowing them to be reprogrammed. This is because flash memory requires an erase operation before data can be rewritten.
3. Block
Definition: A block is an even larger unit of flash memory, consisting of multiple sectors. It is another unit of memory that can be erased.
Size: The W25Q128 typically has blocks of 64 KB or 32 KB:
64 KB Block: Contains 16 sectors (16 sectors x 4 KB/sector).
32 KB Block: Contains 8 sectors (8 sectors x 4 KB/sector).
Operations: Blocks are typically erased as a whole, which is useful for managing large amounts of data that need to be frequently updated.
Hierarchical Structure
To summarize, the flash memory layout hierarchy for the W25Q128 can be described as:

Chip: The entire memory device, with a size of 16 MB (128 Mb).
Block: A large unit, typically 64 KB or 32 KB, composed of multiple sectors.
Sector: A smaller unit, typically 4 KB, composed of multiple pages.
Page: The smallest unit of programming (writeable), typically 256 bytes.
Each unit serves a different purpose in the management of data storage:

Pages are the smallest unit for writing.
Sectors are the smallest unit for erasing.
Blocks provide a larger erase unit, often used for more substantial updates.
Understanding these units helps in optimizing read, write, and erase operations for specific applications, ensuring efficient use of the flash memory's capabilities and longevity.
