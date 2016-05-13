/*
 * Copyright(c) 2016 Hauke Mehrtens <hauke@hauke-m.de>
 *
 * Backport functionality introduced in Linux 4.7.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/export.h>
#include <linux/list.h>
#include <linux/rcupdate.h>
#include <linux/rhashtable.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/skbuff.h>
#include <net/netlink.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,1,0)
/**
 * rhashtable_walk_init - Initialise an iterator
 * @ht:		Table to walk over
 * @iter:	Hash table Iterator
 * @gfp:	GFP flags for allocations
 *
 * This function prepares a hash table walk.
 *
 * Note that if you restart a walk after rhashtable_walk_stop you
 * may see the same object twice.  Also, you may miss objects if
 * there are removals in between rhashtable_walk_stop and the next
 * call to rhashtable_walk_start.
 *
 * For a completely stable walk you should construct your own data
 * structure outside the hash table.
 *
 * This function may sleep so you must not call it from interrupt
 * context or with spin locks held.
 *
 * You must call rhashtable_walk_exit if this function returns
 * successfully.
 */
int rhashtable_walk_init(struct rhashtable *ht, struct rhashtable_iter *iter,
			 gfp_t gfp)
{
	iter->ht = ht;
	iter->p = NULL;
	iter->slot = 0;
	iter->skip = 0;

	iter->walker = kmalloc(sizeof(*iter->walker), gfp);
	if (!iter->walker)
		return -ENOMEM;

	spin_lock(&ht->lock);
	iter->walker->tbl =
		rcu_dereference_protected(ht->tbl, lockdep_is_held(&ht->lock));
	list_add(&iter->walker->list, &iter->walker->tbl->walkers);
	spin_unlock(&ht->lock);

	return 0;
}
EXPORT_SYMBOL_GPL(rhashtable_walk_init);
#endif /* >= 4.1 */

/**
 * __nla_reserve_64bit - reserve room for attribute on the skb and align it
 * @skb: socket buffer to reserve room on
 * @attrtype: attribute type
 * @attrlen: length of attribute payload
 *
 * Adds a netlink attribute header to a socket buffer and reserves
 * room for the payload but does not copy it. It also ensure that this
 * attribute will be 64-bit aign.
 *
 * The caller is responsible to ensure that the skb provides enough
 * tailroom for the attribute header and payload.
 */
struct nlattr *__nla_reserve_64bit(struct sk_buff *skb, int attrtype,
                                  int attrlen, int padattr)
{
       if (nla_need_padding_for_64bit(skb))
               nla_align_64bit(skb, padattr);

       return __nla_reserve(skb, attrtype, attrlen);
}
EXPORT_SYMBOL_GPL(__nla_reserve_64bit);

/**
 * nla_reserve_64bit - reserve room for attribute on the skb and align it
 * @skb: socket buffer to reserve room on
 * @attrtype: attribute type
 * @attrlen: length of attribute payload
 *
 * Adds a netlink attribute header to a socket buffer and reserves
 * room for the payload but does not copy it. It also ensure that this
 * attribute will be 64-bit aign.
 *
 * Returns NULL if the tailroom of the skb is insufficient to store
 * the attribute header and payload.
 */
struct nlattr *nla_reserve_64bit(struct sk_buff *skb, int attrtype, int attrlen,
				 int padattr)
{
       size_t len;

       if (nla_need_padding_for_64bit(skb))
               len = nla_total_size_64bit(attrlen);
       else
               len = nla_total_size(attrlen);
       if (unlikely(skb_tailroom(skb) < len))
               return NULL;

       return __nla_reserve_64bit(skb, attrtype, attrlen, padattr);
}
EXPORT_SYMBOL_GPL(nla_reserve_64bit);

/**
 * __nla_put_64bit - Add a netlink attribute to a socket buffer and align it
 * @skb: socket buffer to add attribute to
 * @attrtype: attribute type
 * @attrlen: length of attribute payload
 * @data: head of attribute payload
 *
 * The caller is responsible to ensure that the skb provides enough
 * tailroom for the attribute header and payload.
 */
void __nla_put_64bit(struct sk_buff *skb, int attrtype, int attrlen,
                    const void *data, int padattr)
{
       struct nlattr *nla;

       nla = __nla_reserve_64bit(skb, attrtype, attrlen, padattr);
       memcpy(nla_data(nla), data, attrlen);
}
EXPORT_SYMBOL_GPL(__nla_put_64bit);

/**
 * nla_put_64bit - Add a netlink attribute to a socket buffer and align it
 * @skb: socket buffer to add attribute to
 * @attrtype: attribute type
 * @attrtype: attribute type
 * @attrlen: length of attribute payload
 * @data: head of attribute payload
 *
 * Returns -EMSGSIZE if the tailroom of the skb is insufficient to store
 * the attribute header and payload.
 */
int nla_put_64bit(struct sk_buff *skb, int attrtype, int attrlen,
                 const void *data, int padattr)
{
       size_t len;

       if (nla_need_padding_for_64bit(skb))
               len = nla_total_size_64bit(attrlen);
       else
               len = nla_total_size(attrlen);
       if (unlikely(skb_tailroom(skb) < len))
               return -EMSGSIZE;

       __nla_put_64bit(skb, attrtype, attrlen, data, padattr);
       return 0;
}
EXPORT_SYMBOL_GPL(nla_put_64bit);
