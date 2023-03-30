#include "timerSlot.h"
#include <utility>

namespace _timer {

void list_init_head(Node &node) { node.prev = &node; };
bool is_list_head(Node &node) { return &node == node.prev; };
/**
 * @brief Add the node to the list's head_
 *
 * @param head_ the list's head_, not node
 * @param node the added node
 */
void list_head_add_node(Node &head_, Node &node) {
  node.prev = &head_, node.next = head_.next;
  head_.next->prev = &node, head_.next = &node;
};

void list_init_tail(Node &node) { node.next = &node; };
bool is_list_tail(Node &node) { return &node == node.next; };
/**
 * @brief Add the node to the list's tail_
 *
 * @param tail_ the list's tail_, not node
 * @param node the added node
 */
void list_tail_add_node(Node &tail_, Node &node) {
  node.prev = tail_.prev, node.next = &tail_;
  tail_.prev->next = &node, tail_.prev = &node;
};

void List::init() {
  list_init_head(head_);
  list_init_tail(tail_);
  head_.next = &tail_;
  tail_.prev = &head_;
  counter = 0;
};

List::List() { init(); };
List::~List(){};

void List::add_front(Node &node) {
  list_head_add_node(head_, node);
  ++counter;
};

void List::add_back(Node &node) {
  list_tail_add_node(tail_, node);
  ++counter;
};

void List::merge_front(List &other) {
  other.tail_.prev->next = head_.next;
  head_.next->prev = other.tail_.prev;

  counter += other.counter;
  head_ = other.head_;

  other.init();
};

void List::merge_back(List &other) {
  tail_.prev->next = other.head_.next;
  other.head_.next->prev = tail_.prev;

  counter += other.counter;
  tail_ = other.tail_;

  other.init();
};

counter_t List::size() { return counter; };

bool List::empty() { return 0 == size(); };

void List::swap(List &other) {
  Node tmp_head = head_, tmp_tail = tail_;
  head_ = other.head_, tail_ = other.tail_;
  other.head_ = tmp_head, other.tail_ = tmp_tail;
  counter = other.counter;
  other.counter = 0;
};

}; // namespace _timer
