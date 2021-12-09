struct Base {
  int b;
  Base() = default;
//   Base() = delete;
};
struct Widget : Base {
  int w;
  Widget() : Base(), w() {} // implicit construction mechanism
};

int main(){
    Widget w{};
}