#include<stdio.h>
#include<stdlib.h>

struct tnode {
	int data;
	char color;
	
	struct tnode *left;
	struct tnode *right;
	struct tnode *parent;
};

struct snode {
	int data;
	char color;
	int parentdata;
	char levelc;
	int level;
	
	struct snode *next;
};

struct tnode *grandpa(struct tnode *new) {

	if (new!=NULL && new->parent!=NULL) {
		return new->parent->parent;
	}
	else {
		return NULL;
	}
}

struct tnode *uncle(struct tnode *new) {
	struct tnode *g,*u;
	g = grandpa(new);
	if (g != NULL) {
		if (new->parent == g->left) {
			u = g->right;
		}
		else {
			u = g->left;
		}
		return u;
	}
	else {
		return NULL;
	}	
}

struct tnode *root = NULL;

void rotateleft(struct tnode *p) {
	struct tnode *n,*g,*hold;
	n = p->right;
	g = p->parent;
	hold = n->left;
	n->left = p;
	p->right = hold; 
	p->parent = n;
	n->parent = g;
	if (hold != NULL) {
		hold->parent = p;
	}
	if (g == NULL) {
		root = n;
	}
	else if (p == g->left){
		g->left = n;
	}
	else{
		g->right = n;
	}
}

void rotateright(struct tnode *p) {
	struct tnode *n,*g,*hold;
	n = p->left;
	g = p->parent;
	hold = n->right;
	n->right = p;
	p->left = hold; 
	p->parent = n;
	n->parent = g;
	if (hold != NULL) {
		hold->parent = p;
	}
	if (g == NULL) {
		root = n;
	}
	else if (p == g->right){
		g->right = n;
	}
	else {
		g->left = n;
	}
}

void rebuild(struct tnode *root,struct tnode *n) {
	struct tnode *parent1;
	struct tnode *grandpa1;
	
	while(n != root && n->color != 'B' && n->parent->color == 'R') {
		parent1 = n->parent;
		grandpa1 = grandpa(n);
		
		if (parent1 == grandpa1->left) {
			struct tnode *uncle1 = uncle(n);
			
			if (uncle1 != NULL && uncle1->color=='R') {
				uncle1 -> color = 'B';
				parent1->color = 'B';
				grandpa1->color = 'R';
				n = grandpa1;
			}
			else {
				if (n == parent1->right) {
					rotateleft(parent1);
					n = parent1;
					parent1 = n->parent;
				}
				rotateright(grandpa1);
				char c = parent1->color;
				parent1->color = grandpa1->color;
				grandpa1->color = c;
				n = parent1;
				
			}	
		}
		else {
			struct tnode *uncle1 = uncle(n);
			
			if (uncle1 != NULL && uncle1->color=='R') {
				uncle1 -> color = 'B';
				parent1->color = 'B';
				grandpa1->color = 'R';
				n = grandpa1;
			}
			else {
				if (n == parent1->left) {
					rotateright(parent1);
					n=parent1;
					parent1 = n->parent;
				}
				
				rotateleft(grandpa1);
				char c = parent1->color;
				parent1->color = grandpa1->color;
				grandpa1->color = c;
				n = parent1;
			
			}
		}
	}
	root->color = 'B';
}


void addtotree(int val) {
	struct tnode *newnode = malloc(sizeof(struct tnode));
	
	newnode -> data = val;
	newnode -> color = 'R';
	newnode -> left = NULL;
	newnode -> right = NULL;
	
	if (root == NULL) {
		newnode -> parent = NULL;
		root = newnode;
	}
	
	else {
		struct tnode *head, *headpre;
		head = root;
		while (head != NULL) {
			if (newnode->data < head->data) {
				headpre = head;
				head = head->left;
			}
			else {
				headpre = head;
				head = head->right;
			}
		}
		if (newnode->data < headpre->data) {
			headpre->left = newnode;
		}
		else {
			headpre->right = newnode;
		}
		newnode->parent = headpre;
	}
	rebuild(root,newnode);
}

void printinorder(struct tnode *root) {
	if (root == NULL) {
		return;
	}
	printinorder(root->left);
	printf("%d\n",root->data);
	printinorder(root->right);
}

int treeheight(struct tnode *root1) {
	if (root1 == NULL) {
		return 0;
	}
	else {
		int lefth = treeheight(root1->left);
		int righth = treeheight(root1->right);
		
		if (lefth > righth) {
			return lefth+1;
		}
		else {
			return righth+1;
		}
	}
}

void addlevel(struct tnode *root1,int level,struct snode **head) {
	struct snode *temp = *head;
	if (root1 == NULL) {
		return;
	}
	if (level == 1) {
		struct snode *newnode = malloc(sizeof(struct snode));
		newnode->data = root1->data;
		newnode->color = root1->color;
		if (root1->parent != NULL) {
			newnode->parentdata = root1->parent->data;
		}
		newnode->next = NULL;
		if (*head == NULL) {
			*head = newnode;
		}
		else {
			while (temp->next != NULL) {
				temp = temp->next;
			}
			temp->next = newnode;
		}
		
	}
	else if (level > 1){
		addlevel(root1->left,level-1,head);
		addlevel(root1->right, level-1,head);
	}
}

void lllevelorder(struct tnode *root1,struct snode **head) {
	int h = treeheight(root1);
	for (int i=1; i<=h; i++) {
		addlevel(root1,i,head);
	}
}

int main() {
	int val=0;
	while(val!=-1) {
		scanf("%d",&val);
		if(val != -1) {
			addtotree(val);
		}
	}
	
	printinorder(root);
	printf("\n");
	
	struct tnode *rootie = root;
	struct snode *head=NULL;
	lllevelorder(rootie,&head);
	
	struct tnode *rootie2 = root;
	struct snode *temp = head;
	
	while (temp!= NULL) {
		if (temp != head) {
			int count=0;
			char c;
			while(temp->data != rootie2->data) {
				count++;
				if (temp->data < rootie2->data) {
					rootie2 = rootie2->left;
					c = 'L';
					
				}
				else {
					rootie2 = rootie2->right;
					c = 'R';
				}
			}
			temp->levelc = c;
			temp->level = count+1;
		}
		temp = temp->next;
		rootie2 = root;
	}
	temp=head;
	printf("%d %c\n",temp->data,temp->color);
	temp=temp->next;
	while(temp!=NULL) {
		if (temp->next!=NULL && temp->level != temp->next->level) {
			printf("%d %c %c%d %c%c\n",temp->data,temp->color,'(',temp->parentdata,temp->levelc,')');
		}
		else {
			printf("%d %c %c%d %c%c ",temp->data,temp->color,'(',temp->parentdata,temp->levelc,')');
		}
		temp = temp->next;
	}	
	
	
	
	return 0;
}












