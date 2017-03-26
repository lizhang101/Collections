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
class SerializableInterface;

class GlobalResourceManger
{
    private:
        deque<weak_ptr<SerializableInterface>> resources;
    public:
        static GlobalResourceManger& get()
        {
            static GlobalResourceManger global_resource_manager_;
            return global_resource_manager_;
        }

        uint64_t getGlobalId() {
            return 0;
        }

        void addResource(shared_ptr<SerializableInterface>& rsc){
            weak_ptr<SerializableInterface> wp = rsc;
            resources.push_back(wp);
        };
        
    private:
        GlobalResourceManger() {};
        GlobalResourceManger(GlobalResourceManger const &);
        void operator=(GlobalResourceManger const &);
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
class RelationshipInterface
{
};

//-----------
class SerializableInterface
{
};
//


//-----------

class Workload: public SerializableInterface, RelationshipInterface
{
    string name_;
    public:
    virtual shared_ptr<Workload> genWorkload() = 0;
    //TODO: children. Parents don't have ownership of children.
    
    //parents
    //alive children should keep parents alive
    map<string, shared_ptr<Workload> > parent_wl_;

    shared_ptr<Workload> getWorkload(const string& cls_name){
        //printf("%s name:%s\n", __FUNCTION__, cls_name.c_str());
        if (parent_wl_.find(cls_name) == parent_wl_.end()){
            return nullptr;
        }
        return parent_wl_[cls_name];
    }

    void addRelated(const string& cls_name, shared_ptr<Workload> wl)
    {
        //printf("%s name:%s\n", __FUNCTION__, cls_name.c_str());
        parent_wl_ = wl->parent_wl_;
        parent_wl_[cls_name] = dynamic_pointer_cast<Workload> (wl);
    }

    virtual void whoami(){
        printf ("I'm %s\n", typeid(this).name());
    }
};
class Instruction : public Workload
{
    public:
    shared_ptr<Workload> genWorkload() override
    {
        //printf ("Instruction Gen\n");
        Instruction *p = new Instruction;
        shared_ptr<Workload> wl(p);
        return wl;
    }
    void whoami(){
        printf ("I'm %s\n", typeid(this).name());
    }
};

class Pixel : public Workload
{
    public:
    shared_ptr<Workload> genWorkload() override
    {
        //printf ("Pixel Gen\n");
        Instruction *p = new Instruction;
        shared_ptr<Workload> wl(p);
        return wl;
    }
    void whoami(){
        printf ("I'm %s\n", typeid(this).name());
    }
};

class Triangle : public Workload
{
    public:
    shared_ptr<Workload> genWorkload() override 
    {
        //printf ("Triangle Gen\n");
        Workload *pix = new Pixel;
        shared_ptr<Workload> wl(pix);
        return wl;
    }
    void whoami(){
        printf ("I'm %s\n", typeid(this).name());
    }
    
};
//----------------

class FuncLayer
{
    public:
    template <class T1, class T2> 
    shared_ptr<T1> query(shared_ptr<T2> &wl) {
        //printf("%s %s\n", typeid(T1).name(), typeid(T2).name());
        shared_ptr<T1> sub_wl = dynamic_pointer_cast<T1> (wl->getWorkload(typeid(T1).name()));
        if (sub_wl == nullptr) {
            sub_wl = dynamic_pointer_cast<T1> (wl->genWorkload());
            sub_wl->addRelated(typeid(T2).name(), wl);
            shared_ptr<SerializableInterface> rsc = dynamic_pointer_cast<SerializableInterface>(sub_wl);
            GlobalResourceManger::get().addResource(rsc);
        }
        return sub_wl;
    }
};

//------------------

int main()
{
    Triangle tri;
    printf ("call with object:\n");
    shared_ptr<Triangle> sp_tri = make_shared<Triangle> (tri);
    sp_tri->whoami();
/*
    auto wl0 = sp_tri->genWorkload();
    wl0->whoami();
    auto wl1 = wl0->genWorkload();
    wl1->whoami();
*/
    printf ("--------------\n");
    printf ("call with template:\n");
    FuncLayer fl;
    sp_tri->whoami();
    shared_ptr<Pixel> pix = fl.query<Pixel, Triangle> (sp_tri);
    pix->whoami();
    shared_ptr<Instruction> ins = fl.query<Instruction, Pixel> (pix);
    ins->whoami();

    printf("get triangle from Instruction\n");
    shared_ptr<Triangle> tri_0 = fl.query<Triangle, Instruction> (ins);
    tri_0->whoami();



/*
    Workload *wp = &tri;
    printf ("typeid by object:%s\n", typeid(tri).name());
    printf ("typeid by parent pointer:%s\n", typeid(*wp).name());

    wp = pix.get();
    printf ("typeid by object:%s\n", typeid(pix.get()).name());
    printf ("typeid by parent pointer:%s\n", typeid(*wp).name());

    GlobalResourceManger *p_gmng = & GlobalResourceManger::get();
    //p_gmng->addWorkloadType<Triangle>();

*/
}
