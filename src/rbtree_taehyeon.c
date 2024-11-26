#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));

  nil_node->color = RBTREE_BLACK;
  nil_node->key = 0;
  nil_node->left = NULL;
  nil_node->right = NULL;
  nil_node->parent = NULL;
  p->nil = nil_node;
  p->root = p->nil;

  return p;
}

void delete_node(node_t *node, node_t *nil) {
  if (node != nil) {
    delete_node(node->left, nil);
    delete_node(node->right, nil);
    free(node);
  } 
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t != NULL) {
    delete_node(t->root, t->nil);
    free(t->nil);
    free(t);
  }
}

void turn_left(rbtree *t, node_t *cur) {
  node_t *temp = cur->right;
  cur->right = temp->left;
  cur->right->parent = cur;
  if (cur->parent == t->nil) {
    t->root = temp;
  } 
  else if (cur->parent->left == cur) {
    cur->parent->left = temp;
  } 
  else {
    cur->parent->right = temp;
  }
  temp->parent = cur->parent;
  temp->left = cur;
  cur->parent = temp;
}

void turn_right(rbtree *t, node_t *cur) {
  node_t *temp = cur->left;
  cur->left = temp->right;
  cur->left->parent = cur;
  if (cur->parent == t->nil) {
    t->root = temp;
  } 
  else if (cur->parent->right == cur) {
    cur->parent->right = temp;
  } 
  else {
    cur->parent->left = temp;
  }
  temp->parent = cur->parent;
  temp->right = cur;
  cur->parent = temp;
}

