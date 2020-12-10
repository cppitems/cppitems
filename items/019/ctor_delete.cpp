struct Base {
  int b;
  Base() = delete;
};
struct Widget : Base {
  int w;
  Widget() : Base(), w() {} // implicit construction mechanism
};

int main(){
    Widget w{};
}