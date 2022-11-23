#include <stdio.h>
#include <malloc.h>

struct Node {
  void* data;
  struct Node* next;
};

typedef struct Node Node;
Node* Node_append(Node* this, void* data);
void Node_new(Node* this);
void* Node_at(Node* this, int index);

struct Poly {
  Node terms;
  int termCount;
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
  Term* term;
  term = (Term*) Node_at(&this->terms, 0);
  printf("%.2f * x ^ %i\n", term->coefficient, term->power);
  for (int i = 1; i < this->termCount; i++) {
    term = (Term*) Node_at(&this->terms, i);
    printf("+ %.2f * x ^ %i\n", term->coefficient, term->power);
  }
}

void Poly_appendTerm(Poly* this, double coefficient, int power) {
  Term* newTerm = (Term*) malloc(sizeof(Term));
  newTerm->power = power;
  newTerm->coefficient = coefficient;

  Node_append(&this->terms, newTerm);
  this->termCount += 1;
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
  this->termCount = 0;
}

void Poly_add(Poly* this, Poly* that, Poly* out) {
  Node* p = this->terms.next;
  while (1) {
    Term* tt = (Term*) p->data;
    Poly_appendTerm(out, tt->coefficient, tt->power);
    p = p->next;
    if (p == NULL) {
      break;
    }
  }

  Node* thatT = that->terms.next;
  while (1) {
    Term* thatCurrentTerm = (Term*) thatT->data;

    int thatPow = thatCurrentTerm->power;
    int thatGotSamePower = 0;

    Node* outT = out->terms.next;
    while (1) {
      Term* t = (Term*) outT->data;
      if (t->power == thatPow) {
        thatGotSamePower = 1;
        break;
      }
      outT = outT->next;
      if (outT == NULL) {
        break;
      }
    }

    if (thatGotSamePower) {
      Node* outTT = out->terms.next;
      while (1) {
        Term* t = (Term*) thatT->data;
        if (t->power == thatPow) {
          ((Term*) outTT->data)->coefficient += t->coefficient;
          break;
        }
        outTT = outTT->next;
        if (outTT == NULL) {
          break;
        }
      }
    } else {
      Poly_appendTerm(
          out,
          thatCurrentTerm->coefficient,
          thatCurrentTerm->power
      );
    }

    thatT = thatT->next;
    if (thatT == NULL) {
      break;
    }
  }
}

void* Node_at(Node* this, int index) {
  int i = 0;
  Node* p = this->next;
  while (i < index) {
    p = p->next;
    i++;
  }
  return p->data;
}
