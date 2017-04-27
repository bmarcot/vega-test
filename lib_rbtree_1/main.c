/* test RB-tree library */

#include <stddef.h>
#include <kernel/kernel.h>
#include <lib/rbtree.h>
#include "unit.h"

#define NODE_COUNT 300

struct my_struct {
	int            val;
	struct rb_node node;
};

int my_insert(struct rb_tree *tree, struct my_struct *x)
{
	struct rb_node *parent;

	INIT_RB_NODE(&x->node, tree);

	if (tree->root == &tree->nil) {
		tree->root = &x->node;
		return 0;
	}

	/* get last node before NIL */
	for (struct rb_node *n = tree->root; n != &tree->nil;) {
		struct my_struct *this = container_of(n, struct my_struct, node);
		parent = n;
		if (x->val < this->val)
			n = n->left;
		else
			n = n->right;
	}
	rb_set_parent(&x->node, parent);
	if (x->val < container_of(parent, struct my_struct, node)->val)
		parent->left = &x->node;
	else
		parent->right = &x->node;
	rb_insert_fixup(tree, &x->node);

	return 0;
}

void print_tree_aux(struct rb_tree *tree, struct rb_node *x)
{
	if (x == &tree->nil)
		return;

	struct my_struct *this = container_of(x, struct my_struct, node);
	int left_val, right_val;

	if (x->left == &tree->nil)
		left_val = -1;
	else
		left_val = container_of(x->left, struct my_struct, node)->val;
	if (x->right == &tree->nil)
		right_val = -1;
	else
		right_val = container_of(x->right, struct my_struct, node)->val;

	printk("on node %p %d %d %d\t%s\n", x, this->val, left_val, right_val,
		RB_COLOR(x) == RB_BLACK ? "[black]" : "[red]");
	print_tree_aux(tree, x->left);
	print_tree_aux(tree, x->right);
}

void print_tree(struct rb_tree *tree)
{
	print_tree_aux(tree, tree->root);
}

int check_tree_rec(struct rb_tree *tree, struct rb_node *x)
{
	struct my_struct *this = container_of(x, struct my_struct, node);
	int val;

	if ((x->left != &tree->nil)
		&& (this->val < (val = check_tree_rec(tree, x->left)))) {
		printk("%d is not ge %d\n", this->val, val);
		TEST_EXIT(1);
	}
	if ((x->right != &tree->nil)
		&& (this->val >= (val = check_tree_rec(tree, x->right)))) {
		printk("%d is not lt %d\n", this->val, val);
		TEST_EXIT(1);
	}

	return this->val;
}

int check_tree(struct rb_tree *tree)
{
	return check_tree_rec(tree, tree->root);
}

int count_nodes_rec(struct rb_tree *tree, struct rb_node *x)
{
	if (x == &tree->nil)
		return 0;

	return 1 + count_nodes_rec(tree, x->left)
		+ count_nodes_rec(tree, x->right);
}

int count_nodes(struct rb_tree *tree)
{
	return count_nodes_rec(tree, tree->root);
}

static int simple_rand(void)
{
	int a = 16807;
	int m = 2147483647;
	static int seed = 997;

	seed = (a * seed) % m;

	return seed;
}

int main()
{
	static struct my_struct ms[NODE_COUNT];
	RB_TREE(tree);

	printk("Checking ordered insertion...\n");
	for (int i = 0; i < NODE_COUNT; i++) {
		ms[i].val = i;
		my_insert(&tree, &ms[i]);
	}
	check_tree(&tree);
	if (count_nodes(&tree) != NODE_COUNT) {
		printk("error: Counted %d nodes, expected %d\n",
			count_nodes(&tree), NODE_COUNT);
		TEST_EXIT(1);
	}

	printk("Checking deletion...\n");
	for (int i = 0; i < NODE_COUNT; i++) {
		rb_delete(&tree, &ms[i].node);
		if (count_nodes(&tree) != NODE_COUNT - 1 - i) {
			printk("error: Wrong number of nodes\n");
			TEST_EXIT(1);
		}
	}

	printk("Checking unordered insertion...\n");
	INIT_RB_TREE(&tree);  /* reinit the RB-tree */
	for (int i = 0; i < NODE_COUNT; i++) {
		ms[i].val = simple_rand();
		my_insert(&tree, &ms[i]);
	}
	check_tree(&tree);
	/* print_tree(&tree); */
	if (count_nodes(&tree) != NODE_COUNT) {
		printk("error: Counted %d nodes, expected %d\n",
			count_nodes(&tree), NODE_COUNT);
		TEST_EXIT(1);
	}

	printk("Checking deletion...\n");
	for (int i = 0; i < NODE_COUNT; i++) {
		rb_delete(&tree, &ms[i].node);
		if (count_nodes(&tree) != NODE_COUNT - 1 - i) {
			printk("error: Wrong number of nodes\n");
			TEST_EXIT(1);
		}
	}

	TEST_EXIT(0);
}
