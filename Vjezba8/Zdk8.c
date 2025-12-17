#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESS 0
#define MALLOC_FAILURE -1

struct TreeNode;
typedef struct TreeNode*  TreePosition;
typedef struct TreeNode {
	int data;
	TreePosition left;
	TreePosition right;
} TreeNode;

struct queue;
typedef struct queue* QueuePosition;
typedef struct queue {
	TreePosition treeNode;
	QueuePosition next;
} Queue;

QueuePosition rear = NULL;

TreePosition insert(TreePosition root, int data);
TreePosition search(TreePosition root, int data);
int PrintPreorder(TreePosition root, int level);
int PrintInorder(TreePosition root, int level);
int PrintPostorder(TreePosition root, int level);
QueuePosition enqueue(TreePosition levelItem);
TreePosition dequeue();
int PrintLevelOrder(TreePosition root);
TreePosition findMin(TreePosition root);
TreePosition Delete(TreePosition root, int data);
int FreeTree(TreePosition root);

int main()
{
	TreePosition root = NULL;

	root = insert(root,4);
	root = insert(root, 9);
	root = insert(root, 2);
	root = insert(root, 10);
	root = insert(root, 6);
	root = insert(root, 3);
	root = insert(root, 1);
	root = insert(root, 5);
	root = insert(root, 7);

	search(root, 6);
	search(root, 11);

	printf("Preorder:\n");
	PrintPreorder(root, 0);

	printf("Inorder:\n");
	PrintInorder(root, 0);

	printf("Postorder:\n");
	PrintPostorder(root, 0);

	printf("Level order:\n");
	PrintLevelOrder(root);

	root = Delete(root, 6);
	root = Delete(root, 7);
	printf("Preorder after deleting:\n");
	PrintPreorder(root, 0);

	FreeTree(root);

	return EXIT_SUCCESS;
}
TreePosition insert(TreePosition root, int data)
{
	if (root == NULL)
	{
		root = (TreePosition)malloc(sizeof(TreeNode));
		if (root == NULL)
		{
			printf("Memory allocation failed!\n");
			return NULL;
		}
		root->data = data;
		root->left = NULL;
		root->right = NULL;
	}
	else if (data < root->data)
	{
		root->left = insert(root->left, data);
	}
	else if (data > root->data)
	{
		root->right = insert(root->right, data);
	}
	return root;
}
TreePosition search(TreePosition root, int data)
{
	if (root == NULL)
	{
		printf("Element %d not found in the tree.\n", data);
		return NULL;
	}
	if(data < root->data)
	{
		return search(root->left, data);
	}
	else if(data > root->data)
	{
		return search(root->right, data);
	}
	else
	{
		printf("Element %d found in the tree.\n", data);
		return root;
	}
}
int PrintPreorder(TreePosition root, int level)
{
	int i = 0;

	for(i=0; i < level; i++)
	{
		printf("   ");
	}
	printf("%d\n", root->data);

	if(root->left != NULL)
	{
		PrintPreorder(root->left, level + 1);
	}
	if(root->right != NULL)
	{
		PrintPreorder(root->right, level + 1);
	}
	return 0;
}
int PrintInorder(TreePosition root, int level)
{
	int i = 0;
	if(root->left != NULL)
	{
		PrintInorder(root->left, level + 1);
	}
	for(i=0; i < level; i++)
	{
		printf("   ");
	}
	printf("%d\n", root->data);
	if(root->right != NULL)
	{
		PrintInorder(root->right, level + 1);
	}
	return 0;
}
int PrintPostorder(TreePosition root, int level)
{
	int i = 0;
	if(root->left != NULL)
	{
		PrintPostorder(root->left, level + 1);
	}
	if(root->right != NULL)
	{
		PrintPostorder(root->right, level + 1);
	}
	for(i=0; i < level; i++)
	{
		printf("   ");
	}
	printf("%d\n", root->data);
	return 0;
}
QueuePosition enqueue(TreePosition levelItem)
{
	QueuePosition newItem = NULL;
	newItem = (QueuePosition)malloc(sizeof(Queue));
	if (newItem == NULL)
	{
		printf("Memory allocation failed!\n");
		return NULL;
	}
	newItem->treeNode = levelItem;
	if (rear == NULL)
	{
		newItem->next = newItem;
		return newItem;
	}
	newItem->next = rear->next;
	rear->next = newItem;
	rear = newItem;

	return newItem;
}
TreePosition dequeue()
{
	if (rear == NULL)
	{
		return NULL;
	}
	TreePosition levelItem;
	if (rear == rear->next)
	{
		levelItem = rear->treeNode;
		free(rear);
		rear = NULL;
	}
	else
	{
		QueuePosition front = rear->next;
		levelItem = front->treeNode;
		rear->next = front->next;
		free(front);
	}
	return levelItem;
}
int PrintLevelOrder(TreePosition root)
{
	rear = enqueue(root);

	if (rear == NULL)
	{
		return MALLOC_FAILURE;
	}

	while (rear != NULL)
	{
		TreePosition current = dequeue();
		printf("%d ", current->data);
		if (current->left != NULL)
		{
			enqueue(current->left);
		}
		if (current->right != NULL)
		{
			enqueue(current->right);
		}
	}
	printf("\n");
	return 0;
}
TreePosition findMin(TreePosition root)
{
	if(root->left == NULL)
	{
		return root;
	}
	return findMin(root->left);
}
TreePosition Delete(TreePosition root, int data)
{
	if(root == NULL)
	{
		printf("Element %d not found in the tree.\n", data);
		return root;
	}
	if(data < root->data)
	{
		root->left = Delete(root->left, data);
	}
	else if(data > root->data)
	{
		root->right = Delete(root->right, data);
	}
	else if(root->left != NULL && root->right != NULL)
	{
		TreePosition temp = findMin(root->right);
		root->data = temp->data;
		root->right = Delete(root->right, root->data);
	}
	else
	{
		TreePosition temp = root;
		if(root->left == NULL)
		{
			root = root->right;
		}
		else
		{
			root = root->left;
		}
		free(temp);
	}
	return root;
}
int FreeTree(TreePosition root)
{
	if(root == NULL)
	{
		return EXIT_SUCCESS;
	}
	FreeTree(root->left);
	FreeTree(root->right);
	free(root);
	return EXIT_SUCCESS;
}