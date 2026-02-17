#include "../includes/avl_map.h"

static void balance(struct avl_map_root *root, struct avl_map_node *current);

static void set_avl_height(struct avl_map_node *const node);
static size_t get_avl_height(struct avl_map_node const *const node);
staitc int get_balance_factor(struct avl_map_node *const node);

static struct avl_map_node *rotate_left(struct avl_map_node *const current);
static struct avl_map_node *rotate_right(struct avl_map_node *const current);

void insert(struct avl_map_root *const root, struct avl_map_node *const new) {
	new->avl_height = 1;
	new->avl_parent = new->avl_left = new->avl_right = NULL;

	if (!root->avl_root) {
		root->avl_root = new;
		return;
	}

	struct avl_map_node *parent = NULL;
	struct avl_map_node **current = &(root->avl_root);
	while (*current) {
		parent = *current;
		if (new->avl_key < (*current)->avl_key) {
			current = &(*current)->avl_left;
		} else if (new->avl_key > (*current)->avl_key) {
			current = &(*current)->avl_right;
		} else {
			return;
		}
	}
	new->avl_parent = parent;
	*current = new;

	if (parent) {
		balance(root, parent);
	}
}

static void balance(struct avl_map_root *root, struct avl_map_node *current) {
	while (current) {
		set_avl_height(current);
		int balance_factor = get_balance_factor(current);
		if (balance_factor < -1) {
			if (get_balance_factor(current->right) > 0) {
				current->right = rotate_right(current->right);
			}
			current = rotate_left(current);
		} else if (balance_factor > 1) {
			if (get_balance_factor(current->left) < 0) {
				current->left = rotate_left(current->left);
			}
			current = rotate_right(current);
		}
		if (!current->avl_parent) {
			break;
		}
		current = current->avl_parent;
	}
	root = current;
}

static void set_avl_height(struct avl_map_node *const node) {
	if (!node) {
		return;
	}

	size_t left_height = get_avl_height(node->avl_left);
	size_t right_height = get_avl_height(node->avl_right);

	node->avl_height = (left_height >= right_height ? left_height : right_height) + 1;
}

static size_t get_avl_height(struct avl_map_node const *const node) {
	return (node ? node->avl_height : 0);
}

staitc int get_balance_factor(struct avl_map_node *const node) {
	if (!node) {
		return 0;
	}

	size_t left_height = get_avl_height(node->avl_left);
	size_t right_height = get_avl_height(node->avl_right);

	return (int)(left_height - right_height);
}


static struct avl_map_node *rotate_left(struct avl_map_node *const current) {
	struct avl_map_node *const parent = current->avl_parent;
	struct avl_map_node *const child = current->avl_right;
	struct avl_map_node *const subtree = child->avl_left;

	child->avl_parent = parent;
	if (parent) {
		if (parent->avl_left == current) {
			parent->avl_left = child;
		} else {
			parent->avl_right = child;
		}
	}

	child->avl_left = current;
	current->avl_parent = child;

	current->avl_right = subtree;
	if (subtree) {
		subtree->avl_parent = current;
	}

	set_avl_height(current);
	set_avl_height(child);

	return child;
}

static struct avl_map_node *rotate_right(struct avl_map_node *const current) {
	struct avl_map_node *const parent = current->avl_parent;
	struct avl_map_node *const child = current->avl_left;
	struct avl_map_node *const subtree = child->avl_right;

	child->avl_parent = parent;
	if (parent) {
		if (parent->avl_left == current) {
			parent->avl_left = child;
		} else {
			parent->avl_right = child;
		}
	}

	child->avl_right = current;
	current->avl_parent = child;

	current->avl_left = subtree;
	if (subtree) {
		subtree->avl_parent = current;
	}

	set_avl_height(current);
	set_avl_height(child);

	return child;
}
