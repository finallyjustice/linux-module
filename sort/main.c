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
#include <linux/sort.h>

static int compare(const void *lval, const void *rval)
{
	u16 lv = * (const u16 *) lval;
	u16 rv = * (const u16 *) rval;

	if (lv < rv) return -1;
	if (lv > rv) return 1;

	return 0;
}

static int __init sort_test_init(void)
{
	int i;
	u16 val[10] = {4, 7, 2, 1, 3, 8, 9, 10, 5, 6};

	sort(val, 10, sizeof(u16), &compare, NULL);

	for (i = 0; i < 10; i++)
		pr_alert("%d\n", val[i]);

	return 0;
}

static void __exit sort_test_exit(void)
{
}

module_init(sort_test_init);
module_exit(sort_test_exit);
