#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_KEYS 4 // Maximum number of keys in a node

typedef struct bnode {
    int keys[MAX_KEYS - 1];
    struct bnode* children[MAX_KEYS];
    int num_keys;
    int leaf;
} BNode;

BNode* btree_create_node() {
    BNode* new_node = (BNode*)malloc(sizeof(BNode));
    if (new_node != NULL) {
        new_node->num_keys = 0;
        new_node->leaf = 1;
        return new_node;
    } else {
        // Handle allocation failure
        printf("Memory allocation failed for B-tree node\n");
        exit(EXIT_FAILURE);
    }
}


void btree_split_child(BNode* parent, int index, BNode* child) {
    BNode* new_child = btree_create_node();
    if (new_child != NULL) {
        new_child->leaf = child->leaf;
        new_child->num_keys = MAX_KEYS / 2 - 1;

        // Copy keys to the new child
        for (int i = 0; i < MAX_KEYS / 2 - 1; i++) {
            new_child->keys[i] = child->keys[i + MAX_KEYS / 2];
        }

        if (!child->leaf) {
            // Copy children to the new child
            for (int i = 0; i < MAX_KEYS / 2; i++) {
                new_child->children[i] = child->children[i + MAX_KEYS / 2];
            }
        }

        child->num_keys = MAX_KEYS / 2 - 1;

        // Move keys in the parent to make space for the new child
        for (int i = parent->num_keys; i > index; i--) {
            parent->children[i + 1] = parent->children[i];
        }

        parent->children[index + 1] = new_child;

        for (int i = parent->num_keys - 1; i >= index; i--) {
            parent->keys[i + 1] = parent->keys[i];
        }

        parent->keys[index] = child->keys[MAX_KEYS / 2 - 1];
        parent->num_keys++;
    } else {
        // Handle allocation failure
        printf("Memory allocation failed for new B-tree child\n");
        exit(EXIT_FAILURE);
    }
}   

void btree_insert_non_full(BNode* node, int key) {
    int i = node->num_keys - 1;

    if (node->leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;

        if (node->children[i]->num_keys == MAX_KEYS - 1) {
            btree_split_child(node, i, node->children[i]);
            if (key > node->keys[i]) {
                i++;
            }
        }
        btree_insert_non_full(node->children[i], key);
    }
}

void btree_insert(BNode** root, int key) {
    BNode* r = *root;
    if (r->num_keys == MAX_KEYS - 1) {
        BNode* new_root = btree_create_node();
        if (new_root != NULL) {
            new_root->leaf = 0;
            new_root->children[0] = r;
            *root = new_root;
            btree_split_child(new_root, 0, r);
            btree_insert_non_full(new_root, key);
        } else {
            // Handle allocation failure
            printf("Memory allocation failed for new B-tree root\n");
            exit(EXIT_FAILURE);
        }
    } else {
        btree_insert_non_full(r, key);
    }
}


BNode* btree_search(BNode* node, int key) {
    int i = 0;
    while (i < node->num_keys && key > node->keys[i]) {
        i++;
    }
    if (i < node->num_keys && key == node->keys[i]) {
        return node;
    }
    if (node->leaf) {
        return NULL;
    }
    return btree_search(node->children[i], key);
}

void btree_inorder_traversal(BNode* node) {
    if (node) {
        int i;
        for (i = 0; i < node->num_keys; i++) {
            if (!node->leaf) {
                btree_inorder_traversal(node->children[i]);
            }
            printf("%d ", node->keys[i]);
        }
        if (!node->leaf) {
            btree_inorder_traversal(node->children[i]);
        }
    }
}

void btree_delete_node(BNode* node, int key);

void btree_delete_from_leaf(BNode* node, int key) {
    int i = 0;
    while (i < node->num_keys && key != node->keys[i]) {
        i++;
    }
    if (i == node->num_keys) {
        printf("Key %d not found\n", key);
        return;
    }
    for (int j = i; j < node->num_keys - 1; j++) {
        node->keys[j] = node->keys[j + 1];
    }
    node->num_keys--;
}

