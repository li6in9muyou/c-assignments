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
  Node coefficients;
  Node powers;
};

typedef struct Poly Poly;
void Poly_add(Poly* this, Poly* that, Poly* out);
void Poly_appendTerm(Poly* this, double coefficient, int power);
void Poly_print(Poly* this);
void Poly_new(Poly* this);

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
  Node* c = this->coefficients.next;
  Node* p = this->powers.next;
  if (c == NULL || p == NULL) {
    printf("0\n");
    return;
  }
  while (1) {
    printf("%.2f * x ^ %i\n", *(double*) c->data, *(int*) p->data);
    c = c->next;
    p = p->next;
    if (c == NULL || p == NULL) {
      break;
    } else {
      printf("+ ");
    }
  }
}

void Poly_appendTerm(Poly* this, double coefficient, int power) {
  double* c = (double*) malloc(sizeof(double));
  *c = coefficient;
  int* p = (int*) malloc(sizeof(power));
  *p = power;
  Node_append(&this->coefficients, c);
  Node_append(&this->powers, p);
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
  Node_new(&this->coefficients);
  Node_new(&this->powers);
}

void Poly_add(Poly* this, Poly* that, Poly* out) {
  Node* p = this->powers.next;
  Node* c = this->coefficients.next;
  while (1) {
    Poly_appendTerm(out, *(double*) c->data, *(int*) p->data);
    p = p->next;
    c = c->next;
    if (p == NULL || c == NULL) {
      break;
    }
  }

  Node* thatC = that->coefficients.next;
  Node* thatP = that->powers.next;
  while (1) {
    double thatCoef = *(double*) thatC->data;
    int thatPow = *(int*) thatP->data;

    int thatGotSamePower = 0;

    Node* outP = out->powers.next;
    Node* outC = out->coefficients.next;
    while (1) {
      if (*(int*) outP->data == thatPow) {
        thatGotSamePower = 1;
        break;
      }
      outP = outP->next;
      outC = outC->next;
      if (outP == NULL || outC == NULL) {
        break;
      }
    }

    if (thatGotSamePower) {
      Node* op = out->powers.next;
      Node* oc = out->coefficients.next;
      while (1) {
        if (*(int*) op->data == thatPow) {
          *(double*) oc->data += thatCoef;
          break;
        }
        op = op->next;
        oc = oc->next;
        if (op == NULL || oc == NULL) {
          break;
        }
      }
    } else {
      Poly_appendTerm(out, thatCoef, thatPow);
    }

    thatP = thatP->next;
    thatC = thatC->next;
    if (thatP == NULL || thatC == NULL) {
      break;
    }
  }
}
