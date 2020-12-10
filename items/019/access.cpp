struct Base {
  private: int prv;
  protected: int pro;
  public:  int pub;
};

struct WidgetPrv : private Base {
    // int prv = Base::prv; // prv is inaccessible
    int pros = Base::pro; // pro access is private
    int pubs = Base::pub; // pub access is private 
    using Base::pub; // pub access is public now 
    using Base::pro; // pro access is public now    
};
struct WidgetPro : protected Base {
    // int prv = Base::prv; // prv is inaccessible
    int pro = Base::pro; // pro access is protected
    int pub = Base::pub; // pub access is protected    
};
struct WidgetPub : public Base {
    // int prv = Base::prv; // prv is inaccessible
    int pro = Base::pro; // pro access is protected
    int pub = Base::pub; // pub access is public    
};


int main(){
    WidgetPrv w{};
    w.pub = 7;
    w.pro = 8;
}