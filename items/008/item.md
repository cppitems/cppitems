1 // item status
# Special member functions
Let's pick a simple user-defined type and consider the expected behavior (which is implicitly available) in certain semantic embeddings:
```pmans
{
  struct Widget {
    int i;
    double j;
    std::vector<double> d;
  };
  Widget a{};              // (1a) def-ctor: where is this constructor implemented?
  Widget b = Widget{};     // (1b) def-ctor: only default ctor (not construct and move/copy)
  // construction/init syntax:
  // int i{}; // C++ way
  // int i = {}; // C origin/heritage
  Widget c(a);             // (2a) copy-ctor
  Widget d = a;            // (2b)
  Widget e(std::move(a));  // (3a) move-ctor
  Widget f = std::move(a); // (3b)
  a = b;                   // (4) copy-assign
  a.operator=(T&);         // ... resolves to copy (as b is lvalue)
  a = std::move(b);        // (5) move-assign:
  a.operator=(T&&);        // ... resolved to move (as rhs is rvalue) 
} // (6) dtors
```
> Are these special member function (SMF) always implicitly available?
> - if you provide user-defined versions: some are disables, exact rules are involved

> Can they be "turned on or off" explicitly?
> - `... = delete;` "turns them off"
> - `... = default;` does not turn them on, but asks for the default (which can actually might be `deleted`)


Further common semantic embeddings are function calls:
```pmans
  void pass_byvalue(Widget w) {}; 
  void pass_lref(Widget &w) {}; 
  void pass_rref(Widget &&w) {};  
  Widget /*f1*/ a{};
  pass_byvalue(/*f1*/ a);                     // (6a) copy constructor (passing lvalue to by by-value argument)
  pass_byvalue(std::move(/*f1*/ a));          // (6b) move contructor (passing rvalue to by-value argument)
  pass_lref(/*f1*/ a);                        // (7) no SMF is involved (simply binding lvalue to lvalue-ref argument)
  pass_rref(std::move(/*f1*/ a));             // (8) no SMF is involved (simply binding an rvalue to an rvalue-ref argumemt)
  pass_rref(Widget{});                 // (9) no SMF is involved (binding + lifetime extension of temporary)
                                       // ... precise: constructor is used to create temporary
```
> Which of the special member functions are involved in (6)-(9)?
> - see inline comments above

Using the clang AST will list exactly this 6 special member functions for `Widget` which will look similar to this:
```pmans
CXXRecordDecl ... struct Widget definition
|-DefinitionData aggregate
| |-DefaultConstructor exists non_trivial // (1)
| |-CopyConstructor simple non_trivial has_const_param needs_overload_resolution implicit_has_const_param // (2)
| |-MoveConstructor exists simple non_trivial needs_overload_resolution // (3)
| |-CopyAssignment non_trivial has_const_param implicit_has_const_param // (4)
| |-MoveAssignment exists non_trivial needs_overload_resolution // (5)
| `-Destructor simple non_trivial needs_overload_resolution // (6)
```
Let's now look at each of the special member functions more closely.
## Construction
As we have seen above, which constructor is called depends on the semantic embedding.

### Default constructor
> Describe the effect of a default constructor:
> - construct object without any arguments provided

> Which are common situations where it is invoked?
> - defining local variables, .e.g., `Widget a;`


The implicitly available default constructor of `Widget` initializes all members using the respective default initialization in order of declaration and looks like this:
```pmans
  struct Widget {
    int i;
    double j;
    std::vector<double> d;
    /*b*/ Widget() /*x*/  /* compile: default initialize members */ {} // (1)
    // this means: fundamental types: not initialization -> indeterminate state
  };
```
Note: *default initialization* means calling the constructor for classes and doing nothing for non-array built-in types.

It is implicitly available if *no other* user-defined constructors are present. Example with user-defined constructors:
```pmans
  struct Widget {
    int i;
    double j;
    std::vector<double> d;
    /*b*/ Widget(int i) /*x*/ : i(i), j(i), d(i,j) {}  // user-defined ctor
    /*b*/ Widget() /*x*/  /* compile: default initialize members */ {}   // default makes still sense?
  };
