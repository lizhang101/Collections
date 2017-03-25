#include <iostream>
#include <memory>
#include <map>
#include <deque>
#include <string>
#include <cassert>
using namespace std;

//TODO:
//1. add an new hierachy
//2. access another hierchy with multiple levels
//4. workload name generator: "WLLevel1_local_id_WLevel2_local_id"
//5. ID <- local id; global id(is unique).
//6. ID <-> Pointer when Save/Load checkpoints.

//-----------
//should be signleton
//sort of hacking in building the workload hierarchy level map

class Triangle;
class Pixel;

class GlobalResourceManger
{
    public:
        static GlobalResourceManger& get()
        {
            static GlobalResourceManger global_resource_manager_;
            return global_resource_manager_;
        }

        uint64_t getGlobalId() {
            return 0;
        }

        template <class T>
        uint32_t getWorkloadLevel() {
            return workload_hierarchy_[typeid(T).name()];
        }
    private:
        map<string, uint32_t> workload_hierarchy_;
    private:
        GlobalResourceManger() {
            printf ("GlobalResourceManger init:\n");
            addWorkloadType<Triangle>();
            addWorkloadType<Pixel>();
        };
        GlobalResourceManger(GlobalResourceManger const &);
        void operator=(GlobalResourceManger const &);
        template <class T>
        void addWorkloadType() {
            workload_hierarchy_[typeid(T).name()] = workload_hierarchy_.size();
        }
    public:
        //GlobalResourceManger(GlobalResourceManger const &) = delete;
        //void operator=(GlobalResourceManger const&) = delete;
};

//-----------
class DataBaseLayer
{
};
//-----------
class Serializer
{
    //When serializing, We tranverse all Workloads, build a address to global Id map, 
    //and associate content with this Id, finally,  save the IDs of each RelationShip Object to DB.
    //When deserializing, we load the content to memroy, and rebuild the ID to address map.
    //  Then according to the saved IDs for RelationShip Object, we can rebuild the relationship matrix.
     
    void serialize() {
        printf ("serialize\n");
    }
    void deserialize() {
        printf ("deserialize\n");
    }
};
//-----------
class Workload;
template <class T>
class RelationShip
{
    string name_;
    map<string, deque< weak_ptr<T> >> children_wl_;
    map<string, shared_ptr<T> > parent_wl_;

    shared_ptr<T> getChild(uint32_t lId) {
        return nullptr;
    }

    shared_ptr<T> getWorkload(){
        return parent_wl_[typeid(T).name()];
        
    }

};


//-----------

class Workload
{
    string name_;
    public:
    virtual shared_ptr<Workload> genWorkload() = 0;
    RelationShip<Workload> heirarchy_;
};

class Pixel : public Workload
{
    public:
    shared_ptr<Workload> genWorkload() override
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
    shared_ptr<Workload> genWorkload() override 
    {
        printf ("Triangle\n");
        Workload *pix = new Pixel;
        shared_ptr<Workload> wl(pix);
        return wl;
    }
    
};
//----------------

class FuncLayer
{
    public:
    template <class T> 
    shared_ptr<T> query(Workload & wl) {
        shared_ptr<T> t = static_pointer_cast<T> (wl.genWorkload());
        return t;
    }

    //Next Gen:
    /*
    template <class RtnT, class SrcT>
    shared_ptr<RtnT> query(SrcT &wl){} ; 
    */
};

//------------------

int main()
{
    Triangle tri;
    printf ("call with object:\n");
    auto p = tri.genWorkload();
    p->genWorkload();


    printf ("call with template:\n");
    FuncLayer fl;
    //auto pix = fl.query<shared_ptr<Workload>> (tri);
    auto pix = fl.query<Pixel> (tri);
    pix->genWorkload();
    pix->who();


    Workload *wp = &tri;
    printf ("typeid by object:%s\n", typeid(tri).name());
    printf ("typeid by parent pointer:%s\n", typeid(*wp).name());

    wp = pix.get();
    printf ("typeid by object:%s\n", typeid(pix.get()).name());
    printf ("typeid by parent pointer:%s\n", typeid(*wp).name());

    GlobalResourceManger *p_gmng = & GlobalResourceManger::get();
    //p_gmng->addWorkloadType<Triangle>();

    p_gmng = & GlobalResourceManger::get();
    printf("Triangle level:%d\n", p_gmng->getWorkloadLevel<Triangle>());
    printf("Pixel level:%d\n", p_gmng->getWorkloadLevel<Pixel>());


}
