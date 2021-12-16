struct Base {
  private: int prv;   // access only from inside 'Base'
  protected: int pro; // + access from within more-derived classes
  public:  int pub;   // + access from outside
};

struct WidgetPrv : private Base {
    //int base_prv = Base::prv; // prv is inaccessible
    int base_pro = Base::pro; // Base::pro access is private
    int base_pub = Base::pub; // Base::pub access is private
// public: // (implicit due to 'struct') 
//     using Base::pub; // introduce WidgetPrv::pub (public)
//     using Base::pro; // introduce WidgetPrv::pro (public)   
// };
// struct WidgetPro : protected Base {   
//     // int base_prv = Base::prv; // prv is inaccessible
//     int base_pro = Base::pro; // Base::pro access is protected
//     int base_pub = Base::pub; // Base::pub is protected    
// };
// struct WidgetPub : public Base {  
//     // int base_prv = Base::prv; // prv is inaccessible
//     int base_pro = Base::pro; // Base::pro access is protected
//     int base_pub = Base::pub; // Base::pub is public    
// };


int main(){
    WidgetPrv w{};
    w.pub = 7;
    w.pro = 8;
}