```
> Why does this rule make sense?
> - the presence of a custom constructor (with arguments) indicates that a default construction might not make sense (or is even dangerous) as it might lead to an object in an undesired state.

Further, it is *not* implicitly available if `const` or reference members are present (or if members have no accessible default construction mechanism). Examples:
```pmans
  struct Widget {
    const int i;
    /*b*/ Widget() /*x*/ 
     /* compile: default initialize members */ {} // makes sense? 
                                 // no, 'i' is const and must be explicitly initialized 
  };
```
```pmans
  struct Widget {
    int &i;
    /*b*/ Widget() /*x*/ 
     /* compile: default initialize members */ {} // makes sense?
                                 // no, 'i' is reference and must be explicitly initialized
  };
```
> Does this rule make sense?
> - yes, members of reference-type cannot be default initialized
>- yes, unreasonable to default initialize const members


Default member initializers lift this last rule:
```pmans
  struct Widget {
    int &i = gobal;
    const int j = 5;
    std::vector<double> d;
    Widget() 
    : /* compile: default initialize d, use defaults for i and j */ {} // makes sense? 
                                                  // yes, defaults for 'i' and 'j' will be used
                                                  // 'd' is default initialized -> def-ctor
  };
``` 

> Why is it error-prone to not follow the declaration order for the *initializer list*?
>```pmans
>  struct Widget {
>    int i;
>    double j;
>    std::vector<double> d;
>    Widget() : j(5), i(j), d()  { // 'j' put before 'i'
>    } 
>  };
>```
> - because the order of initializations always happens in the order of declaration (and not in the order given in an user-provided initializer list).  


> What happens (for a user-defined constructor) when a member is not initialized using the *initializer list*?
>```pmans
>  struct Widget {
>    int i; 
>    double j;
>    std::vector<double> d;
>    Widget() /* compile: no initializer list */ {
>    } 
>  };
>```
> - member will be default initialized: for classes this means the default construtor is used; for built-in types (e.g., `i` and `j`) nothing is done


### Copy constructor
> Describe the effect of a copy constructor. 
> - construct an object using an lvalue reference to another object of the same type

> Which are common situations where it is invoked?
> - invoking functions with non-reference arguments


A copy constructor is implicitly available if no user-defined copy constructors are present. For `Widget` it looks like this:
```pmans
  struct Widget {
    int i;
    double j;
    std::vector<double> d;
    /*b*/ Widget(const Widget &w) /*x*/ : i(w.i), j(w.j), d(w.d) {} // (2) implicitly available
  };
```
It is *not* implicitly available if any of the members do not support copy construction (i.e., the initializer list invokes these copy constructors).
If all members support a copy constructor from a `const` reference, the argument is also declared `const`; otherwise the copy constructor looks like this:
```pmans
  struct Widget {
    /// ...
    /*b*/ Widget(Widget &w) /*x*/ : i(w.i), j(w.j), d(w.d) {} // (2)
  };
```
> Why is `const` preferred by the rules? 
>```pmans
>   const Widget cw{};
>   Widget a(cw); // does this work with Widget(Widget &w) ? -> no, cannot bind const to non-const ref
>   Widget a(Widget{}); // does this work with Widget(Widget &w) ? -> no, cannot bind rvalue to lvalue ref
>```
> - cases where an rvalue or a const object are passed do not work without `const`


Further, it is *not* implicitly available if other user-defined special member functions (beside constructors) are present.

> Informally, when is a copy constructor not available implicitly?
> - if one or more of the members do not support beeing copied
> - if a custom implementation for other SMFs are present (beside regular constructors)


### Move constructor
> Describe the effect of a move constructor.
> - construct an object using an rvalue reference to another object of the same type
> - potentially using/moving resource of the other object

> Which are common situations where it is invoked?
> - whenever an rvalue is passed in a "construction situation"

The move constructor is implicitly available if no other user-defined special member functions are present (beside user-defined constructors) and all members are "allowed to be moved".

> Why does this rule make sense?
> - destructor is present: indicating special treatment which has to be considered also during "moving"
> - copy constructor is present: possible special treatment when copying will likely also apply for move-construction
> - copy/move assignment present: again, indicating that special treatment is required also for move-construction


For `Widget` the implicit move constructor looks like this:
```pmans
  struct Widget {
    int /*f1*/ i;
    double /*f1*/ j;
    std::vector<double> /*f1*/ d;
    /*b*/ Widget(Widget &&w) /*x*/
        : /*f1*/ i(std::move(w)./*f1*/ i), 
          /*f1*/ j(std::move(w)./*f1*/ j), 
          /*f1*/ d(std::move(w)./*f1*/ d) {}          // (3)
  };
