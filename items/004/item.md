10 // item status
# Object behaviour: lifecycle hooks
Looking at the example library introduced in item [001](https://cppitems.github.io/#/item/001) which looks like
```pmans
struct Grid {
  int nx;
  int ny;
  double *data;
};

void grid_init(Grid *that) {...} // allocates dynamic memory
void grid_free(Grid *that) {...}
double *grid_at(Grid *that, int x, int y) {...}
````
and a usage could look like
```pmans
{
  Grid grid;
  grid_init(&grid);
  *grid_at(&grid, 0, 0) = 5;
  grid_free(&grid);
}
```
it is apparent that all functions are tightly connected and only useful in combination with a `grid` object.
As the *free functions* live outside of the structure so this `grid` object is passed as `that` argument.

> Why might this usage be considered error-prone/inconvenient?

## Member functions

Of course, in C++ the go-to solution is to use *non-static member functions*
for `grid_at`. So `grid_at` which looks like 
```pmans
double */*b*/ grid_at /*x*/(Grid */*f*/ that /*x*/, int x, int y) {
  return &/*f*/ that /*x*/->data[x + /*f*/ that /*x*/->nx * y];
}
```
is changed to
```pmans
struct Grid {
  /// ...
  double */*b*/ at /*x*/(int x, int y) { 
    return &/*f*/ this /*x*/->data[x + /*f*/ this /*x*/->nx * y]; }
  /// ...
}
```
which makes use of the *implicit object parameter* `this`. This is nice:
- no noisy argument `that` (using implicit `this` instead) 
- no need for prefix `grid_` anymore (function is accessible through the object)

## Special member functions 

To set a "proper" object state `grid_init` is used (which also performs a dynamic memory allocation).
And `grid_free` is used to manually free this dynamic memory at the end of the usage of the object.
Of course, the go-to solution is to change this and make use of the special member functions for construction and deconstruction: *constructors* and *destructors*.

So `grid_init` and `grid_free` which look like
```pmans
void /*b*/ grid_init /*x*/(Grid *that) {
  that->nx = 3;
  that->ny = 4;
  that->data = /*f*/ std::calloc /*x*/(that->nx * that->ny, sizeof(double));
};
void /*b*/ grid_free /*x*/(Grid *that) {
  /*f*/ std::free /*x*/(that->data);
  that->nx = 0;
  that->ny = 0;
}
```
are changed to 
```pmans
struct Grid {
  int nx;
  int ny;
  double *data;
  /*b4*/ Grid() : nx(3), ny(4), data(/*f3*/ new double[nx * ny]) {
    /*f*/ std::fill_n /*x*/(data, nx * ny, 0);
  }
  /*b5*/ ~Grid() { /*f*/ delete[] /*x*/ data; }
...
}
```
Note that `calloc` and `free` are "replaced" by using `new` and `delete`.
An equivalent usage now looks like
```pmans
{
  Grid grid; // (1)
  *grid.at(0, 0) = 5;
} // (2)
```
This "works" because constructors and destructors take part in the construction and destruction mechanisms defined by the standard  and are implicitly called. 

In our case (1) is implicitly converted to a call to the default constructor
```pmans
  Grid grid = Grid();
```
which *default-constructs* a `grid` object.
As we did not specify anything, the "cheapest" option for the construction is the default in the standard, which means: 
All members are default-constructed, too.
If a member is a *fundamental type* (*build-in type*) there is no guarantee for the initial value (again, cheapest option is the default). 
For non-fundamental types it depends on their respective default constructors, again.

> What are examples for fundamental types?

If no default constructor is explicitly defined for `Grid`  we could mimic the consequences by implementing a (hypotentical) default constructor like
```pmans
struct Grid {
  int nx;
  int ny;
  double *data;     
  Grid() : nx(126), ny(-23), data(-23) {}
  ...
}
```
which is what we would have to expect.

Similar for the deconstruction: at (2) the local variable `grid` goes out of scope (i.e., its *name* is not valid outside the scope). 
This triggers the invokation of `grid`'s destructor.

```pmans
{
  Grid grid; // (1)
  *grid.at(0, 0) = 5;
} // (2) 
```
> Can a destructor have arguments?

The destruction rules first execute the body of the destructor, in our case:
```pmans
  delete[] data;
```
The `delete[]` triggers the invokation of the destructor of each object in the dynamic array.
After the execution of the body the non-static data members of the `grid` object are destructed by invoking the respective destructors, which can hypothetically look like
```pmans
  this->data.~double*();
  this->ny.~int();
  this->nx.~int();
``` 

> Why is this deconstruction order reasonable/intuitive?

> What else might have to be deconstructed?
