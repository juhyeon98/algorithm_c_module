#include "../includes/avl_map.h"

static void balance(struct avl_map_root *root, struct avl_map_node *current);

static void set_avl_height(struct avl_map_node *const node);
static size_t get_avl_height(struct avl_map_node const *const node);
static int get_balance_factor(struct avl_map_node *const node);

static void change_parent(struct avl_map_node *target, struct avl_map_node *child);
static struct avl_map_node *exchange_left_tree(struct avl_map_node *target);
static struct avl_map_node *exchange_right_tree(struct avl_map_node *target);

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

struct avl_map_node *remove(struct avl_map_root *const root, int const key) {
	struct avl_map_node *target = search(root, key);
	if (!target) {
		return NULL;
	}

	struct avl_map_node *const parent = target->avl_parent;
	if (target->avl_left == NULL && target->avl_right == NULL) {
		change_parent(target, NULL);
	} else if (target->avl_left == NULL) {
		target->avl_right->avl_parent = parent;
		change_parent(target, target->avl_right);
	} else if (target->avl_right == NULL) {
		target->avl_left->avl_parent = parent;
		change_parent(target, target->avl_left);
	} else {
		int const balance_factor = get_balance_factor(target);
		balance_point = (balance_factor < 0 ? \
				exchange_right_tree(target) : exchange_left_tree(target) );
	}
	balance(root, balance_point);

	target->avl_parent = target->avl_left = target->avl_right = NULL;
	return target;
}

struct avl_map_node *search(struct avl_map_root const *const root, int const key) {
	struct avl_map_node *target = root->avl_root;
	while (target) {
		if (key < target->avl_key) {
			target = target->avl_left;
		} else if (key > target->avl_key) {
			target = target->avl_right;
		} else {
			break;
		}
	}
	return target;
}

static void balance(struct avl_map_root *root, struct avl_map_node *current) {
	while (current) {
		set_avl_height(current);
		int balance_factor = get_balance_factor(current);
		if (balance_factor < -1) {
			if (get_balance_factor(current->avl_right) > 0) {
				current->avl_right = rotate_right(current->avl_right);
			}
			current = rotate_left(current);
		} else if (balance_factor > 1) {
			if (get_balance_factor(current->avl_left) < 0) {
				current->avl_left = rotate_left(current->avl_left);
			}
			current = rotate_right(current);
		}
		if (!current->avl_parent) {
			break;
		}
		current = current->avl_parent;
	}
	root->avl_root = current;
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

static int get_balance_factor(struct avl_map_node *const node) {
	if (!node) {
		return 0;
	}
	return (int)(get_avl_height(node->avl_left) - get_avl_height(node->avl_right));
}

static void change_parent(struct avl_map_node *target, struct avl_map_node *child) {
	if (target->avl_parent) {
		if (target->avl_parent->avl_left == target) {
			target->avl_parent->avl_left = child;
		} else {
			target->avl_parent->avl_right = child;
		}
	}
}

static struct avl_map_node *exchange_left_tree(struct avl_map_node *target) {
	struct avl_map_node *const parent = target->avl_parent;
	struct avl_map_node *to_exchange = target->avl_left;
	struct avl_map_node *exchange_parent = target;

	while (to_exchange->avl_right) {
		exchange_parent = to_exchange;
		to_exchange = to_exchange->avl_right;
	}

	to_exchange->avl_parent = parent;
	change_parent(target, to_exchange);

	to_exchange->avl_right = target->avl_right;
	if (target->avl_right) {
		target->avl_right->avl_parent = to_exchange;
	}

	if (target->avl_left != to_exchange) {
		exchange_parent->avl_right = to_exchange->avl_left;
		if (to_exchange->avl_left) {
			to_exchange->avl_left->avl_parent = exchange_parent;
		}

		to_exchange->avl_left = target->avl_left;
		if (target->avl_left) {
			target->avl_left->avl_parent = to_exchange;
		}
	} else {
		return to_exchange;
	}

	return exchange_parent;
}

static struct avl_map_node *exchange_right_tree(struct avl_map_node *target) {
	struct avl_map_node *to_exchange = target->avl_right;
	struct avl_map_node *exchange_parent = target;

	while (to_exchange->avl_left) {
		exchange_parent = to_exchange;
		to_exchange = to_exchange->avl_left;
	}

	to_exchange->avl_parent = target->avl_parent;
	change_parent(target, to_exchange);

	to_exchange->avl_left = target->avl_left;
	if (target->avl_left) {
		target->avl_left->avl_parent = to_exchange;
	}

	if (target->avl_right != to_exchange) {
		exchange_parent->avl_left = to_exchange->avl_right;
		if (to_exchange->avl_right) {
			to_exchange->avl_right->avl_parent = exchange_parent;
		}

		to_exchange->avl_right = target->avl_right;
		if (target->avl_right) {
			target->avl_right->avl_parent = to_exchange;
		}
	} else {
		return to_exchange;
	}

	return exchange_parent;
}

static struct avl_map_node *rotate_left(struct avl_map_node *const current) {
	struct avl_map_node *const child = current->avl_right;
	struct avl_map_node *const subtree = child->avl_left;

	child->avl_parent = current->avl_parent;
	change_parent(current, child);

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
	struct avl_map_node *const child = current->avl_left;
	struct avl_map_node *const subtree = child->avl_right;

	child->avl_parent = current->avl_parent;
	change_parent(current, child);

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
