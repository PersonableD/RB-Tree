#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  // p->nil 초기화
  p->nil = malloc(sizeof(node_t));
  //RB-Tree nill노드는 모두 검정
  p->nil->color = RBTREE_BLACK;
  //nil노드 왼쪽 오른쪽 부모 초기화
  p->nil->parent = p->nil;
  p->nil->left = p->nil;
  p->nil->right = p->nil;
  //루트노드가 닐노드
  p->root = p->nil;
  return p;
}

//////////////////////////////////////////////////////////////////
void printTree(rbtree *t, node_t *node){
    if(t == NULL || node == t->nil) return;

    printf("%d(%d) ", node->key,node->color);
    printTree(t, node->left);
    printTree(t, node->right);
}
//////////////////////////////////////////////////////////////////

//노드 삭제 함수
//순회하며 nil아닌 노드 삭제
void deleteHelper(rbtree *t, node_t *node){
  if(node==t->nil)
    return;
  deleteHelper(t, node->left);
  deleteHelper(t, node->right);
  free(node);
}
//나머지 nil, t 메모리해제
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if(t == NULL) return;
  deleteHelper(t, t->root);
  free(t->nil);
  free(t);
}

void Right_Rotate(rbtree *t, node_t *x){
  node_t *y;
  y = x->left;
  x->left = y->right;
  //경우 1 : y에 애가 딸린경우
  if(y->right!=t->nil){
    // 애들을 x의 자식으로 붙여준다
    y->right->parent = x;
  }
  y->parent = x->parent;
  //x가 루트인지아닌 지 검사
  if(x->parent == t->nil){
    t->root = y;
  }//parent기준 x의 위치를 찾음
  else if(x==x->parent->right){
    x->parent->right = y;
  }
  else{
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}


//부모가 된 z를 x 로 가져온다
void Left_Rotate(rbtree *t, node_t *x){
  node_t *y;
  //<y의 왼쪽 자식을 x 의 오른쪽 자식으로 붙여주는 과정>//
  y = x->right;
  x->right = y->left;
  if(y->left!=t->nil){
    //y에 애가 딸린경우 애들을 x의 자식으로 붙여준다
    y->left->parent = x;
  }
  //<p의 자식을 x에서 y로 만드는 과정>//
  //y 가 x의 부모를 가리킴
  y->parent = x->parent;
  //x가 루트인지아닌 지 검사
  if(x->parent == t->nil){
    t->root = y;
  }//부모의 위치 따짐
  else if(x==x->parent->left){
    x->parent->left = y;
  }
  else{//x의 부모가 y를 가리킴
    x->parent->right = y;
  }
  //마지막에서 최종적으로 x y의 서로의 관계를 바꿔준다.
  y->left = x;
  x->parent = y;
}

void insert_fixup(rbtree *t, node_t *z){
  //y는 삼촌
  node_t *y;
  //지금 현재 부모가 빨갛고 나도 빨간상태라면 while문 계속 돔.
  while(z->parent->color==RBTREE_RED)
  { // 내 엄마가 할배의 왼쪽 자식이라면
    if(z->parent == z->parent->parent->left)
    {   //삼촌쓰 y 할배 오른쪽 자식
        y = z->parent->parent->right;
        //1번케이스 : 삼촌이 빨갛다면 (그럼 나 엄마 삼촌 다 빨간상태~)
        if(y->color==RBTREE_RED){
          //할배검정색이랑 할배자식인 부모/삼촌색이랑 바꿈
          z->parent->color = RBTREE_BLACK;
          y->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          //할배가 빨간색됐으니 책임전가 = 할배 엄마가 또 빨간색이면 fixed up 다시 돔.
          z = z->parent->parent;
        } //삼촌쓰가 검정색이면
        else{ 
          //척추 굽은 상태면 내 엄마 기준으로 왼쪽 드리프트로 펴준다
          if(z==z->parent->right){
            z = z->parent;
            Left_Rotate(t, z);
          }
          //엄마랑 할배 색 체인지 해서 엄마 검정색 만들고
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          //오른쪽으로 관성드리프트
          Right_Rotate(t, z->parent->parent);
        }
    }
    else
    {   // 내 엄마가 할배의 오른쪽 자식이라면(위와 대칭되는 구조)
        y = z->parent->parent->left;
        if(y->color==RBTREE_RED){
          z->parent->color = RBTREE_BLACK;
          y->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          z = z->parent->parent;
        }
        else{ 
          if(z==z->parent->left){
            z = z->parent;          
            Right_Rotate(t, z);
          }
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          Left_Rotate(t, z->parent->parent);
        }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *x, *y, *z;
  y = t->nil;
  x = t->root;
  z = (node_t*)malloc(sizeof(node_t));
  z->key = key;
  //삽입하는 노드는 무조건 색이 레드이고 자식들이 모두 nil임!
  z->color = RBTREE_RED;
  z->left = t->nil;
  z->right = t->nil;
  //<삽입할 위치 찾는 로직>//
  while(x!=t->nil)  // 빈 트리가 아닐 때 BST 방식과 동일하게 집어넣을 위치를 찾는다.
  {//y에 x의 부모값을 저장
    y = x;
    if(key < x->key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }
  //찾은 후 y를 삽입하는 노드의 부모로 만들어준다.
  z->parent = y;
  //<삽입하는 로직>//
  //y가 x 의 부모니까 부모가 nil 노드라면(내가 첫 노드라면)지금넣는 노드를 루트에 넣는다.
  if(y==t->nil){
    t->root = z;
  }//내가 첫 노드가 아니라면
  else if(z->key < y->key){ //현재 노드의 값이 부모의 값보다 작다면 부모 왼쪽 자식이 된다.
    y->left = z;
  }
  else{//아니면 부모의 오른쪽 자식이 된다.
    y->right = z;
  }
  //<규칙 위반 시 바로잡는 로직>//
  insert_fixup(t, z);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *cur;
  cur = t->root;
  //같은 값을 찾을 때까지 계속 돔
  while(cur!= t->nil){
    if(cur->key == key){
      break;
    }
    if(key < cur->key){
      cur = cur->left;
    }
    else{
      cur = cur->right;
    }
  }
  //없는 값 찾을 때 테스트케이스 q=NULL 통과용
  if(cur==t->nil){
    return NULL;
  }
  return cur;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *cur;
  cur = t->root;
  while(cur->left!=t->nil){
    cur = cur->left;
  }
  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur;
  cur = t->root;
  while(cur->right!=t->nil){
    cur = cur->right;
  }
  return cur;
}


node_t *Tree_Minimum(rbtree *t,node_t *z){
  while (z->left!=t->nil)
  {
    z = z->left;
  }
  return z;
}

//u를 v로 대체
void RB_Transplant(rbtree *t,node_t *u,node_t *v){
  if(u->parent == t->nil){
    t->root = v;
  }
  else if(u==u->parent->left){
    u->parent->left = v;
  }
  else{
    u->parent->right = v;
  }
  v->parent = u->parent;
}


void RB_Delete_Fixup(rbtree *t, node_t *x){
  node_t *w;
  while(x != t->root && x->color == RBTREE_BLACK){
    //x가 왼쪽 노드일 때
    if(x==x->parent->left){
      //w는 내 형제
      w = x->parent->right;
      //내 형제가 빨간색일 경우
      if(w->color == RBTREE_RED){
        //형제 컬러를 블랙으로 바꾼뒤 
        w->color = RBTREE_BLACK;
        //내 부모의 색을 레드로 바꾼다.
        x->parent->color = RBTREE_RED;
        Left_Rotate(t, x->parent);
        w = x->parent->right;
      }
      //형제 노드가 검은색이고 형제의 두 자식 모두 검은색인 경우
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        // 양쪽의 블랙을 올려줌. 나는 엑스트라 블랙을 올리고 그냥블랙됨. 형제는 블랙을 올렸으니까 검은 옷이 벗겨져서 속살(레드)이 드러남.
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else 
      {
        if(w->right->color == RBTREE_BLACK){
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        Right_Rotate(t, w);
        w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        Left_Rotate(t, x->parent);
        x = t->root;
      }
    }
    else{
      //x가 오른쪽 노드일 때
      //w는 내 형제
      w = x->parent->left;
      //내 형제가 빨간색일 경우
      if(w->color == RBTREE_RED){
        //형제 컬러를 블랙으로 바꾼뒤 
        w->color = RBTREE_BLACK;
        //내 부모의 색을 레드로 바꾼다.
        x->parent->color = RBTREE_RED;
        Right_Rotate(t, x->parent);
        w = x->parent->left;
      }
      //형제 노드가 검은색이고 형제의 두 자식 모두 검은색인 경우
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        //형제를 빨간색으로 바꿈
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else 
      {
        if(w->left->color == RBTREE_BLACK){
        w->right->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        Left_Rotate(t, w);
        w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        Right_Rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  if(z==NULL)
    return 0;
  node_t *x;
  node_t *y = z;
  color_t y_origin_color = y->color;

  // 삭제할 노드를 임시 변수에 저장
  node_t *to_delete = z;

  //자식이 없거나 하나만 있을 경우
  if(z->left == t->nil){
    x = z->right;
    RB_Transplant(t, z, z->right);
  }
  else if(z->right == t->nil){
    x = z->left;
    RB_Transplant(t, z, z->left);
  }
  else{
    //자식이 2개 있을 경우 후계자 찾기
    y = Tree_Minimum(t, z->right);
    //후계자의 색 저장.트리 균형 맞출 때 필요
    y_origin_color = y->color;
    //후계자의 오른쪽 자식 저장. 왼쪽자식은 자기가 젤 작은 값이기 때문에 있을수가 없음.
    x = y->right;
    //후계자가 z의 직접적인 자식일 경우(z의 오른쪽 자식의 왼쪽 자식이 없는 경우)
    //////////////////////////////////////////////////////////////////////
    if(y!=z->right){
      RB_Transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    else{
      x->parent = y;
    }
    //////////////////////////////////////////////////////////////////////
    RB_Transplant(t,z,y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  // 여기서 메모리 해제
  free(to_delete);

  if(y_origin_color == RBTREE_BLACK){
    RB_Delete_Fixup(t,x);
  }

  return 0;
}

// 포인터를 사용하면 함수 내에서 변경한 값이 함수 외부에서도 유지됨.
void inorder(const rbtree *t, node_t *node, key_t *arr, size_t *index, const size_t n){
  if(node==t->nil || *index >=n) return;
  inorder(t, node->left, arr, index, n);
  arr[*index] = node->key;
  (*index)++;
  inorder(t, node->right, arr, index, n);
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  if(t->root==t->nil)return 0;
  size_t index = 0;
  inorder(t, t->root, arr, &index, n);
  return 0;
}
