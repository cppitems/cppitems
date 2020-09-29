10 // item status
# Association of data and operations
Looking at the example library introduced in item [001](https://cppitems.github.io/#/item/001), extending it by a `grid_resize` function to change the size of the dynamic array, and switching to a header-only implementation looks like
```pmans
struct Grid {
  int nx;
  int ny;
  double *data;
};

void grid_init(Grid *that) {...}
void grid_free(Grid *that) {...}
double *grid_at(Grid *that, int x, int y) {...}
void grid_resize(struct Grid *that, size_t x, size_t y) {...}
````
and a usage could look like
```pmans
Grid grid;
grid_init(&grid);
*grid_at(&grid, 0, 0) = 5;
grid_resize(&grid, 10, 10);
*grid_at(&grid, 9, 9) = 5;
grid_free(&grid);
```
where it is apparent that all functions are tightly connected and only useful in combination with a `grid` object.
As the *free functions* live outside of the structure this object is passed as `that` argument.

## Member functions

Of course, in C++ the go-to solution is to use *non-static member functions*.
And for example `grid_at`, which looked like
```pmans
double *grid_at(Grid *that, int x, int y) {
  return &that->data[x + that->nx * y];
}
```
transforms to
```pmans
struct Grid {
  /// ...
  double *at(int x, int y) { 
    return &this->data[x + this->nx * y]; }
  /// ...
}
```
which makes use of the *implicit object parameter* `this` and a usage now looks like
```pmans
Grid grid;
grid.init(); 
*grid.at(0, 0) = 5;
grid.resize(10, 10);
*grid.at(9, 9) = 5;
grid.free();
```
which is nice: 
- no noisy argument `that` (using implicit `this` instead) 
- no need for prefix `grid_` anymore

## Object lifecyle
Up to now in our usage the lifecycle (i.e., period of accessibility) of our `grid` starts with it's construction at (1) and ends togehter with the local scope at (2):
```pmans
{
  Grid grid; // (1)
  ...
  grid.free();
} // (2)
```
We did not yet make explicit use of the *special member functions*: *constructors* and *destructors*.

### Construction

The construction is implicitly converted to
```pmans
Grid grid = Grid(); // implicit default constructor
```
which *default-constructs* a `grid` object.
As we did not specify anything, the "cheapest" option for the construction is the default in the standard, which means: 
All members are default-constructed, too.
If a member is a *fundamental type* (*build-in type*) there is no guarantee for the initial value. 
For non-fundamental types it depends on the respective default constructors, again.

> What are examples for fundamental types?

To make this implicit behaviour explicit we could mimic the consequences by implementing a default constructor like
```pmans
struct Grid {
  int nx;
  int ny;
  double *data;     
  Grid() : nx(126), ny(-23), data(-23) { // initializer list
      // constructor body
  }
  ...
}
```
which sets arbitray default values using the *member initializer list*.

> What are reasons to prefer the member initializer list over an initialization in the constructor body?

For our usage this means that after (1) the grid might have undefined state at (2), before the `init` member is called at (3) which creates a "proper" state
```pmans
Grid grid;   // (1) implicit default-construction
/// ...          // (2) 
grid.init(); // (3) init
```
This is not ideal as a "non-proper" object state is accessible at (2).

### Destruction
A similar situation is encountred towards the end of he lifetime.
After (1) the grid is probably in a "useless" state but still accessible at (2) until it is automatically destructed when the accessibility (of the *name*) ends with the local scope.
```pmans
{
  ...
  grid.free(); // (1) 
  ...          // (2)
}              // (3) implicit desctructor
```
The implicit destructor looks like
```pmans
struct Grid {
  ...   
  ~Grid()  { 
    // empty destructor body
  }
  ...
}
```
and performs no explicit action but: the deconstruction mechanism (like the construction mechanism) triggers the invocation of destructors of all members.

> Why does a destructor not take arguments?

## what about ctor and dtor for base classes?