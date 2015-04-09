/** File:		main.c
 ** Author:		Dongli Zhang
 ** Contact:	dongli.zhang0129@gmail.com
 **
 ** Copyright (C) Dongli Zhang 2013
 **
 ** This program is free software;  you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation; either version 2 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY;  without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 ** the GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program;  if not, write to the Free Software 
 ** Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/* run dmesg to see the list */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/cdev.h>

#include <linux/crypto.h>
#include <crypto/hash.h>
#include <linux/scatterlist.h>

#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>

#define SHA1_LENGTH 20
#define MAX_PWD 32
#define AES_KEY_SIZE 32
#define AES_BLOCK_SIZE 16

void dump(char *str, char *ptr)
{
	int i;
	
	printk("%s", str);
	for (i = 0; i < 32; i++)
	{
		printk("%02x ", (unsigned char)ptr[i]);
	}
	printk("\n");
}

// given a string, generate a 32-bit key
int generate_key(char *pwd, u8 *pkey)
{
	int len_pwd = strlen(pwd);
	struct scatterlist sg;
	struct crypto_hash *tfm;
	struct hash_desc desc;
	int i;

	unsigned char output[SHA1_LENGTH];  // key generated
	char *buf = kmalloc(MAX_PWD, GFP_KERNEL); // password buffer
	memset(buf, 0, MAX_PWD);
	strncpy(buf, pwd, len_pwd);

	tfm = crypto_alloc_hash("sha1", 1, CRYPTO_ALG_ASYNC);
	desc.tfm = tfm;
	desc.flags = 0;

	sg_init_one(&sg, buf, len_pwd);
	crypto_hash_init(&desc);

	crypto_hash_update(&desc, &sg, len_pwd);
	crypto_hash_final(&desc, output);

	for(i=0; i<16; i++)
		pkey[i] = output[i];

	for(i=0; i<16; i++)
		pkey[i+16] = output[i];

	crypto_free_hash(tfm);
	kfree(buf);

	return 0;
}

void aes_encrypt(u8 *pdata, u8 *cdata, u8 *ndata, u8 *key)
{
	struct crypto_cipher *tfm;

	tfm = crypto_alloc_cipher("aes", 4, CRYPTO_ALG_ASYNC);

	crypto_cipher_encrypt_one(tfm, &cdata[0], &pdata[0]);
	crypto_cipher_encrypt_one(tfm, &cdata[16], &pdata[16]);

	dump("PlainText: ", pdata);
	dump("Crypted:   ", cdata);

	crypto_cipher_decrypt_one(tfm, &ndata[0], &cdata[0]);
	crypto_cipher_decrypt_one(tfm, &ndata[16], &cdata[16]);

	dump("Decrypted: ", ndata);
	
	crypto_free_cipher(tfm);

	return;
}

static int __init crypto_test_init(void)
{
	char msg[] = "stonybrook";
	char ptext[] = "12345678123456781234567812345678";
	char ctext[32];
	char ntext[32];
	u8 key[32];
	//char *key = "justice";
	
	generate_key(msg, key);
	aes_encrypt((u8 *)ptext, (u8 *)ctext, (u8 *)ntext, key);
	
	printk(KERN_ALERT "init the module\n");
	return 0;
}

static void __exit crypto_test_exit(void)
{
	printk(KERN_ALERT "exit the module\n");
}

module_init(crypto_test_init);
module_exit(crypto_test_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Test Module");
MODULE_AUTHOR("Dongli");
