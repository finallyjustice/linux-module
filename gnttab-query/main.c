/*
 * This is based on 4.1.12-124.37.1.el6uek.x86_64.
 *
 * We will have below in syslog:
 *
 * [ 3656.172132] orabug(0): dom=32752, nr_frames=17, max_nr_frames=256, status=0
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <xen/interface/grant_table.h>
#include <asm/xen/hypercall.h>

static int __init gnttab_query_init(void)
{
        struct gnttab_query_size query;
        int rc;

        query.dom = DOMID_SELF;
        rc = HYPERVISOR_grant_table_op(GNTTABOP_query_size, &query, 1);

        pr_alert("orabug(%d): dom=%u, nr_frames=%u, max_nr_frames=%u, status=%d\n",
                 rc, query.dom, query.nr_frames, query.max_nr_frames, query.status);

        return 0;
}

static void __exit gnttab_query_exit(void)
{
}

module_init(gnttab_query_init);
module_exit(gnttab_query_exit);

MODULE_LICENSE("GPL");
