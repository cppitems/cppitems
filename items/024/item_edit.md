1 // item status
# Exceptions
Let's first consider C (which does not have exceptions). 

> What are the options in C to do "exceptions"?
> - return value carries error code
>   - disadv: ...
> - question: error -> should/can the program recover?
> - jumps -> to seperate code that raises error and code that handles error

But of course the desire to capture and handle unforeseen events is present. A pattern to achieve this is to always use the return value of a function as error indication. Let's introduce this pattern for a `grid_init` function to capture problems during allocation which we adopt from
```pmans
/*b*/ void /*x*/ grid_init(struct Grid *grid, size_t nx, size_t ny) {
  grid->data = (double *)calloc(nx * ny, sizeof(double));
  ...  
}        
// usage example
grid_init(&grid, 3, 4);    
```
to a version returning an error code instead of void:
```pmans
/*b*/ int /*x*/ grid_init(struct Grid *grid, size_t nx, size_t ny) {
  void * ptr = (double *)calloc(nx * ny, sizeof(double));
  if(ptr == NULL)
     /*b*/ return 101; /*x*/       // error code for bad alloc
  grid->data = ptr;
  ...  
  return 0;            // success
}
```
Now also the call site has to be adopted to capture (and propagate) the error code:
```pmans
int main() {
  ...
  int error = grid_init(&grid, 3, 4);
  if (!error) { // check if success
    ...
    grid_free(&grid);
    /*b*/ return 0; /*x*/    // success
  }
  /*b*/ return error; /*x*/   // forward error code
}
```
Essentially, this pattern of error code handling forces many functions to return an error code: obviously those function which do things that are error-prone (like allocation, or user input) but also all other functions which have to forward error codes of other functions. This is necessary to pass the error information up the call stack. This approach is not ideal, as the source code in many functions is concerned about error code logic, even if the error is just forwarded:
```pmans
int func() {
  double d = func1();     // might error (or just forwarding)
  if(get_last_error()){
    ...
  }
  func2();     // might error (or just forwarding)
  if(get_last_error()){
    ...
  }  
  if (error1 | errro2)
    return error1 & error2; // forward both errors
  return 0;                 // no errors
} 
```
and also the return value is always utilized to forward the error codes, and cannot be used for other purposes.

> Are there alternative approaches in C beside using the return value to propagate errors?
>- yes: using a global function `get_last_error()` which allows the caller to check for the most recently reported error of an API.


C++ provides an extra mechanism to handle such scenarios: **exceptions**. To see how they work, let's adopt the above grid example to an exception:
```pmans
/*b*/ void /*x*/ grid_init(Grid *grid, size_t nx, size_t ny) {
  void *ptr = /*b*/ std:: /*x*/calloc(nx * ny, sizeof(double));
  if (ptr == NULL)
     /*b*/ throw "bad alloc"; /*x*/ // what type do we throw here?
  grid->data = (double *)ptr;
  ...
}
```

> Which objects can be "thrown" and where is this allowed? 
> - any type (copyable), anywhere in the code



> Where is the exception reported/catched?
> - in the next enclosing and fitting catch clause (this could be several nested function calls earlier, or even main())

The normal execution is interrupted at the point of `throw ...` and the exception object is created/constructed. 
In our case the exception object is a string literal "bad alloc" which provides minimal information about what happened. Of course also more complex exceptions objects can be created, e.g., the standard library provides `std::bad_alloc` which inherits from `std::exception`. 

The execution of the program is then continued at the containing (and matching) catch block of a `try{ } catch(...){ }` statement that is found.
In our example we adopt the main function body to catch exceptions that match our exception object:
```pmans
int main() /*b*/ try { /*x*/
  grid_init(&grid, 3, 4);            // can throw, error "one" and error "two"
  throw "error";    // throw string literal
  grid_free(&grid);  
  return 0;

/*b*/ } catch (const std::exception& e) { /*x*/           // matching catch block  
... branch for types? -> dynamic_cast? -> not noexcept
/*b*/ } catch (char const *e) { /*x*/           // matching catch block
...
/*b*/ } catch (const std::bad_alloc& bad_alloc) { /*x*/           // matching catch block
  ...                                // output debug information 
  return 0;                          // terminate without error
/*b*/ } /*x*/ 
/*b*/  catch (...) { /*x*/           // matching catch block    
// dealing with unexpected exceptions
}
```



If no matching catch block is found in the call stack, the program is terminated (`std::terminate`).