```
Comparing the signature of copy and move constructors
```pmans
  Widget(const Widget /*b1*/ &w) ... // (2)
  Widget(Widget /*b2*/ &&w) ... // (3)
```
reveals that both arguments can bind in situations like this:
```pmans
Widget w{};
Widget copy(std::move(w)); // both can bind to xvalue (=rvalue)
```
If both are available, the *overload resolution* selects 
- the copy constructor for lvalues, and
- the move constructor for rvalues.

This also means that the implicit move constructor "works" if some (or all) members only support a copy construction (and their move constructor is implicitly available = "allowed to be moved"):
```pmans
  struct Widget {
    /// ...
    /*f*/ Widget(Widget &&w) /*x*/ 
        : i/*b1*/ (std::move(w).i/*b1*/ ), // move or resolve to copy
          j/*b1*/ (std::move(w).j/*b1*/ ), // ...
          d/*b1*/ (std::move(w).d/*b1*/ )  // ...
          {}
  };
```

## Assignment

Assignment is implicitly available in form of the assignment `operator=`:
```pmans
  Widget a, b;
  a /*b1*/ = b;                        // (4) copy-assign
  a /*b1*/ = std::move(b);             // (5) move-assign
  
```
> Are there any special rules for the assignment `operator=` compared to any other non-static "ordinary" member-function? 
How do the above lines look like when using the "member-function like" syntax?
<!--
> - no, `operator=` overloads behave like "normal" members functions (but might be implicitly available a SMF)
> - above lines could be also written as `a.operator=(b);` and `a.operator=(std::move(b));`
-->

The signature and rules for the implicit availability of the copy (4) and move (5) assignment operators are very similar to the rules of their construction counterparts.
What differs most is that no initializer list is available and a reference to a `Widget` is returned:
```pmans
struct Widget {
  ...
  /*b*/ Widget /*x*/           (const Widget &other) : /* compile: do copies here */ {}
  /*b*/ Widget & /*x*/operator=(const Widget &other) { /* compile: do copies here */ /*b*/ return *this /*x*/; }

  /*b*/ Widget /*x*/           (Widget &&other) : /* compile: do moves here */ {}
  /*b*/ Widget & /*x*/operator=(Widget &&other) { /* compile: do moves here */ /*b*/ return *this /*x*/; }
};
```
This means all moving/copying is implemented in the function body where both objects (`*this` and `other`) are accessible.
Overload resolution works in the same way as for the move and copy constructors: if both (move and copy) assignment operators are available lvalue parameters resolve to the copy assignment and rvalue parameters resolve to the move assignment.

### Copy assignment
> Describe the effect of a copy assignment (operator). 
> - a copy assignment "assigns" an object to an already existing object of the same type, e.g. `a = b;`.
> - the detailed effect of the assignment depends on the implementation of the assignment operator, but commonly is is expected that `b` is not modified by the assignment and `a` will be in the same state as `b` after the assignment.



A copy assignment operator is implicitly available if *no* user-defined copy constructors are present and all member are copy-assignable. For `Widget` it looks like this:
```pmans
  struct Widget  {
    int i;
    double j;
    std::vector<double> d;
    /*b*/ Widget &operator=(const Widget &other) /*x*/{
      this->i = other.i; // requires i to be copy-assignable
      this->j = other.j; // ...
      this->d = other.d; // ...
      return *this;
    } // (4) copy-assign
  };  
