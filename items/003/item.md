10 \\ item status
# ...
In C++ every *entity* that can be "used" has a *type*.
This type controls *how* an entity can be "used" and what it excatly means when it is used in a certain semantic embedding.

For the *fundamental types* it is intuitively clear what to expect in most situations.
```pmans
{
  double a = 2.0;
  double b = 2.0;
  double c = (a + b) / 8.0 * 3.5;
}
```
> What are potential problems with arithmetic types?

## Class types

Let's further pick the group of *class types* and restrict it to a subset:
- no base classes
- only public members 
- only non-static members 
- no user-defined constructors/destructor
- no user-defined operators
- no virtual functions

An example of this subset could look like
```pmans
struct ClassA {
    double a;
    int b;
    ClassB c;
}
```


## Fundamental types

## Class types

## Links
- C++ type categories: https://en.cppreference.com/w/cpp/language/type)