1 // item status
# Exercise EX3.3

The exercise is available as a git repository at:
https://github.com/cppitems/ex3.3

# Possible solution

## Sync using atomics (and explicit constraints/relaxations)

```pmans
namespace sync_atomics {
template <typename T> class shared_ptr {
[...]
public:
  using SharedOwners = /*b*/ std::atomic<long> /*x*/;
  SharedOwners *shared_owners = nullptr;
  long /*b*/ atomic_increment /*x*/(SharedOwners *shared_owners,
                        std::memory_order atomic_fence) {
    return shared_owners->fetch_add(+1, atomic_fence) + 1;
  }
  long /*b*/ atomic_decrement /*x*/(SharedOwners *shared_owners,
                        std::memory_order atomic_fence) {
    return shared_owners->fetch_add(-1, atomic_fence) - 1;
  }
  long /*b*/ atomic_load /*x*/(const SharedOwners *shared_owners,
                   std::memory_order atomic_fence) const {
    return shared_owners->load(atomic_fence);
  }

[...]

  shared_ptr(const shared_ptr &other)
      : ptr(other.ptr), shared_owners(other.shared_owners) {
     /*b*/ atomic_increment /*x*/(shared_owners, /*b*/ std::memory_order_relaxed /*x*/);
  }
  ~shared_ptr() {
    if (shared_owners) {
      if (/*b*/ atomic_decrement /*x*/(shared_owners, /*b*/ std::memory_order_acq_rel /*x*/) == -1) {
        delete shared_owners;
        delete ptr;
      }
    }
  }
  long use_count() const {
    if (shared_owners) {
      return /*b*/ atomic_load /*x*/(shared_owners, /*b*/ std::memory_order_relaxed /*x*/) + 1;
    } else {
      return 0;
    }
  }
[...]
};
} // namespace sync_atomics
```

## Sync using critical sections

```pmans
namespace sync_critical_sections {
template <typename T> class shared_ptr {
[...]
  /*b*/ using CriticalSection = std::mutex /*x*/;
  /*b*/ using Lock = std::lock_guard<CriticalSection> /*x*/;
  /*b*/ CriticalSection *critical_section = nullptr /*x*/;

  void swap(shared_ptr &other) {
    [...]
    /*b*/ std::swap(critical_section, other.critical_section); /*x*/
  }

public:
  shared_ptr(T *ptr) : ptr(ptr) {
    [...]
    /*b*/ critical_section = new CriticalSection{}; /*x*/
  }
  shared_ptr(const shared_ptr &other)
      : [...] /*b*/ critical_section(other.critical_section) /*x*/ {
    /*b*/ Lock lock(*critical_section) /*x*/;
    ++(*shared_owners);
  }
  shared_ptr(shared_ptr &&other)
      : [...] /*b*/ critical_section(other.critical_section) /*x*/ {
    [...]
    /*b*/ other.critical_section = nullptr; /*x*/
  }
  ~shared_ptr() {
    /*b*/ bool no_owners_left = false; /*x*/
    if (shared_owners) {
      /*b*/ Lock lock(*critical_section); /*x*/
      if (--(*shared_owners) == -1) {
        no_owners_left = true;
        delete shared_owners;
        delete ptr;
      }
    }
    if (no_owners_left) {
      /*b*/ delete critical_section; /*x*/
    }
  }
  long use_count() const {
    if (shared_owners) {
      /*b*/ Lock lock(*critical_section); /*x*/
      return (*shared_owners) + 1;
    } else {
      return 0;
    }
  }
};
} // namespace sync_critical_sections
```