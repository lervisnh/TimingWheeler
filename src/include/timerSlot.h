#pragma once

#include "timerNode.h"
#include <assert.h>
#include <atomic>
#include <cstdint>

namespace _timer {

class List {
private:
  Node head;
  Node tail;

public:
  counter_t counter;
  void init();

public:
  List();
  ~List();

  void add_front(Node &node);
  void add_back(Node &node);

  void merge_front(List &other);
  void merge_back(List &other);

  template <typename T> bool pop_front(T *&item) {
    if (!empty()) {
      Node *node = head.next;

      head.next = node->next;
      head.next->prev = &head;
      --counter;

      node->next = nullptr, node->prev = nullptr;
      item = dynamic_cast<T *>(node);
      return true;
    }
    return false;
  };

  template <typename T> bool pop_back(T *&item) {
    if (!empty()) {
      Node *node = tail.prev;

      tail.prev = node->prev;
      tail.prev->next = &tail;
      --counter;

      node->next = nullptr, node->prev = nullptr;
      item = dynamic_cast<T *>(node);
      return true;
    }
    return false;
  };

  bool empty();
  void swap(List &other);
  counter_t size();
};

}; // namespace _timer
