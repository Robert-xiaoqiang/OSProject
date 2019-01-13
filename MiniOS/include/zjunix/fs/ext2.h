#ifndef _ZJUNIX_FS_EXT_2_H
#define _ZJUNIX_FS_EXT_2_H

#ifndef EXT2_DEBUG
#include <zjunix/fs/fat.h>
#endif

#include <zjunix/type.h>
#include <zjunix/fs/fscache.h>

#define EXT2_BOOT_BLOCK_SECT                2
#define EXT2_SUPER_BLOCK_SECT               2
#define EXT2_NAME_LEN                       255
#define EXT2_GROUP_DESC_BYTE                32
#define EXT2_ROOT_INO                       2
#define EXT2_N_BLOCKS                       15
#define EXT2_BASE_BLOCK_SIZE                1024
#define EXT2_FIRST_MAP_INDEX                12
#define EXT2_SECOND_MAP_INDEX               13
#define EXT2_THIRD_MAP_INDEX                14
#define EXT2_BLOCK_ADDR_SHIFT               2
#define MAX_DIRENT_NUM                      128

#define DPT_MAX_ENTRY_COUNT                     4
#define DPT_ENTRY_LEN                           16
#define SECTOR_SIZE                             512
#define SECTOR_SHIFT                            9
#define BITS_PER_BYTE                           8


/* 4k data buffer number in each file struct */
// FILE inner buffer
#define LOCAL_DATA_BUF_NUM 4

// fat
#define FAT_BUF_NUM 2
// dir data
#define DIR_DATA_BUF_NUM 4
/**
 * ext2 meta information
 */
struct ext2_base_information {
    u32 ex_base;                            // base sector ID
    u32 ex_first_sb_sect;                   // super block in block group 0
    u32 ex_first_gdt_sect;                  // GDT in group 0
    union {
        u8 *data;
        struct ext2_super_block   *sb;
    } usb;                                   // data    
};

/**
 * ext2 super block
 * in disk
 */
struct ext2_super_block {
    u32                 inode_num;                          // inode��
    u32                 block_num;                          // ����
    u32                 res_block_num;                      // ��������
    u32                 free_block_num;                     // ���п���
    u32                 free_inode_num;                     // ����inode��
    u32                 first_data_block_no;                // ��һ�����ݿ��
    u32                 block_size;                         // �鳤�ȣ���1K��ʼ����λ����
    u32                 slice_size;                         // Ƭ���ȣ���1K��ʼ����λ����
    u32                 blocks_per_group;                   // ÿ�����
    u32                 slices_per_group;                   // ÿ��Ƭ��
    u32                 inodes_per_group;                   // ÿ��indoes��
    u32                 install_time;                       // ��װʱ��
    u32                 last_write_in;                      // ���д��ʱ��
    u16                 install_count;                      // ��װ����
    u16                 max_install_count;                  // ���װ��
    u16                 magic;                              // ħ��
    u16                 state;                              // ״̬
    u16                 err_action;                         // ������
    u16                 edition_change_mark;                // �İ��־
    u32                 last_check;                         // �����ʱ��
    u32                 max_check_interval;                 // �������
    u32                 operating_system;                   // ����ϵͳ
    u32                 edition_mark;                       // �汾��־
    u16                 uid;                                // uid
    u16                 gid;                                // pid
    u32                 first_inode;                        // ��һ���Ǳ�����inode
    u16                 inode_size;                         // inode�Ĵ�С
};

// EXT2 dentry
// in disk
struct ext2_dir_entry {
	u32	                ino;                                // �ļ���inode��
	u16                 rec_len;                            // Ŀ¼��ȣ��ֽڣ�
    u8	                name_len;                           // ���ֳ��ȣ��ֽڣ�
    u8                  file_type;                          // �ļ�����
	char	            name[EXT2_NAME_LEN];                // ����
};

// EXT2 GDT
// in disk
struct ext2_group_desc {
	u32	                block_bitmap;                       // ��λͼ���ڿ�
	u32	                inode_bitmap;                       // inodeλͼ���ڿ�
	u32	                inode_table;                        // inode�б����ڿ�
	u16	                free_blocks_count;                  // ���п���
	u16	                free_inodes_count;                  // ���нڵ���
	u16	                used_dirs_count;                    // Ŀ¼��
	u16	                pad;                                // ���¾�Ϊ����
	u32	                reserved[3];
};

// EXT2 inode
// in disk
struct ext2_inode {
	u16	                i_mode;                             // �ļ�ģʽ
	u16	                i_uid;                              // UID�ĵ�16λ
	u32	                i_size;                             // �ļ���С���ֽ�����
	u32	                i_atime;                            // �������ʱ��
	u32	                i_ctime;                            // ����ʱ��
	u32	                i_mtime;                            // �޸�ʱ��
	u32	                i_dtime;                            // ɾ��ʱ��
	u16	                i_gid;                              // GID�ĵ�16λ
	u16	                i_links_count;                      // ���Ӽ���
	u32	                i_blocks;                           // �����Ŀ���
	u32	                i_flags;                            // �򿪵ı��
	u32                 osd1;                               // �����ϵͳ���1
	u32	                i_block[EXT2_N_BLOCKS];             // ���������صĿ��ַ
	u32	                i_generation;                       // ��NFS�ã��ļ��İ汾
	u32	                i_file_acl;                         // �ļ���ACL
	u32	                i_dir_acl;                          // Ŀ¼��ACL
    u32	                i_faddr;                            // ��Ƭ��ַ
    u32                 osd2[3];                            // �����ϵͳ���2
};


/* ext2 file struct */
typedef struct ext2_file {
    unsigned char path[256];
    /* Current file pointer */
    unsigned long loc;
    /* Current directory entry position */
    unsigned long dir_entry_pos;
    unsigned long dir_entry_sector;
    /* current directory entry */
    struct ext2_dir_entry dir_entry;
    struct ext2_inode inode;
    /* Buffer clock head */
    unsigned long clock_head;
    /* For normal FAT32, cluster size is 4k */
    BUF_4K data_buf[LOCAL_DATA_BUF_NUM];
} EXT2_FILE_STRUCT;

#define FILE_EXT2 FILE

unsigned long fs_find_ext2(FILE_EXT2 *file);

unsigned long init_fs_ext2();

unsigned long fs_open_ext2(FILE_EXT2 *file, unsigned char *filename);

unsigned long fs_close_ext2(FILE_EXT2 *file);

unsigned long fs_read_ext2(FILE_EXT2 *file, unsigned char *buf, unsigned long count);

unsigned long fs_write_ext2(FILE_EXT2 *file, const unsigned char *buf, unsigned long count);

unsigned long fs_fflush_ext2();

void fs_lseek_ext2(FILE_EXT2 *file, unsigned long new_loc);

// usr.c
unsigned long fs_create_ext2(unsigned char *filename);

unsigned long fs_mkdir_ext2(unsigned char *filename);

unsigned long fs_rm_ext2(unsigned char *filename);

unsigned long fs_mv_ext2(unsigned char *src, unsigned char *dest);

unsigned long fs_cat_ext2(unsigned char *path);

unsigned long fs_dump_ext2();

// ext2_utisl.c
u32 get_entry_filesize_ext2(u8 *entry);

u32 get_entry_attr_ext2(u8 *entry);

// utils.c
u16 get_u16(u8 *ch);
u32 get_u32(u8 *ch);
void set_u16(u8 *ch, u16 num);
void set_u32(u8 *ch, u32 num);
u32 fs_wa(u32 num);
u32 fs_next_slash(u8 *f, u8 *output11);

#endif