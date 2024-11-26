#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  // p->nil 초기화
  p->nil = malloc(sizeof(node_t));
  //알비트리 nill노드는 모두 검정
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
// void printTree(rbtree *t, node_t *node){
//     if(t == NULL || node == t->nil) return;

//     printf("%d(%d) ", node->key,node->color);
//     printTree(t, node->left);
//     printTree(t, node->right);
// }
//////////////////////////////////////////////////////////////////

//노드 삭제 함수
//순회하며 nil아닌 노드 삭제
void deleteHelper(rbtree *t, node_t *node){
  if(node==t->nil)
    return;
  deleteHelper(t, t->root->left);
  deleteHelper(t, t->root->right);
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

//부모가 된 z를 x 로 가져온다
void Right_Rotate(rbtree *t, node_t *x){
  node_t *y;
  y = x->left;
  x->left = y->right;
  if(y->right!=t->nil){
    //y에 애가 딸린경우 애들을 x의 자식으로 붙여준다
    y->right->parent = x;
  }
  //그다음 내부모와 내오른쪽 자식을바꿔준다.
  y->parent = x->parent;
  //내부모가 닐노드라면(부모가없어..)
  if(x->parent == t->nil){
    t->root = y;
  }//부모의 위치 따짐
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
  y = x->right;
  x->right = y->left;
  if(y->left!=t->nil){
    //y에 애가 딸린경우 애들을 x의 자식으로 붙여준다
    y->left->parent = x;
  }
  //그다음 내부모와 내오른쪽 자식을바꿔준다.
  y->parent = x->parent;
  //내부모가 닐노드라면(부모가없어..)
  if(x->parent == t->nil){
    t->root = y;
  }//부모의 위치 따짐
  else if(x==x->parent->left){
    x->parent->left = y;
  }
  else{
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void insert_fixup(rbtree *t, node_t *z){
  //y는 삼촌
  node_t *y;
  while(z->parent->color==RBTREE_RED)
  { // 부모가 할배의 왼쪽에 있다면 삼촌은 할배의 오른쪽자식
    if(z->parent == z->parent->parent->left)
    {
        y = z->parent->parent->right;
        //1번케이스 : 삼촌이 빨갛다면
        if(y->color==RBTREE_RED){
          //할배검정색이랑 부모/삼촌색이랑 바꿈
          z->parent->color = RBTREE_BLACK;
          y->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          //할배가 빨간색됐으니 책임전가
          z = z->parent->parent;
        } // 2번케이스 : 내가 부모의 오른쪽이라면(꺽인상태)
        else{ 
          if(z==z->parent->right){
            z = z->parent;
            //척추펴준다
            Left_Rotate(t, z);
          }
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          //펴있는상태가되었을때 오른쪽으로 회전해서 균형을 맞춰준다.
          Right_Rotate(t, z->parent->parent);
        }
    }
    else{
        y = z->parent->parent->left;
          //1번케이스 : 삼촌이 빨갛다면
          if(y->color==RBTREE_RED){
            //할배검정색이랑 부모/삼촌색이랑 바꿈
            z->parent->color = RBTREE_BLACK;
            y->color = RBTREE_BLACK;
            z->parent->parent->color = RBTREE_RED;
            //할배가 빨간색됐으니 책임전가
            z = z->parent->parent;
          } // 2번케이스 : 내가 부모의 오른쪽이라면(꺽인상태)
          else{ 
            if(z==z->parent->left){
              z = z->parent;
              //척추펴준다
              
              Right_Rotate(t, z);
            }
            z->parent->color = RBTREE_BLACK;
            z->parent->parent->color = RBTREE_RED;
            //펴있는상태가되었을때 오른쪽으로 회전해서 균형을 맞춰준다.
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
  while(x!=t->nil)  // 빈 트리가 아닐 때
  {//y에 x의 부모값을 저장
    y = x;
    if(key <= x->key)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }
  z->parent = y;
  //y가 x 의 부모니까 부모가 nil 노드라면 지금넣는 노드를 루트에 넣는다.
  if(y==t->nil){
    t->root = z;
  }
  else if(z->key < y->key){ //현재 노드의 값이 부모의 값보다 작다면 부모 왼쪽에 현재노드를 넣는다.
    y->left = z;
  }
  else{
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
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
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  // node_t *cur;
  // while(cur!=t->nil){
  //   cur = cur->right;
  // }
  return t->root;
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
    t->root = u;
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
        //형제를 빨간색으로 바꿈
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else if( w->right->color == RBTREE_BLACK){
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
      else if(w->left->color == RBTREE_BLACK){
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
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
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
    x = z->right;
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
    if(y->parent == z){
      x->parent = y;
    }
    else{
      RB_Transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    //////////////////////////////////////////////////////////////////////
    RB_Transplant(t,z,y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
    // 실제로 삭제되는 노드가 y로 변경됨
    to_delete = y;
  }

  // 여기서 메모리 해제
  free(to_delete);

  if(y_origin_color == RBTREE_BLACK){
    RB_Delete_Fixup(t,x);
  }
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
    // TODO: implement to_array
  node_t *cur, *pre;
  cur = t->root;
  size_t i = 0;
  while(cur != t->nil && i<n){
    if(cur->left == t->nil){
      arr[i++] = cur->key;
      cur = cur->right;
     
    }
    else{
      pre = cur->left;
      while(pre->right != t->nil && pre->right != cur){
        pre = pre->right;
      }

    if(pre->right == t->nil){
      pre->right = cur;
      cur = cur->left;
    }else{
      pre->right =t->nil;
      arr[i++] = cur->key;
      cur = cur->right;
    }
    }
  }
  return 0;
}
