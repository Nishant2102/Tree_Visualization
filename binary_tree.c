#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure for a node of the binary tree
struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
};
// Function to create a new node with the given data
struct TreeNode* createNode(int data) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to insert a new node with the given data into a binary tree
struct TreeNode* insertBinaryNode(struct TreeNode* root, int data) {
    // If the tree is empty, return a new node
    if (root == NULL) {
        return createNode(data);
    }
    // Otherwise, recur down the tree
    if (data < root->data) {
        root->left = insertBinaryNode(root->left, data);
    } else if (data > root->data) {
        root->right = insertBinaryNode(root->right, data);
    }
    // Return the unchanged node pointer
    return root;
}

// Function to perform inorder traversal of the binary tree
void binaryInorderTraversal(struct TreeNode* root) {
    if (root != NULL) {
        binaryInorderTraversal(root->left);
        printf("%d ", root->data);
        binaryInorderTraversal(root->right);
    }
}

// Function to find the minimum value node in a binary tree
struct TreeNode* minBinaryValueNode(struct TreeNode* node) {
    struct TreeNode* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Function to delete a node with the given key from the binary tree
struct TreeNode* deleteBinaryNode(struct TreeNode* root, int key) {
    // Base case: If the tree is empty
    if (root == NULL) {
        return root;
    }
    // Recur down the tree
    if (key < root->data) {
        root->left = deleteBinaryNode(root->left, key);
    } else if (key > root->data) {
        root->right = deleteBinaryNode(root->right, key);
    } else {
        // Node with only one child or no child
        if (root->left == NULL) {
            struct TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct TreeNode* temp = root->left;
            free(root);
            return temp;
        }
        // Node with two children: Get the inorder successor (smallest in the right subtree)
        struct TreeNode* temp = minBinaryValueNode(root->right);
        // Copy the inorder successor's content to this node
        root->data = temp->data;
        // Delete the inorder successor
        root->right = deleteBinaryNode(root->right, temp->data);
    }
    return root;
}

// Function to print the binary tree
void printGivenLevelBinary(struct TreeNode* root, int level, bool diamond) {
    if (root == NULL) {
        if (diamond)
            printf("  ");
        else
            printf("- ");
        return;
    }
    if (level == 1) {
        if (diamond)
            printf("%-2d", root->data);
        else
            printf("%d ", root->data);
    } else if (level > 1) {
        printGivenLevelBinary(root->left, level - 1, diamond);
        printGivenLevelBinary(root->right, level - 1, diamond);
    }
}

// Function to get the height of the binary tree
int binaryTreeHeight(struct TreeNode* root) {
    if (root == NULL) {
        return 0;
    } else {
        int leftHeight = binaryTreeHeight(root->left);
        int rightHeight = binaryTreeHeight(root->right);
        return (leftHeight > rightHeight) ? (leftHeight + 1) : (rightHeight + 1);
    }
}

// Function to print the binary tree
void printBinaryTree(struct TreeNode* root, bool diamond) {
    if (root == NULL) {
        return;
    }
    int h = binaryTreeHeight(root);
    int i, j;
    for (i = 1; i <= h; i++) {
        for (j = 0; j < h - i; j++) {
            printf("  ");
        }
        printGivenLevelBinary(root, i, diamond);
        printf("\n");
    }
}
