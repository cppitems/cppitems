10 // status of this item: 1=ready ... 10=not ready
# Title of Item 1
## SubTitle of Item 1
First line of text 

### regular highligting
```cpp
// regular c++ highlighting
int main (){
    int a = 5;
}
```

### special highligting
```pmans
// pmans c++ highlighting
/* file: main.cpp */ /* compile: clang++ -std=c++17 main.cpp */
int main /*b2*/ (){
    int a = /*b1*/ 5;
    /*b*/ double /*x*/ b = static_cast<double>(int);
    /*f*/ double /*x*/ b = static_cast<double>(/*f3*/ int);
    // comment
    "this will be not shown" /// ... // line removed and replaced with ...
    "this line is fully removed" /// including the comment /// 
}
```

### code highlighting `inline`
Inline `code` highlighting 