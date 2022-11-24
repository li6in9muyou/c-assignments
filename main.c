#include <stdio.h>
#include <malloc.h>

struct Node {
  void* data;
  struct Node* next;
};

typedef struct Node Node;
Node* Node_append(Node* this, void* data);
void Node_new(Node* this);

struct Poly {
  Node terms;
};

typedef struct Poly Poly;
void Poly_add(Poly* this, Poly* that, Poly* out);
void Poly_appendTerm(Poly* this, double coefficient, int power);
void Poly_print(Poly* this);
void Poly_new(Poly* this);

struct Term {
  double coefficient;
  int power;
};

typedef struct Term Term;

int main() {
  Poly pa;
  Poly_new(&pa);
  Poly_appendTerm(&pa, 2.7, 0);
  Poly_appendTerm(&pa, 1.7, -1);
  Poly_print(&pa);

  Poly pb;
  Poly_new(&pb);
  Poly_appendTerm(&pb, 27, 2);
  Poly_appendTerm(&pb, 100.7, 0);
  Poly_appendTerm(&pb, -1, 3);
  Poly_print(&pb);

  Poly pc;
  Poly_new(&pc);
  Poly_add(&pa, &pb, &pc);
  Poly_print(&pc);

  return 0;
}

void Poly_print(Poly* this) {
  Node* t = this->terms.next;
  if (t == NULL) {
    printf("0\n");
    return;
  }
  Node* p = this->terms.next;
  Term* tt = (Term*) p->data;
  printf("%.2f * x ^ %i\n", tt->coefficient, tt->power);
  for (p = p->next; p != NULL; p = p->next) {
    tt = (Term*) p->data;
    printf("+ %.2f * x ^ %i\n", tt->coefficient, tt->power);
  }
}

void Poly_appendTerm(Poly* this, double coefficient, int power) {
  Term* newTerm = (Term*) malloc(sizeof(Term));
  newTerm->power = power;
  newTerm->coefficient = coefficient;

  Term* findSamePowerInOut = NULL;
  for (Node* q = this->terms.next; q != NULL; q = q->next) {
    Term* t = (Term*) q->data;
    if (t->power == power) {
      findSamePowerInOut = t;
      break;
    }
  }
  if (findSamePowerInOut != NULL) {
    findSamePowerInOut->coefficient += coefficient;
  } else {
    Node_append(&this->terms, newTerm);
  }
}

Node* Node_append(Node* this, void* data) {
  Node* newElement = (Node*) malloc(sizeof(Node));
  newElement->data = data;
  newElement->next = NULL;

  Node* p = this;
  while (p != NULL && p->next != NULL) {
    p = p->next;
  }
  p->next = newElement;
  return this;
}

void Node_new(Node* this) {
  this->next = NULL;
  this->data = (void*) 0x1b91b9;
}

void Poly_new(Poly* this) {
  Node_new(&this->terms);
}

void Poly_add(Poly* this, Poly* that, Poly* out) {
  for (Node* p = this->terms.next; p != NULL; p = p->next) {
    Term* t = (Term*) p->data;
    Poly_appendTerm(out, t->coefficient, t->power);
  }

  for (Node* p = that->terms.next; p != NULL; p = p->next) {
    Term* thatT = (Term*) p->data;
    Poly_appendTerm(out, thatT->coefficient, thatT->power);
  }
}