```
It is *not* implicitly available if other user-defined SMFs are present (beside regular constructors).
Further, it is not available in the presence of `const` or reference type members.
```pmans
  struct Widget {
    /*b5*/ const int i;
    double /*b1*/ &j;
    /*f*/ Widget &operator=(const Widget &other) /*x*/{
        this->i = other.i;   // (a) makes sense? 
        this->j = other.j;   // (b) makes sense? 
    }
  };
``` 
> Do this rules for implicit availability make sense?
> - yes, if other SMFs are available indicates that a copy assignment might need "special care"
> - yes, if members do not support copy assignment, the implicit form of the copy assignment is not possible.


### Move assignment
> Describe the effect of a move assignment (operator). 
> - a move assignment "assigns" an object to an already existing object of the same type, e.g. `a = std::move(b);`
> - the detailed effect of the move assignment depends on the implementation of the operator
> - commonly is is expected that `a` represents the state of `b` after the assignment
> - the state of `b` after the assignment is required to be valid and destructible, nothing more, nothing less
>```pmans
>{
>  ... a,b; // assume a and b each own resources
>  a = std::move(b); // is dtor of b called after this line? -> no 
>  // does b still own resources here? -> maybe
>} // dtors: here b is deconstructed and its resources (which it still might own) are freed
>```


A move assignment operator is implicitly available if *no* other special member functions are present (beside constructors).  For `Widget` it looks like this:
```pmans
  struct Widget  {
    int i;
    double j;
    std::vector<double> d;
    /*b*/ Widget &operator=(Widget &&other) /*x*/{
      this->i = std::move(other).i; // requires i to be move-assignable
      this->j = std::move(other).j;
      this->d = std::move(other).d;
      return *this;
    } // (5) move-assign
  };  
```
If `const` or reference type members are present, or if one of the members is not move-assignable, the move assignment operator is not available implicitly.


## Forcing generation
Up to now we looked at compiler generated special member function (1) to (5) for construction and assignment which look like this for a `Widget`:
```pmans
struct Widget {
  int i;
  std::vector<double> j;
  /*b*/ Widget() /*x*/ {};                           // (1) default constructor
  /*b*/ Widget(const Widget &w) /*x*/              // (2) copy constructor
      : i(w.i),                           // member wise copy-construction
        j(w.j){};                         // ...
  /*b*/ Widget(Widget &&w) /*x*/                    // (3) move constructor
      : i(std::move(w).i),                // member wise move-construction
        j(std::move(w).j){};              // ...
  /*b*/ Widget &operator=(const Widget &w) /*x*/ { // (4) copy assignment
    this->i = w.i;                        // member wise copy assignment
    this->j = w.j;                        // ...
    return *this;                         // return reference
  };                                      //
  /*b*/ Widget &operator=(Widget &&w) /*x*/ {      // (5) move assignment
    this->i = std::move(w).i;             // member wise move assignment
    this->j = std::move(w).j;             // ...
    return *this;                         // return reference
  };                                      //
}; 
```
These special member functions are available implicitly depending on the "anatomy" of a user-defined type (see all the rules above).
If a special member function is not implicitly available for a classit will be generated by the compiler as *inline public member* of the class.

The user can force generation for any of the special member function like this:
```pmans
struct /*f7*/ Widget {
  int i;
  std::vector<double> j;
  Widget() = /*b7*/ default;
  Widget(const Widget &w) = /*b7*/ default;
  Widget(Widget &&w) = /*b7*/ default;
  Widget &operator=(const Widget &w) = /*b7*/ default;
  Widget &operator=(Widget &&w) = /*b7*/ default;
};
```
For `Widget`, this will add exactly the implementations from above. 
For a different type with a different "anatomy" this might be different:
```pmans
struct Other {
    int /*b1*/ &i; // reference type implicitly "deletes" some of the SMFs
}
```
In such a case, the '` ...=default`' does not generate the default constructor and copy-assignment operator as they are *deleted* according to the rules above. 
So the full situation looks like this:
```pmans
struct Other {
  int /*b1*/ &i;
  Other() = /*b7*/ delete;                           // "default == delete" in this case, why?
  Other(const Other &w) = /*f8*/ default;
  Other(Other &&w) = /*f8*/ default;
  Other &operator=(const Other &w) = /*b7*/ delete;  // "default == delete" in this case, why?
  Other &operator=(Other &&w) = /*b7*/ delete;       // "default == delete" in this case, why?
};
```
So, requiring a SMF explicitly using `default` does not guarantee its availability, as it might be marked as *deleted*.
On the other hand, it *is* possible to mark a SMF as *deleted* and therefore overrule (turn off) the implicit availability.

## Destruction
> Describe the effect of a destructor. 
> - allows to implement custom actions required to "properly" tear-down an object; e.g., releasing allocated resources

The destruction mechanism for a user-defined type is triggered at the end of the lifetime of an object.

> Example for when the lifetime of an object ends?
> - local variable goes out of scope, e.g., a function body ends
> - execution reaches the end of program: static variables reach end of lifetime
> - a class object is deconstructed: lifetime of non-static members ends

If no user-declared destructor is present, an implicitly declared constructor (with an empty body) is available. For `Widget` it looks like this:
```pmans
struct Widget {
  int i;
  std::vector<double> j;    
  ...
  /*b*/ ~Widget(){} /*x*/ 
};
```
The destruction mechanism then first executes the body of the destructor followed by a destruction of the members in reverse order of declaration. Conceptually this looks like this for `Widget`:
```pmans
// conceptual code, does not compile
void /*f*/ deconstruction_mechanism /*x*/(T& obj) {
    ~obj();
    // deconstruct members in reverse order of declaration
    for(auto& member: [reverse list of members of obj]) {
      /*f*/ deconstruction_mechanism /*x*/(member);
    }
}
{
  Widget w{};
} /*f*/ deconstruction_mechanism /*x*/(w); // end of lifetime for 'w' 

