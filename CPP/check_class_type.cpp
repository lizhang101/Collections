#include <stdio.h>
#include <typeinfo>
#include <memory>

using namespace std;

float f1(shared_ptr<float> i){
    return 1.0;
}

int f1(shared_ptr<int> i){
    return 1;
}

template <typename T>
T f(shared_ptr<T> i)
{
    T r = f1(i);
    return r;
}



int main()
{
    float i = shared_ptr<float>(1.0);
    float o = f<float>(i);
    if (typeid(o) == typeid(float)){
        printf("float\n");
    } 
}
