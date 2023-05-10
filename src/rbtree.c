#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); //tree 메모리에 올리기
  node_t *nil = (node_t *)calloc(1,sizeof(node_t)); //nil 메모리에 올리기
  nil->color = RBTREE_BLACK; //nil는 항상 black
  p->nil = nil; //tree를 처음 생성해서 nill를 가르킨다
  p->root = nil; //node가 없음으로 root는 nil
  // TODO: 필요한 경우 구조체 초기화
  return p; //tree를 반환
}

void delete_rbtree(rbtree *t) {
  // TODO: 트리 노드의 메모리 회수
  node_t * node = t->root; 
  if(node != t->nil) //root부터 nil가 나올 때까지
  {
    delete_nil(t,node); //재귀로 모든 tree의 node를 찾아준다
  }
  free(t->nil); //nil 메모리 반환
  free(t); //tree의 메모리 반환
}
void delete_nil(rbtree *t,node_t *node)
{
  if(node->left != t->nil) //왼쪽 nil가 나올 때 까지
  {
    delete_nil(t,node->left); //재귀를 돈다
  }
  if(node->right != t->nil) //오른쪽 nil가 나올 때 까지
  { 
    delete_nil(t,node->right); //재귀를 돈다
  }
  free(node); //나온 node들을 반환 해준다
}