void btree_delete_from_non_leaf(BNode* node, int key, int index) {
    int k = node->keys[index];
    if (node->children[index]->num_keys >= MAX_KEYS / 2) {
        // Move key from the child's predecessor
        BNode* pred = node->children[index];
        while (!pred->leaf) {
            pred = pred->children[pred->num_keys];
        }
        node->keys[index] = pred->keys[pred->num_keys - 1];
        btree_delete_node(node->children[index], pred->keys[pred->num_keys - 1]);
    } else if (node->children[index + 1]->num_keys >= MAX_KEYS / 2) {
        // Move key from the child's successor
        BNode* succ = node->children[index + 1];
        while (!succ->leaf) {
            succ = succ->children[0];
        }
        node->keys[index] = succ->keys[0];
        btree_delete_node(node->children[index + 1], succ->keys[0]);
    } else {
        // Merge the child with its sibling
        BNode* child = node->children[index];
        BNode* sibling = node->children[index + 1];
        child->keys[child->num_keys] = k;
        for (int i = 0; i < sibling->num_keys; i++) {
            child->keys[child->num_keys + 1 + i] = sibling->keys[i];
        }
        if (!child->leaf) {
            for (int i = 0; i <= sibling->num_keys; i++) {
                child->children[child->num_keys + 1 + i] = sibling->children[i];
            }
        }
        child->num_keys += sibling->num_keys + 1;
        // Remove key from parent and shift keys and children accordingly
        for (int i = index; i < node->num_keys - 1; i++) {
            node->keys[i] = node->keys[i + 1];
        }
        for (int i = index + 1; i < node->num_keys; i++) {
            node->children[i] = node->children[i + 1];
        }
        node->num_keys--;
        free(sibling);
        // Recursively delete the key from the merged child
        btree_delete_node(child, key);
    }
}

void btree_delete_node(BNode* node, int key) {
    int index = 0;
    // Find the index of the key to delete
    while (index < node->num_keys && key > node->keys[index]) {
        index++;
    }
    // If the key is found in this node
    if (index < node->num_keys && key == node->keys[index]) {
        if (node->leaf) {
            // Key found in a leaf node, delete it
            for (int i = index; i < node->num_keys - 1; i++) {
                node->keys[i] = node->keys[i + 1];
            }
            node->num_keys--;
        } else {
            // Key found in a non-leaf node, delete it from appropriate child
            btree_delete_from_non_leaf(node, key, index);
        }
    } else {
        // If the key is not found in this node
        if (node->leaf) {
            // Key not found in a leaf node
            printf("Key %d not found\n", key);
            return;
        }
        int flag = (index == node->num_keys) ? 1 : 0;
     
        btree_delete_node(node->children[index], key);
    }
}

int btreeHeight(BNode* root) {
    if (root == NULL) {
        return 0;
    } else {
        int height = 0;
        for (int i = 0; i < root->num_keys + 1; ++i) {
            int child_height = btreeHeight(root->children[i]);
            if (child_height > height) {
                height = child_height;
            }
        }
        return height + 1;
    }
}

void printGivenLevelOrderBTree(BNode* root, int level) {
    if (root == NULL)
        return;
    if (level == 1) {
        for (int i = 0; i < root->num_keys; ++i)
            printf("%d ", root->keys[i]);
    } else if (level > 1) {
        for (int i = 0; i < root->num_keys + 1; ++i)
            printGivenLevelOrderBTree(root->children[i], level - 1);
    }
}

void printBTree(BNode* root, bool diamond) {
    if (root == NULL) {
        return;
    }
    int h = btreeHeight(root);
    int i, j;
    for (i = 1; i <= h; i++) {
        for (j = 0; j < h - i; j++) {
            printf("  ");
        }
        printGivenLevelOrderBTree(root, i);
        printf("\n");
    }
}