> Are exceptions "lightweight"? 
> - yes and no
> - no: undwinding is heavy-lifting (cleanup of muliple nested stages of local variables -> dtors)
> - no: binary size for exception handling tables
> - yes: runtime overhead low if no exceptions are triggered
> - noexcept effect: stdlib detects at compile time if SMFs are noexcept or not -> move or copy

> Does google use exceptions in large protions of their c++ codebase
> - disabling exceptions altogether is possible


Exceptions provide a powerful mechanism to unwind the call stack until a matching catch block is found. During unwinding a lot of local variables/objects go out of scope and are automatically destructed (the destructor is called) according to the rules of the C++ standard.

> Can more than one exception be active at the same time? If no, what are immediate consequences?
> - no
> - destructors should not throw (in general: dellocation routines should never be interrupted by an exception)

> Do you find an example for a "deallocation routine" in EX3 which is not a destructor?
> - unique_ptr::reset(...)
> - shared_ptr assignment operators



The intended use of exceptions is to react on (rare) cases where there is an issue with an external resource (e.g., access to a file or memory allocation). 
Valuable debug information can be collected before stack unwinding (part) of an application.

> How do asserts compare to exceptions?
> - asserts: check for situations which are not expected to be able to happen (during debug/tests)
> - exceptions: use to enable separation between code that throws and the code that handles the problem
> - exceptions: should capture rare events -> unwind overhead is not critical


## Exception Safety
Common exception safety levels (not part of the C++ standard) can be used to characterize functions by additional information about the side-effects of a function if an exceptions occurs:
- **no guarantee**: an exception within the function might lead to an invalid state.
```pmans
void no_guarantee(){
  int *m = new int();
  ++(*m);
  other_func(); // might throw
  // /*b*/ throw "leak"; /*x*/
  delete m;
}
```
- **basic guarantee**: an exception will not lead to an invalid state, but the state might have changed. (this is what you always want at least, e.g. proper deconstruction)
```pmans
void basic_guarantee(){
  /*f*/ ++global; /*x*/
  std::unique_ptr<int> m = std::make_unique<int>();
  other_func(); // might throw
  ++(*m);
  ///*b*/ throw "noleak"; /*x*/
}
```
- **strong guarantee**: the state of the program is valid and identical to the state before the call.
```pmans
void strong_guarantee(){
  try {
    std::unique_ptr<int> m = std::make_unique<int>();
    ++(*m);
    /*b*/ throw "strong"; /*x*/
  } catch (...) {
    ... // rollback
    /*b*/ throw; /*x*/ //rethrow
  }
  /*f*/ ++global; /*x*/
}
```
- **no fail**: no exception will propagate to the outside. 
(or: if an exception is propagates it is ok to terminate)
```pmans
void no_fail() /*b*/ noexcept /*x*/ {
  try {
    throw "error";
  } catch (...) {
    // handle error
  }    // is it ok to throw and catch inside?
}
```
> What happens if an exception is thrown in a `noexcept` function?
> - termination


> Why is `noexcept` useful?
> - optimization (detecting at compile time): moving elements into stdlib container (if move ctor of `T` is not tagged `noexcept`, copies are made)
>    - why so: a potentially throwing-moving would not allow a rollback (as this could throw again and therefore terminate)
> - compiler 


> Are any functions implicitly defined `noexcept`?
> - yes, default SMFs for construction and assignment are `noexcept` if the involved (base)class members' SMFs are `noexcept`


> Can a constructor of a type which requires dynamic allocation of members be `noexcept`?
> - not really, but there is a trick, let's look at `std::vector`


### Example

Let's have a look at a copy assignment operator implementation for a `Widget`:
```pmans
struct Widget{
    int *m;
    double always_7 = 7;
    Widget(int init) : m(new int) {
        *m = init;
    };
    ~Widget(){delete m;}
    ...
    // (1)
    Widget& operator=(const Widget& other){ // can we mark that noexcept?
        delete this->m; // no throw
        always_7 = 8;
        this->m = new int; // might throw, if throws, execution is interrupted here
        // A) leak? no
        // B) state of "this", no recover/rollback option
        // C) invariant violated: no guarantee, invalid state 
        *(this->m) = other->m;
        return *this;
    }
    // (2)
    Widget& operator=(const Widget& other){ // not noexcept, but recoverable, -> no effect full rollback, no leaks
        Widget(other).swap(*this); // swap is expected always to be noexcept
        return *this;
    }    
}
```
> What about exception safety of the involved functions in the above snippet?
> - see inline comments

## links
- C++ exceptions: https://en.cppreference.com/w/cpp/language/exceptions
- Exceptions Best Practices: https://docs.microsoft.com/en-us/cpp/cpp/errors-and-exception-handling-modern-cpp