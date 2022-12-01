#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

struct Node {
  void* data;
  struct Node* next;
};

typedef struct Node Node;
Node* Node_append(Node* this, void* data);
void Node_init(Node* this);
void Node_dispose(Node* this);

struct Poly {
  Node terms;
};

typedef struct Poly Poly;
void Poly_add(Poly* this, Poly* that, Poly* out);
void Poly_addTerm(Poly* this, double coefficient, int power);
void Poly_print(Poly* this);
Poly* Poly_new();
void Poly_dispose(Poly* this);
void Poly_buildInteractively(Poly* this);

struct Term {
  double coefficient;
  int power;
};

typedef struct Term Term;

int main() {
  Poly* pa = Poly_new();
  puts("first polynomial");
  Poly_buildInteractively(pa);
  puts("first polynomial is: ");
  Poly_print(pa);

  Poly* pb = Poly_new();
  puts("second polynomial");
  Poly_buildInteractively(pb);
  puts("second polynomial is: ");
  Poly_print(pb);

  Poly* pc = Poly_new();
  Poly_add(pa, pb, pc);
  puts("sum:");
  Poly_print(pc);

  Poly_dispose(pa);
  Poly_dispose(pb);
  Poly_dispose(pc);

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

void Poly_addTerm(Poly* this, double coefficient, int power) {
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

  bool notFound = findSamePowerInOut != NULL;
  if (notFound) {
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

void Node_init(Node* this) {
  this->next = NULL;
  this->data = (void*) 0x1b91b9;
}

Poly* Poly_new() {
  Poly* this = (Poly*) malloc(sizeof(Poly));
  Node_init(&this->terms);
  return this;
}

void Poly_add(Poly* this, Poly* that, Poly* out) {
  for (Node* p = this->terms.next; p != NULL; p = p->next) {
    Term* thisT = (Term*) p->data;
    Poly_addTerm(out, thisT->coefficient, thisT->power);
  }

  for (Node* p = that->terms.next; p != NULL; p = p->next) {
    Term* thatT = (Term*) p->data;
    Poly_addTerm(out, thatT->coefficient, thatT->power);
  }
}

void Poly_dispose(Poly* this) {
  for (Node* p = this->terms.next; p != NULL; p = p->next) {
    free(p->data);
  }
  Node_dispose(&this->terms);
}

void Node_dispose(Node* this) {
  if (this->next == NULL) {
    free(this);
    return;
  } else {
    Node_dispose(this->next);
  }
}

void Poly_buildInteractively(Poly* this) {
  double c;
  int p, cc;
  puts("enter term count");
  scanf_s("%d", &cc);
  printf("%d terms\n", cc);
  while (cc--) {
    scanf_s("%lf %d", &c, &p);
    Poly_addTerm(this, c, p);
  }
}
