#include <iostream>
#include <memory>
using namespace std;

class Workload
{
    public:
    virtual shared_ptr<Workload> getValue() = 0;
};

class Pixel : public Workload
{
    public:
    shared_ptr<Workload> getValue() override
    {
        printf ("Pixel\n");
        Pixel *pix = new Pixel;
        shared_ptr<Workload> wl(pix);
        return wl;
    }
    void who(){
        printf("i'm Pixel\n");
    }
};

class Triangle : public Workload
{
    public:
    shared_ptr<Workload> getValue() override 
    {
        printf ("Triangle\n");
        Workload *tri = new Pixel;
        shared_ptr<Workload> wl(tri);
        return wl;
    }
    
};

class FuncLayer
{
    public:
    template <class T> 
    shared_ptr<T> query(Workload & wl) {
        //T t =  wl.getValue();
        
        shared_ptr<T> t = static_pointer_cast<T> (wl.getValue());
        return t;
    }
};

int main()
{
    Triangle tri;
    printf ("call with object:\n");
    auto p = tri.getValue();
    p->getValue();


    printf ("call with template:\n");
    FuncLayer fl;
    //auto pix = fl.query<shared_ptr<Workload>> (tri);
    auto pix = fl.query<Pixel> (tri);
    pix->getValue();
    pix->who();
}
