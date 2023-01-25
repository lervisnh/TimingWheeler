#include "timerSlot.h"
#include <utility>

namespace _timer {

void list_init_head(Node &node) { node.prev = &node; };
bool is_list_head(Node &node) { return &node == node.prev; };
/**
 * @brief Add the node to the list's head
 *
 * @param head the list's head, not node
 * @param node the added node
 */
void list_head_add_node(Node &head, Node &node) {
  node.prev = &head, node.next = head.next;
  head.next->prev = &node, head.next = &node;
};

void list_init_tail(Node &node) { node.next = &node; };
bool is_list_tail(Node &node) { return &node == node.next; };
/**
 * @brief Add the node to the list's tail
 *
 * @param tail the list's tail, not node
 * @param node the added node
 */
void list_tail_add_node(Node &tail, Node &node) {
  node.prev = tail.prev, node.next = &tail;
  tail.prev->next = &node, tail.prev = &node;
};

void List::init() {
  list_init_head(head);
  list_init_tail(tail);
  head.next = &tail;
  tail.prev = &head;
  counter = 0;
};

List::List() { init(); };
List::~List(){};

void List::add_front(Node &node) {
  list_head_add_node(head, node);
  ++counter;
};

void List::add_back(Node &node) {
  list_tail_add_node(tail, node);
  ++counter;
};

void List::merge_front(List &other) {
  other.tail.prev->next = this->head.next;
  this->head.next->prev = other.tail.prev;

  this->counter += other.counter;
  this->head = other.head;

  other.init();
};

void List::merge_back(List &other) {
  this->tail.prev->next = other.head.next;
  other.head.next->prev = this->tail.prev;

  this->counter += other.counter;
  this->tail = other.tail;

  other.init();
};

counter_t List::size() { return this->counter; };

bool List::empty() { return 0 == size(); };

void List::swap(List &other) {
  Node tmp_head = this->head, tmp_tail = this->tail;
  this->head = other.head, this->tail = other.tail;
  other.head = tmp_head, other.tail = tmp_tail;
  counter = other.counter;
  other.counter = 0;
};

}; // namespace _timer
