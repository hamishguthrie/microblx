/*
 * uthash stuff
 *
 * SPDX-License-Identifier: BSD-1-Clause
 */

typedef struct UT_hash_bucket {
	struct UT_hash_handle *hh_head;
	unsigned int count;

	/*
	 * expand_mult is normally set to 0. In this situation, the max chain
	 * length threshold is enforced at its default value,
	 * HASH_BKT_CAPACITY_THRESH. (If the bucket's chain exceeds this
	 * length, bucket expansion is triggered).  However, setting
	 * expand_mult to a non-zero value delays bucket expansion (that would
	 * be triggered by additions to this particular bucket) until its chain
	 * length reaches a *multiple* of HASH_BKT_CAPACITY_THRESH. (The
	 * multiplier is simply expand_mult+1). The whole idea of this
	 * multiplier is to reduce bucket expansions, since they are expensive,
	 * in situations where we know that a particular bucket tends to be
	 * overused. It is better to let its chain length grow to a longer
	 * yet-still-bounded value, than to do an O(n) bucket expansion too
	 * often.
	 */
	unsigned int expand_mult;

} UT_hash_bucket;

typedef struct UT_hash_table {
	UT_hash_bucket *buckets;
	unsigned int num_buckets, log2_num_buckets;
	unsigned int num_items;
	/* tail hh in app order, for fast append */
	struct UT_hash_handle *tail;
	/* hash handle offset (byte pos of hash handle in element */
	ptrdiff_t hho;
	/*
	 * in an ideal situation (all buckets used equally), no bucket would
	 * have more than ceil(#items/#buckets) items. that's the ideal chain
	 * length.
	 */
	unsigned int ideal_chain_maxlen;

	/*
	 * nonideal_items is the number of items in the hash whose chain
	 * position exceeds the ideal chain maxlen. these items pay the
	 * penalty for an uneven hash distribution; reaching them in a chain
	 * traversal takes >ideal steps
	 */
	unsigned int nonideal_items;

	/*
	 * ineffective expands occur when a bucket doubling was performed, but
	 * afterward, more than half the items in the hash had nonideal chain
	 * positions. If this happens on two consecutive expansions we inhibit
	 * any further expansion, as it's not helping; this happens when the
	 * hash function isn't a good fit for the key domain. When expansion
	 * is inhibited the hash will still work, albeit no longer in constant
	 * time.
	 */
	unsigned int ineff_expands, noexpand;
	/* used only to find hash tables in external analysis */
	uint32_t signature;

} UT_hash_table;

typedef struct UT_hash_handle {
	struct UT_hash_table *tbl;
	void *prev;                       /* prev element in app order */
	void *next;                       /* next element in app order */
	struct UT_hash_handle *hh_prev;   /* previous hh in bucket order */
	struct UT_hash_handle *hh_next;   /* next hh in bucket order */
	void *key;                        /* ptr to enclosing struct's key */
	unsigned int keylen;              /* enclosing struct's key len */
	unsigned int hashv;               /* result of hash-fcn(key) */
} UT_hash_handle;

