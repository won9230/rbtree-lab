#include "rbtree.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t * nil = (node_t *)calloc(1,sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p->nil = nil;
  p->root = nil;
  // TODO: 필요한 경우 구조체 초기화
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: 트리 노드의 메모리 회수
  node_t * node = t->root;
  if(node != t->nil)
  {
    delete_nil(t,node);
  }
  free(t->nil);
  free(t);
}
void delete_nil(rbtree *t,node_t *node)
{
  if(node->left != t->nil)
  {
    delete_nil(t,node->left);
  }
  if(node->right != t->nil)
  {
    delete_nil(t,node->right);
  }
  free(node);
}

node_t *rbtree_insert(rbtree *t, const key_t key) 
{
  // TODO: 삽입을 구현
  node_t * y = t->nil;
  node_t * x = t->root;
  node_t * z = (node_t *)calloc(1,sizeof(node_t));
  z->key = key;
  while(x != t->nil)
  {
    y = x;
    if (key < x->key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }
  z->parent = y;
  if (y == t->nil)
  {
    t->root = z;
  }
  else if(z->key < y->key)
  {
    y->left = z;
  }
  else
  {
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rbtree_inster_fixup(t,z);
  return t->root;
}
void rbtree_inster_fixup(rbtree *t,node_t * z)
{
  while(z->parent->color == RBTREE_RED)
  {
    if(z->parent == z->parent->parent->left)
    {
      node_t * y = z->parent->parent->right;
      if(y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else 
      {
        if(z == z->parent->right)
        {
          z = z->parent;
          left_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t,z->parent->parent);
      }
    }
    else
    {      
      node_t * y = z->parent->parent->left;
      if(y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if(z == z->parent->left)
        {
          z = z->parent;
          right_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t,z->parent->parent);
      }
    }
    if(z->parent == t->nil)
      break;
  }
  t->root->color = RBTREE_BLACK;
}
void left_rotate(rbtree *t,node_t * x)
{
  node_t * y = x->right; //지금 노드에 오른쪽
  x->right = y->left; 
  if (y->left != t->nil)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == x->parent->left)
  {
    x->parent->left = y;
  }
  else if(x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}
void right_rotate(rbtree * t,node_t *x)
{
  node_t * y = x->left;
  x->left = y->right;
  if (y->right != t->nil)
  {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil)
  {
    t->root = y;
  }
  else if(x == x->parent->right)
  {
    x->parent->right = y;
  }
  else
  {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: 찾기를 구현
  node_t * currentNode = t->root;
  while(currentNode != t->nil)
  {
    if(currentNode->key == key)
    {
      return currentNode;
    }
    else if(currentNode->key < key)
    {
      currentNode = currentNode->right;
    }
    else
    {
      currentNode = currentNode->left;
    }
  }
  return NULL;
  // return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: 제일 작은 수 찾기 구현
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: 제일 큰 수 찾기 구현
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: 삭제 구현
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: 레드 블랙 트리를 배열 바꾸는 걸 구현
  return 0;
}
