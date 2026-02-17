#ifndef AVL_MAP_H
#define AVL_MAP_H

#include <stddef.h>

struct avl_map_node {
	size_t avl_key;
	size_t avl_height;
	struct avl_tree_node *avl_parent;
	struct avl_tree_node *avl_left, *avl_right;
};

struct avl_map_root {
	struct avl_tree_node *avl_root;
};

void insert(struct avl_map_root *const root, struct avl_map_node *const new);

#endif
