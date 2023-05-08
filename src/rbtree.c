#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1,sizeof(node_t));
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
  //node_t * z;
  z->key = key;
  while(x != t->nil)
  {
    y = x;
    if (z->key < x->key)
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
  return z;
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

  }
  t->root->color = RBTREE_BLACK;
}
void left_rotate(rbtree * t,node_t * x)
{
  node_t * y = x->right; //지금 노드에 오른쪽
  x->right = y->left; 
  if (y->left != t->nil)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = y;
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
void right_rotate(rbtree * t,node_t * x)
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
  node_t *currentNode = t->root;
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
  node_t * currentNode = t->root;
  while (currentNode->left != t->nil)
  {
    currentNode = currentNode->left;
  }
  return currentNode;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: 제일 큰 수 찾기 구현
  // node_t * currentNode = t->root;
  // while (currentNode != t->nil)
  // {
  //   currentNode = currentNode->right;
  // }
  // return currentNode->parent;
  node_t * currentNode = t->root;
  while (currentNode->right != t->nil)
  {
    currentNode = currentNode->right;
  }
  return currentNode;
}
node_t *tree_minimun(const rbtree *t, node_t *x) 
{
  //TODO : 후임 찾기
  //node_t *curNode = x;
  while (x->left != t->nil)
  {
      x = x->left;
  }
  return x;
}
int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: 삭제 구현
  node_t *y = z;
  node_t *x = t->nil;
  color_t y_original_color = y->color;
  if(z->left == t->nil)
  {
    x = z->right;
    rb_transplant(t,z,z->right);
  }
  else if(z->right == t->nil)
  {
    x = z->left;
    rb_transplant(t,z,z->left);
  }
  else 
  {
    y = tree_minimun(t,z->right); 
    y_original_color = y->color;
    x = y->right;
    if(y->parent == z)
    {
      x->parent = y;
    }
    else
    {
      rb_transplant(t,y,y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rb_transplant(t,z,y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if(y_original_color == RBTREE_BLACK)
  {
    rb_delete_fixup(t,x);
  }

  free(z);
  return 0;
  //---------------------------------
}



void rb_delete_fixup(rbtree *t,node_t *x)
{
  while(x != t->root && x->color == RBTREE_BLACK)
  {
    if(x == x->parent->left)
    {
      node_t *w = x->parent->right;
      if(w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t,x->parent);
        w = x->parent->right;
      }
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else 
      {
        if(w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t,w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t,x->parent);
        x = t->root;
      }
    }
    else
    {
      node_t *w = x->parent->left;
      if(w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t,x->parent);
        w = x->parent->left;
      }
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else 
      {
        if(w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t,w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t,x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

void rb_transplant(rbtree * t, node_t * u, node_t * v)
{
  if(u->parent == t->nil)
  {
    t->root = v;
  }
  else if(u == u->parent->left)
  {
    u->parent->left = v;
  }
  else
  {
    u->parent->right = v;
  }
  v->parent = u->parent;
}
void array_walk(const rbtree *t,node_t *x,key_t *arr,int *idx)
{
  if(x->left != t->nil)
  {
    array_walk(t,x->left,arr,idx);
  }
  arr[*idx] = x->key;
  *idx += 1;

  if(x->right != t->nil)
  {
    array_walk(t,x->right,arr,idx);
  }
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: 레드 블랙 트리를 배열 바꾸는 걸 구현
  node_t *x = t->root;
  int idx = 0;
  array_walk(t,x,arr,&idx);
  return 0;
}