```
## What can go wrong?
Let's consider a resource owning `Widget` with a user-defined constructor (which allocates memory) and a user-defined destructor (which frees the allocated memory):
```pmans
struct WidgetOwns {
  double *data;
  size_t size;
  WidgetOwns(size_t size) : size(size), data(/*b3*/ new double[size]) {} // (1)
  ~WidgetOwns() { /*b6*/ delete[] data;} // (6)
}
```
> Which SMF are additionally implicitly available for the `Widget` above? 
> - the user-defined destructor (6) disables the implicit availability of "move related" SMFs (as these were introduced in C++11)
> - the user-defined constructor (1) disables the availability of the default constructor
> - "copy related" SMFs are implicitly available (deprecated since C++11)
> - the member `data` of type pointer-to-`double` is not considered as a reference

> Describe problems resulting from the implicit availablity of "copy related SMFs":

> - a copy constructor is implicitly available and will copy the value of `data`: the newly constructed object points to the same memory location
>```pmans
>Widget w1(10);
>Widget w2 = w1; //  will copy pointer value
>// what happens at destruction? 
>```
> - a copy assignment is implicitly available and leads will assign the value of `data`: after assignment both objects point to the same memory resource; additionally the original value of `data` is overwritten 
>```pmans
>Widget w1(10);
>Widget w2(10);
>w2 = w1; //  will copy pointer value; value of 'w2.data' is lost 
>// what happens at destruction?
>// what else?
>```

## Links

- Special member functions: https://en.cppreference.com/w/cpp/language/member_functions
- Default constructor: https://en.cppreference.com/w/cpp/language/default_constructor
- Copy constructor: https://en.cppreference.com/w/cpp/language/copy_constructor
- Move constructor https://en.cppreference.com/w/cpp/language/move_constructor
- Copy assignment: https://en.cppreference.com/w/cpp/language/copy_assignment
- Move assignment: https://en.cppreference.com/w/cpp/language/move_assignment
- Destructor: https://en.cppreference.com/w/cpp/language/destructor
- Defaulted special members: https://abseil.io/tips/131