#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Include necessary header files
#include "avl_tree.c"
#include "binary_tree.c"
#include "b_tree.c"

// Main function with user interface
int main() {
    Node* root = NULL;
    int choice, choice1,key;
    
    struct TreeNode* root1 = NULL;
    
    struct bnode* root2 = btree_create_node();
    printf("\n1. AVL Tree\n2. Bianry Tree\n3. B-Tree\n");
    printf("Enter your choice: ");
    scanf("%d",&choice1);
    switch (choice1){
        case 1:while (1) {
                    printf("For AVL Tree");
                    printf("\n1. Insert\n2. Delete\n3. Inorder traversal\n4. Print tree\n5. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        printf("Enter the key to insert: ");
                        scanf("%d", &key);
                        root = insert(root, key);
                        break;
                    case 2:
                        printf("Enter the key to delete: ");
                        scanf("%d", &key);
                        root = deleteNode(root, key);
                        break;
                    case 3:
                        printf("Inorder traversal of the AVL tree is: ");
                        inorder(root);
                        printf("\n");
                        break;
                    case 4:
                        printf("AVL Tree:\n");
                        printTree(root, false);
                        break;
                    case 5:
                        exit(0);
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
            }
            break;
        case 2:while (1) {
                printf("For Binary Tree");
                printf("\n1. Insert\n2. Delete\n3. Inorder Traversal\n4. Print Tree\n5. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        printf("Enter the key to insert in binary tree: ");
                        scanf("%d", &key);
                        root1 = insertBinaryNode(root1, key);
                        break;
                    case 2:
                        printf("Enter the key to delete from binary tree: ");
                        scanf("%d", &key);
                        root1 = deleteBinaryNode(root1, key);
                        break;
                    case 3:
                        printf("Inorder traversal of the binary tree: ");
                        binaryInorderTraversal(root1);
                        printf("\n");
                        break;
                    case 4:
                        printf("Binary Tree:\n");
                        printBinaryTree(root1, false);
                        break;
                    case 5:
                        exit(0);
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
            }
            break;
        case 3:while (1) {
               printf("For B-Tree");
               printf("\n1. Insert\n2. Delete\n3. Inorder Traversal\n4. Print Tree\n5. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    case 1:
                        printf("Enter the key to insert in B-tree: ");
                        scanf("%d", &key);
                        btree_insert(&root2, key);
                        break;
                    case 2:
                        printf("Enter the key to delete from B-tree: ");
                        scanf("%d", &key);
                        btree_delete_node(root2, key);
                        break;
                    case 3:
                        printf("Inorder traversal of the B-tree: ");
                        btree_inorder_traversal(root2);
                        printf("\n");
                        break;
                    case 4:
                        printf("B-Tree:\n");
                        printBTree(root2, false);
                        break;
                    case 5:
                        exit(0);
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
            }
            break;
    }

    return 0;
}