node_t *rbtree_insert(rbtree *t, const key_t key) 
{
  // TODO: 삽입을 구현
  node_t * y = t->nil; //nil 노드 ()
  node_t * parent = t->root; //부모노드 노드
  node_t * node = (node_t *)calloc(1,sizeof(node_t)); //새로 넣을 node
  //node_t * z;
  node->key = key; //새로 넣을 node에 키값을 넣어준다
  while(parent != t->nil) //새로 넣을 node를 어느곳에 넣을지 정한다.
  {
    y = parent;
    if (node->key < parent->key) 
    {
      parent = parent->left; //z가 x보다 작으면 왼쪽으로 간다
    }
    else
    {
      parent = parent->right; //z가 x보다 크면 오른쪽으로 간다
    }
  }
  node->parent = y; //z node을 y의 부모로 지정
  if (y == t->nil) //위에를 하지 않으면(tree의 root가 nil일 경우)
  {
    t->root = node; //새로 넣어주는 node가 root가 된다
  }
  else if(node->key < y->key) //root가 있으면 y의 왼쪽인지 오른쪽인지 정한다.
  {
    y->left = node; 
  }
  else
  {
    y->right = node;
  }
  node->left = t->nil; //가장 아래의 node는 자식node가 nil
  node->right = t->nil; //가장 아래의 node는 자식node가 nil
  node->color = RBTREE_RED; //새로 넣어주는 node는 무조건 red다
  rbtree_inster_fixup(t,node); //tree의 케이스를 검사해주고 tree를 수정한다
  return node;
}
void rbtree_inster_fixup(rbtree *t,node_t * node)
{
  while(node->parent->color == RBTREE_RED) //새로넣은 ndoe의 부모의 색이 red면
  { 
    if(node->parent == node->parent->parent->left) //node가 왼쪽에 있으면
    {
      node_t * uncle = node->parent->parent->right; //삼촌node를 찾아준다
      if(uncle->color == RBTREE_RED) //삼촌node가 red면
      {
        node->parent->color = RBTREE_BLACK; //부모의 색은 black
        uncle->color = RBTREE_BLACK; //삼촌node의 색은 black
        node->parent->parent->color = RBTREE_RED; //할아버지node의 색은 red
        node = node->parent->parent; //z를 바꿔준다
      }
      else //삼촌node가 black이면
      {
        if(node == node->parent->right) //z가 오른쪽에 있으면
        {
          node = node->parent; //z을 부모node로 바꾼다
          left_rotate(t,node); //왼쪽으로 돌린다.
        }
        node->parent->color = RBTREE_BLACK; //z의 부모의 색은 black
        node->parent->parent->color = RBTREE_RED; //z의 할아버지의 색은 red
        right_rotate(t,node->parent->parent); //오른쪽으로 돌린다
      }
    }
    else //node가 오른쪽에 있으면
    {      
      node_t * uncle = node->parent->parent->left; //삼촌 node를 찾아준다
      if(uncle->color == RBTREE_RED) //삼촌 node가 red면
      {
        node->parent->color = RBTREE_BLACK; //z의 부모 색은 black
        uncle->color = RBTREE_BLACK; //삼촌의 색은 red
        node->parent->parent->color = RBTREE_RED; //할아버지node의 색은 red
        node = node->parent->parent; //z를 바꿔준다.
      }
      else //삼촌 node가 black이면
      {
        if(node == node->parent->left) //z가 왼쪽에 있으면
        {
          node = node->parent; //z을 부모로 바꾸고
          right_rotate(t,node); //오른쪽으로 돌린다
        }
        node->parent->color = RBTREE_BLACK; //z의 부모의 색은 블랙
        node->parent->parent->color = RBTREE_RED; //z의 할아버지의 색은 레드
        left_rotate(t,node->parent->parent); //왼쪽으로 돌린다.
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

void left_rotate(rbtree * t,node_t * node)
{
  node_t * y = node->right; //돌리기 전에 옳길 node를 정해준다
  node->right = y->left;  //옳겨야할 node를 옳긴다
  if (y->left != t->nil) //y의 왼쪽에 자식이 없으면
  {
    y->left->parent = node; //y의 왼쪽은 node로 준다
  }
  y->parent = node->parent; //y의 부모를 node의 부모로 넘긴다
  if (node->parent == t->nil) //node의 부모가 없다 == node는 root다
  {
    t->root = y; //y가 루트가 된다
  }
  else if(node == node->parent->left) //node의 부모가 있고 node가 왼쪽에 있으면
  {
    node->parent->left = y; //node의 부모의 왼쪽은 y
  }
  else //node가 오른쪽에 있으면
  {
    node->parent->right = y; //node의 부모의 오른쪽은 y
  }
  y->left = node; //y의 왼쪽은 node
  node->parent = y; //node의 부모는 y
}
void right_rotate(rbtree * t,node_t * node)
{
  node_t * y = node->left; //돌리기 전에 옳길 node를 정해준다
  node->left = y->right; //옳겨야할 node를 옳긴다
  if (y->right != t->nil) //y의 오른쪽에 자식이 없으면
  {
    y->right->parent = node;  //y의 오른쪽은 node로 준다
  }
  y->parent = node->parent; //y의 부모를 node의 부모로 넘긴다
  if(node->parent == t->nil) //node의 부모가 없다 == node는 root다
  {
    t->root = y; //y가 루트가 된다
  }
  else if(node == node->parent->right)
  {
    node->parent->right = y;
  }
  else
  {
    node->parent->left = y;
  }
  y->right = node;
  node->parent = y;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: 찾기를 구현
  node_t *findNode = t->root; //루트에서 시작
  while(findNode != t->nil) //nil을 찾을 때까지
  {
    if(findNode->key == key) //찾는 키랑 지금 노드랑 같으면
    { 
      return findNode; //지금 노드 반환
    }
    else if(findNode->key < key) //찾는 키가 지금 키보다 크면
    {
      findNode = findNode->right; //오른쪽으로 이동
    }
    else //찾는 키가 지금 키보다 작으면
    {
      findNode = findNode->left; //왼쪽으로 이동
    }
  }
  return NULL; //찾는게 없으면 NULL반환
  // return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  node_t * minNode = t->root; //루트에서 시작
  while (minNode->left != t->nil) //지금 node의 왼쪽 자식이 nil이 아니면
  {
    minNode = minNode->left; //왼쪽으로 계속 이동
  }
  return minNode; //제일 작은 값 반환
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: 제일 큰 수 찾기 구현
  // node_t * currentNode = t->root;
  // while (currentNode != t->nil)
  // {
  //   currentNode = currentNode->right;
  // }
  // return currentNode->parent;
  node_t * maxNode = t->root; //루트에서 시작
  while (maxNode->right != t->nil) //지금 node의 오른쪽 자식이 nil이 아니면
  {
    maxNode = maxNode->right; //오른쪽으로 계속 이동
  }
  return maxNode; //제일 작은 값 반환
}
node_t *tree_minimun(const rbtree *t, node_t *successorNode) //x->right를 가져온다  
{
  //TODO : 후임 찾기
  //node_t *curNode = x;
  while (successorNode->left != t->nil) //그 노트에서 가장 작은값 찾아오기
  {
      successorNode = successorNode->left;
  }
  return successorNode; //그 노트에서 가장 작은 값 == 후임
}
int rbtree_erase(rbtree *t, node_t *eraseNode) {
  // TODO: 삭제 구현
  node_t *y = eraseNode; //지울 node를 받아둔다
  node_t *x = t->nil; //전역으로 받을 x
  color_t y_original_color = y->color; //엑스트라블랙설정
  if(eraseNode->left == t->nil) //지울 node의 왼쪽이 nil이면
  {
    x = eraseNode->right; //x에 지울node의 오른쪽을 받는다.
    rb_transplant(t,eraseNode,eraseNode->right); //eraseNode를 지우고 그 자리에 eraseNode의 right을 넣는다
  }
  else if(eraseNode->right == t->nil) //z의 오른쪽이 nil이면
  {
    x = eraseNode->left; //x에 z의 왼쪽을 받는다.
    rb_transplant(t,eraseNode,eraseNode->left); //eraseNode를 지우고 그 자리에 eraseNode의 left를 넣는다
  }
  else //eraseNode의 자식이 nil이 아니면
  {
    y = tree_minimun(t,eraseNode->right); //후임을 찾아준다.
    y_original_color = y->color; //y의 색을 가져온다
    x = y->right;                //x에 y의 오른쪽을 넣는다
    if(y->parent == eraseNode)           //지울 node가 y의 부모면
    {
      x->parent = y;            //x의 부모는 y
    }
    else                       //지울 node가 후임의 부모가 아니면
    {
      rb_transplant(t,y,y->right); //y를 버려두고 y의 right를 넣어준다
      y->right = eraseNode->right; 
      y->right->parent = y;
    }
    rb_transplant(t,eraseNode,y); //eraseNode를 버려놨던 y랑 바꾼다
    y->left = eraseNode->left; //바꾼 노드를 연결시켜준다
    y->left->parent = y;
    y->color = eraseNode->color;
  }
  if(y_original_color == RBTREE_BLACK) //바꾼 node의 원래 색이 검정이면
  {
    rb_delete_fixup(t,x); //노드를 수정해준다
  }

  free(eraseNode);  //완료하면 메모리 반환
  return 0;
  //---------------------------------
}



void rb_delete_fixup(rbtree *t,node_t *x)
{
  while(x != t->root && x->color == RBTREE_BLACK) //수정할 node가 root가 아니고 black일때 까지
  {
    if(x == x->parent->left) //x가 left에 있으면
    {
      node_t *w = x->parent->right; //w에 x의 parent의 right를 넣는다.
      if(w->color == RBTREE_RED) //w의 색은 red면
      {
        w->color = RBTREE_BLACK; //w의 색은 black
        x->parent->color = RBTREE_RED; //x의 부모의 색은 red
        left_rotate(t,x->parent); //x의 parent를 기준으로 왼쪽으로 돌린다
        w = x->parent->right; //w를 x의 parent의 right으로 변경
      }
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) //w의 left색이 black이고 w의 right색이 black이면
      {
        w->color = RBTREE_RED; //w이 색은 red
        x = x->parent; //x는 x의 parent
      }
      else //w의 자식중 red가 있으면
      {
        if(w->right->color == RBTREE_BLACK) //w의 right의 색이 black이면
        {
          w->left->color = RBTREE_BLACK; //w의 left의 색은 black
          w->color = RBTREE_RED; //w의 색은 red
          right_rotate(t,w); //오른쪽으로 돌린다
          w = x->parent->right; //w는 x의 parent의 rgiht
        }
        w->color = x->parent->color; //w의 색은 x의 부모의 색
        x->parent->color = RBTREE_BLACK; //x의 parent의 색은 black
        w->right->color = RBTREE_BLACK; //w의 right의 색은 black
        left_rotate(t,x->parent); //왼쪽으로 돌린다
        x = t->root; //x는 root
      }
    }
    else //x가 right에 있으면
    {
      node_t *w = x->parent->left; //w에 x의 parent의 left를 넣는다.
      if(w->color == RBTREE_RED) //w의 색은 red면
      {
        w->color = RBTREE_BLACK; //w의 색은 black
        x->parent->color = RBTREE_RED; //x의 부모의 색은 red
        right_rotate(t,x->parent); //x의 parent를 기준으로 오른쪽으로 돌린다
        w = x->parent->left; //w를 x의 parent의 right으로 변경
      }
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) //w의 left색이 black이고 w의 right색이 black이면
      {
        w->color = RBTREE_RED; //w이 색은 red
        x = x->parent; //x는 x의 parent
      }
      else  //w의 자식중 red가 있으면
      {
        if(w->left->color == RBTREE_BLACK) //w의 left의 색이 black이면
        {
          w->right->color = RBTREE_BLACK; //w의 right의 색은 blac
          w->color = RBTREE_RED; //w의 색은 red
          left_rotate(t,w); //왼쪽으로 돌린다
          w = x->parent->left; //w는 x의 parent의 rgiht
        }
        w->color = x->parent->color; //w의 색은 x의 부모의 색
        x->parent->color = RBTREE_BLACK; //x의 parent의 색은 black
        w->left->color = RBTREE_BLACK; //w의 right의 색은 black
        right_rotate(t,x->parent); //오른쪽으로 돌린다
        x = t->root; //x는 root
      }
    }
  }
  x->color = RBTREE_BLACK; //x의 색은 black
}

void rb_transplant(rbtree * t, node_t * u, node_t * v) //u는 트리에서 때어내고 v를 u자리에 넣는다
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
void array_walk(const rbtree *t,node_t *x,key_t *arr,int *idx) //중위 순위를 돌릴 함수
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