void Fix_rbtree(rbtree *t, node_t *cur) {
  while (cur->parent->color == RBTREE_RED) {
    if (cur->parent->parent->left->color == cur->parent->parent->right->color) {
      cur->parent->parent->left->color = RBTREE_BLACK;
      cur->parent->parent->right->color = RBTREE_BLACK;
      cur->parent->parent->color = RBTREE_RED;
      cur = cur->parent->parent;
    } 
    else {
      if (cur->parent->parent->left == cur->parent) {
        if (cur->parent->right == cur) {
          turn_left(t, cur->parent);
          cur = cur->left;
        }
        cur->parent->color = RBTREE_BLACK;
        cur->parent->parent->color = RBTREE_RED;
        turn_right(t, cur->parent->parent);
      } 
      else {
        if (cur->parent->left == cur) {
          turn_right(t, cur->parent);
          cur = cur->right;
        }
        cur->parent->color = RBTREE_BLACK;
        cur->parent->parent->color = RBTREE_RED;
        turn_left(t, cur->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->parent = t->nil;

  node_t *cur = t->root;
  node_t *pre = NULL;

  if (t->root == t->nil) {
    t->root = new_node;
    t->root->color = RBTREE_BLACK;
    return new_node;
  }
  while (cur != t->nil) {
    pre = cur;
    if (cur->key <= key) {
      cur = cur->right;
    } 
    else if (cur->key > key) {
      cur = cur->left;
    }
  }
  new_node->parent = pre;
  if (pre->key <= key) {
    pre->right = new_node;
  } 
  else {
    pre->left = new_node;
  }
  Fix_rbtree(t, new_node);
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *cur = t->root;
  while (cur != t->nil) {
    if (cur->key == key) {
      return cur;
    } 
    else if (cur->key > key) {
      cur = cur->left;
    } 
    else {
      cur = cur->right;
    }
  } return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  node_t *pre = NULL;
  while (cur != t->nil) {
    pre = cur;
    cur = cur->left;
  }
  return pre;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  node_t *pre = NULL;
  while (cur != t->nil) {
    pre = cur;
    cur = cur->right;
  }
  return pre;
}

void case1(rbtree *t, node_t *cur, int is_right) {
  if (is_right == 1) {
    cur->left->color = RBTREE_BLACK;
    cur->color = RBTREE_RED;
    turn_right(t, cur);
  }
  else {
    cur->right->color = RBTREE_BLACK;
    cur->color = RBTREE_RED;
    turn_left(t, cur);
  }
}

void case2(rbtree *t, node_t *cur, int is_right) {
  if (is_right == 1) {
    cur->left->color = cur->color;
    cur->color = RBTREE_BLACK;
    cur->left->left->color = RBTREE_BLACK;
    turn_right(t, cur);
  }
  else {
    cur->right->color = cur->color;
    cur->color = RBTREE_BLACK;
    cur->right->right->color = RBTREE_BLACK;
    turn_left(t, cur);
  }
}

void case3(rbtree *t, node_t *cur, int is_right) {
  if (is_right == 1) {
    cur->left->color = RBTREE_RED;
    cur->left->right->color = RBTREE_BLACK;
    turn_left(t,cur->left);
  }
  else {
    cur->right->color = RBTREE_RED;
    cur->right->left->color = RBTREE_BLACK;
    turn_right(t,cur->right);
  }
}

void case4(rbtree *t, node_t *cur, int is_right) {
  if (is_right == 1) {
    cur->left->color = RBTREE_RED;
  }
  else {
    cur->right->color = RBTREE_RED;
  }
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *nil = t->nil;
  node_t *cur = p->parent;
  color_t color;
  int is_right;
  //둘다 자식이 있는 경우
  if (p->left != nil && p->right != nil) {
    //후임자 찾기
    node_t *node = p->right;
    while (node->left != nil) {
      node = node->left;
    }
    //후임자의 엄마를 cur 로 설정
    cur = node->parent;
    //후임자의 색 저장
    color = node->color;
    //후임자의 키를 냅다 삭제하려는 키에 넣어버린다.
    p->key = node->key;
    //후임자가 엄마의 왼쪽 자식이면
    if (node->parent->left == node) {
      node->parent->left = node->right;
      if (node->right != nil) {
        node->right->parent = node->parent;
      }
      is_right = 0;
    } //후임자가 엄마의 오른쪽 자식이면
    else {
      node->parent->right = node->right;
      if (node->right != nil) {
        node->right->parent = node->parent;
      }
      is_right = 1;
    }
    free(node);
  } //자식이 없는 경우
  else if (p->left == nil && p->right == nil) {
    color = p->color;
    if (p == t->root) {
      t->root = t->nil;
    } //부모의 왼쪽에 있을땐 is_right = 0 
    else if (p->parent->left == p) {
      //extraBlack 부여
      p->parent->left = nil;
      is_right = 0;
    } 
    else {//부모의 오른쪽에 있을 땐 is_right = 1
      //extraBlack 부여
      p->parent->right = nil;
      is_right = 1;
    }//일단 냅다 삭제 
    free(p);
  } 
  else {
    color = p->color;
    if (p == t->root) {
      if (p->left != nil) {
        t->root = p->left;
        p->left->parent = nil;
      } 
      else {
        t->root = p->right;
        p->right->parent = nil;
      }
      free(p);
    } 
    else if (p->parent->left == p) {
      is_right = 0;
      if(p->left == nil) {
        p->parent->left = p->right;
        p->right->parent = p->parent;
        free(p);
      } 
      else {        
        p->parent->left = p->left;
        p->left->parent = p->parent;
        free(p);
      }
    } 
    else {
      is_right = 1;
      if(p->left == nil) {
        p->parent->right = p->right;
        p->right->parent = p->parent;
        free(p);
      } 
      else {
        p->parent->right = p->left;
        p->left->parent = p->parent;
        free(p);
      }
    }
  }
  //컬러를 보는 부분
  //삭제하려는 노드 컬러가 레드일경우
  if (color == RBTREE_RED) {
    return 0;
  } 
  else {//삭제하려는 노드 컬러가 블랙일경우
    while (1) {//cur 은 엄마노드
      if (cur == nil) {
        t->root->color = RBTREE_BLACK;
        return 0;
      }
      else if (is_right == 1) {
        if (cur->right->color == RBTREE_RED) {
          cur->right->color = RBTREE_BLACK;
          return 0;
        }
        if (cur->left->color == RBTREE_RED) {
          case1(t, cur, is_right);
        }
        else if (cur->left->color == RBTREE_BLACK) {
          if (cur->left->left->color == RBTREE_RED) {
            case2(t, cur, is_right);
            if (cur->parent->color == RBTREE_RED) {
            }
            return 0;
          }
          else {
            if (cur->left->right->color == RBTREE_RED) {
              case3(t, cur, is_right);
            }
            else {
              case4(t, cur, is_right);
              if (cur->parent->left == cur) {
                is_right = 0;
              }
              else {
                is_right = 1;
              }
              cur = cur->parent;
            }
          }
        }
      } 
      else {
        if (cur->left->color == RBTREE_RED) {
          cur->left->color = RBTREE_BLACK;
          return 0;
        }
        if (cur->right->color == RBTREE_RED) {
          case1(t, cur, is_right);
        }
        else if (cur->right->color == RBTREE_BLACK) {
          if (cur->right->right->color == RBTREE_RED) {
            case2(t, cur, is_right);
            return 0;
          }
          else {
            if (cur->right->left->color == RBTREE_RED) {
              case3(t, cur, is_right);
            }
            else {
              case4(t, cur, is_right);
              if (cur->parent->left == cur) {
                is_right = 0;
              }
              else {
                is_right = 1;
              }
              cur = cur->parent;
            }
          }
        }
      }
    }
  }
}

void recursive_make_array(rbtree *t, key_t *arr, node_t *node, int* n) {
  if (node != t->nil) {
    recursive_make_array(t, arr, node->left, n);
    arr[*n] = node->key;
    (*n)++;
    recursive_make_array(t, arr, node->right, n);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t *cur = t->root;
  int num = 0;
  recursive_make_array(t, arr, cur, &num);
  return 0;
}

////////////////////////////////////////////////////////////////
void printTree(rbtree *t, node_t *node){
    if(t == NULL || node == t->nil) return;

    printTree(t, node->left);
    printf(" %d(%d) ", node->key,node->color);
    printTree(t, node->right);
}
////////////////////////////////////////////////////////////////