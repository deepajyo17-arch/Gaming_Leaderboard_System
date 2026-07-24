#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100

typedef struct Node {
    char name[MAX_NAME];
    int score;
    struct Node *left;
    struct Node *right;
} Node;

/* Create node */
Node* createNode(char name[], int score) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    strncpy(newNode->name, name, MAX_NAME - 1);
    newNode->name[MAX_NAME - 1] = '\0';
    newNode->score = score;
    newNode->left = newNode->right = NULL;
    return newNode;
}

/* Search player by name (full traversal) */
Node* searchByName(Node *root, char name[]) {
    if (root == NULL) return NULL;

    if (strcmp(root->name, name) == 0)
        return root;

    Node *found = searchByName(root->left, name);
    if (found != NULL) return found;

    return searchByName(root->right, name);
}

/* Insert node based on score */
Node* insert(Node *root, char name[], int score) {
    if (root == NULL)
        return createNode(name, score);

    if (score < root->score)
        root->left = insert(root->left, name, score);
    else
        root->right = insert(root->right, name, score);

    return root;
}

/* Find minimum (used in deletion) */
Node* findMin(Node *root) {
    while (root->left != NULL)
        root = root->left;
    return root;
}

/* Delete node by name (FULL traversal – FIXED) */
Node* deleteNode(Node *root, char name[]) {
    if (root == NULL)
        return NULL;

    if (strcmp(root->name, name) == 0) {
        if (root->left == NULL) {
            Node *temp = root->right;
            free(root);
            return temp;
        } 
        else if (root->right == NULL) {
            Node *temp = root->left;
            free(root);
            return temp;
        }

        Node *temp = findMin(root->right);
        strcpy(root->name, temp->name);
        root->score = temp->score;
        root->right = deleteNode(root->right, temp->name);
        return root;
    }

    root->left = deleteNode(root->left, name);
    root->right = deleteNode(root->right, name);
    return root;
}

/* Update score */
Node* updateScore(Node *root, char name[], int newScore) {
    root = deleteNode(root, name);
    root = insert(root, name, newScore);
    return root;
}

/* Display leaderboard (descending order) */
void displayDescending(Node *root) {
    if (root == NULL) return;

    displayDescending(root->right);
    printf("%-30s : %d\n", root->name, root->score);
    displayDescending(root->left);
}

/* Rank helper */
int findRankHelper(Node *root, char name[], int *rank) {
    if (root == NULL) return 0;

    if (findRankHelper(root->right, name, rank))
        return 1;

    (*rank)++;
    if (strcmp(root->name, name) == 0)
        return 1;

    return findRankHelper(root->left, name, rank);
}

/* Find rank */
int findRank(Node *root, char name[]) {
    if (searchByName(root, name) == NULL)
        return -1;

    int rank = 0;
    findRankHelper(root, name, &rank);
    return rank;
}

/* Display top scorer */
void displayTopScorer(Node *root) {
    if (root == NULL) {
        printf("No players in leaderboard!\n");
        return;
    }

    while (root->right != NULL)
        root = root->right;

    printf("\nTOP SCORER\n");
    printf("Name  : %s\n", root->name);
    printf("Score : %d\n", root->score);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    Node *root = NULL;
    int choice, score, newScore;
    char name[MAX_NAME];

    while (1) {
        printf("\n1. Add Player");
        printf("\n2. Update Score");
        printf("\n3. Display Leaderboard");
        printf("\n4. Find Rank");
        printf("\n5. Top Scorer");
        printf("\n6. Exit");
        printf("\nEnter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1:
                printf("Enter name: ");
                fgets(name, MAX_NAME, stdin);
                name[strcspn(name, "\n")] = 0;

                if (searchByName(root, name) != NULL) {
                    printf("Player already exists!\n");
                    break;
                }

                printf("Enter score: ");
                scanf("%d", &score);
                clearInputBuffer();

                root = insert(root, name, score);
                printf("Player added.\n");
                break;

            case 2:
                printf("Enter name: ");
                fgets(name, MAX_NAME, stdin);
                name[strcspn(name, "\n")] = 0;

                if (searchByName(root, name) == NULL) {
                    printf("Player not found!\n");
                    break;
                }

                printf("Enter new score: ");
                scanf("%d", &newScore);
                clearInputBuffer();

                root = updateScore(root, name, newScore);
                printf("Score updated.\n");
                break;

            case 3:
                displayDescending(root);
                break;

            case 4: {
                printf("Enter name: ");
                fgets(name, MAX_NAME, stdin);
                name[strcspn(name, "\n")] = 0;

                int rank = findRank(root, name);
                if (rank == -1)
                    printf("Player not found!\n");
                else
                    printf("Rank: %d\n", rank);
                break;
            }

            case 5:
                displayTopScorer(root);
                break;

            case 6:
                exit(0);

            default:
                printf("Invalid choice!\n");
        }
    }
}