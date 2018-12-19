#include <stdio.h>
#include <stdlib.h>

// definicao da estrutura usada para representar os nos da arvore:
typedef struct avl AVL;

struct avl{
    int n;
    int b;
    AVL* left;
    AVL* right;
};

// retorna o valor representado pelo no;
int info(AVL* a) {
	return a->n;
}

// retorna a altura da arvore, armazenada no no:
int height(AVL* tree){
    if(tree){
	   return tree->b;
    }
	return -1;
}

// retorna o fator de balanceamento do no, checando se ha diferenca nas alturas dos seus filhos da direita e da esquerda:
int balancing(AVL* tree){
    return labs(height(tree->left) - height(tree->right));
}

// funcao auxiliar que retorna o maior de dois inteiros:
int returnsGreater(int x, int y){
    return ((x > y) ? x : y);
}

// funcao que realiza a rotacao para a esquerda, usada para o balanceamento:
void rotateLeft(AVL** tree){
    AVL* temp = (*tree)->left;
    (*tree)->left = temp->right;
    temp->right = *tree;
    (*tree)->b = returnsGreater(height((*tree)->left), height((*tree)->right)) + 1;
    temp->b = returnsGreater(height(temp->left), (*tree)->b) + 1;
    *tree = temp;
}

// funcao que realiza a rotacao para a direita, usada para o balanceamento:
void rotateRight(AVL** tree){
    AVL* temp = (*tree)->right;
    (*tree)->right = temp->left;
    temp->left = (*tree);
    (*tree)->b = returnsGreater(height((*tree)->left),height((*tree)->right)) + 1;
    temp->b = returnsGreater(height(temp->right), (*tree)->b) + 1;
    *tree = temp;
}

// funcao que realiza a rotacao dupla direita/esquerda:
void rotateRL(AVL** tree){
    rotateLeft(&((*tree)->right));
    rotateRight(tree);
}

// funcao que realiza a rotacao dupla esquerda/direita:
void rotateLR(AVL** tree){
    rotateRight(&((*tree)->left));
    rotateLeft(tree);
}

// funcao que realiza a insercao na arvore e realiza o balanceamento automatico se necessario:
int avl_insert(AVL** tree, int num){
    int res;
    if(!(*tree)){
        AVL* aux = malloc(sizeof(AVL));
        aux->n = num; aux->b = 0; aux->left = aux->right = NULL;
        (*tree) = aux;
        return 1;
    }

    AVL* temp = (*tree);
    if(num < temp->n){
        if((res = avl_insert((&(temp->left)), num)) == 1)
            if(balancing(temp) >= 2){
                if(num < (*tree)->left->n)
                    rotateLeft(tree);
                else
                    rotateLR(tree);
            }
    }
    else{
        if((res = avl_insert((&(temp->right)), num)) == 1)
            if(balancing(temp) >= 2) {
                if(num >= (*tree)->right->n) rotateRight(tree);
                else rotateRL(tree);
            }
    }

    temp->b = returnsGreater(height(temp->left), height(temp->right)) + 1;

    return res;
}

// funcao que retorna o menor valor da arvore:
AVL* avl_returnsLess(AVL* tree){
    AVL* aux = tree, *temp = tree->left;
    while(temp){
        aux = temp;
        temp = temp->left;
    }
    return aux;
}

// funcao que remove o valor do parametro num da arvore, se existir, e realiza o balanceamento automatico se necessario:
int avl_remove(AVL** tree, int num){
	if(!(*tree)){
	    printf("Valor não existe!\n");
	    return 0;
	}

    int res;
	if(num < (*tree)->n)
	    if((res = avl_remove(&(*tree)->left, num)) == 1)
            if(balancing(*tree) >= 2){
                if(height((*tree)->right->left) <= height((*tree)->right->right))
                    rotateRight(tree);
                else
                    rotateRL(tree);
            }
	if(num > (*tree)->n)
	    if((res = avl_remove(&(*tree)->right, num)) == 1)
            if(balancing(*tree) >= 2){
                if(height((*tree)->left->right) <= height((*tree)->left->left))
                    rotateLeft(tree);
                else
                    rotateLR(tree);
            }

	if((*tree)->n == num){
	    if( (!(*tree)->left) || (!(*tree)->right)) {
			AVL* aux = *tree;
			if( (*tree)->left)
                *tree = (*tree)->left;
            else
                *tree = (*tree)->right;
			free(aux);
		}
		else {
			AVL* temp = avl_returnsLess((*tree)->right);
			(*tree)->n = temp->n;
			avl_remove( &(*tree)->right, (*tree)->n );
            if(balancing(*tree) >= 2){
				if(height((*tree)->left->right) <= height((*tree)->left->left))
					rotateLeft(tree);
				else
					rotateLR(tree);
			}
		}
		if (*tree)
            (*tree)->b = returnsGreater(height((*tree)->left), height((*tree)->right)) + 1;
		return 1;
	}

	(*tree)->b = returnsGreater(height((*tree)->left), height((*tree)->right)) + 1;

	return res;
}

// funcao para impressao da arvore:
void avl_print(AVL* tree){
    printf("(");
    if(tree) {
        printf("%d ", tree->n);
        avl_print(tree->left);
        avl_print(tree->right);
    }
    printf(")");
}

// funcao recursiva para liberacao de toda a arvore:
void avl_free(AVL* a){
  if (a == NULL) return;
  avl_free(a->left);
  avl_free(a->right);
  free(a);
  return;
}
