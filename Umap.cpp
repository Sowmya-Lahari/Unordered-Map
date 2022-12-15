#include<iostream>
#include<vector>
#include<sstream>
using namespace std;

template <typename T1,typename T2>
class Node{
    public:
    T1 key;
    T2 val;
    Node<T1,T2> *next;
    Node<T1,T2> *prev;
    Node(T1 k,T2 v)
    {
        key=k;
        val=v;
        next=NULL;
        prev=NULL;
    }
};

template <typename T1,typename T2>
class list{
    public:
    Node<T1,T2> *head;
    int count;
    list()
    {
        head=NULL;
        count=0;
    }
    int insert_beg(T1 k,T2 v)
    {
        Node<T1,T2> *temp=head;
        while(temp!=NULL && temp->key!=k)
        {
            temp=temp->next;
        }
        if(temp==NULL)
        {
            Node<T1,T2> *n=new Node<T1,T2>(k,v);
            n->next=head;
            if(head!=NULL)
                head->prev=n;
            head=n;
            count++;
            return 1;
        }
        else
        {
            temp->val=v;
            return 0;
        }
    }
    int delete_node(T1 k)
    {
        //cout<<"delete_node  "<<k<<endl;
        Node<T1,T2> *temp=head;
        while(temp!=NULL && temp->key!=k)
        {
            temp=temp->next;
        }
        if(temp==NULL)
            return 0;
        count--;
        if(temp==head)
        {
            if(temp->next==NULL)
            {
                head=NULL;
                return 1;
            }
            head=head->next;
            head->prev=NULL;
            free(temp);
            return 1;
        }
        if(temp->next==NULL)
        {
            temp->prev=temp->next;
            free(temp);
            return 1;
        }
        temp->next->prev=temp->prev;
        temp->prev->next=temp->next;
        free(temp);
        return 1;
    }
};

template <typename T1,typename T2>
class unordered_map{
    int size=50021;
    list<T1,T2> **table;
    int *cnt;
    public:
    unordered_map()
    {
        table=new list<T1,T2>*[size];
        cnt=new int[size];
        for(int i=0;i<size;i++)
            cnt[i]=0;
    }
    int hash_func(T1 key)
    {
        stringstream s1(key);
        string s=s1.str();
        long sum=0;
        for(int i=0;i<s.size();i++)
        {
            int as=int(s[i]);
            if(as<10)
                sum=(sum*10+as)%size;
            else if(as<100)
                sum=(sum*100+as)%size;
            else
                sum=(sum*1000+as)%size;
        }
        return sum%size;
    }
    void insert(T1 k,T2 v)
    {
        int ind=hash_func(k);
        if(cnt[ind]==0)
        {
            cout<<"insert   "<<k<<"\t"<<ind<<endl;
            list<T1,T2> *newlst=new list<T1,T2>();
            newlst->head=NULL;
            newlst->count=0;
            cnt[ind]+=newlst->insert_beg(k,v);
            //cout<<newlst->head->key<<"  here"<<endl;
            table[ind]=newlst;
            //cout<<table[ind]->head->key<<"  here"<<endl;
        }
        else
        {
            cnt[ind]+=table[ind]->insert_beg(k,v);
        }
    }
    void erase(T1 k)
    {
        int ind=hash_func(k);
        //cout<<"erase    "<<k<<"\t"<<ind<<endl;
        if(cnt[ind]!=0)
            cnt[ind]-=table[ind]->delete_node(k);
        else
            return;
    }
    bool find(T1 k)
    {
        int ind=hash_func(k);
        //cout<<"findfun "<<k<<"\t"<<ind<<endl;
        if(cnt[ind]==0)
            return false;
        Node<T1,T2> *temp=table[ind]->head;
        //cout<<"h2"<<temp->key<<endl;
        while(temp!=NULL && temp->key!=k)
        {
            //cout<<temp->key<<"\t";
            temp=temp->next;
        }
        //cout<<endl;
        if(temp==NULL)
            return false;
        return true;
    }
    T2 operator [] (T1 k)
    {
        static T2 def;
        int ind=hash_func(k);
        //cout<<k<<"\t"<<ind<<endl;
        if(cnt[ind]==0)
            return def;
        Node<T1,T2> *temp=table[ind]->head;
        while(temp!=NULL && temp->key!=k)
        {
            temp=temp->next;
        }
        if(temp==NULL)
            return def;
        return temp->val;
    }
};
template<typename T1>
vector<int> solve(vector<T1> &arr,int size,int k)
{
    unordered_map<T1,int> map1;
    vector<int> res;
    //cout<<"here"<<endl;
    if(k>size)
        return res;
    int cur=0;
    for(int i=0;i<size;i++)
    {
        if(map1.find(arr[i]))
        {
            //cout<<"find"<<arr[i]<<endl;
            int k=map1[arr[i]];
            map1.insert(arr[i],k+1);
        }
        else
        {
            map1.insert(arr[i],1);
            cur++;
        }
        //cout<<arr[i]<<"\t";
        //cout<<map1[arr[i]]<<endl;
        if(i==k-1)
            res.push_back(cur);
        else if(i>k-1)
        {
            int rem=map1[arr[i-k]];
            if(rem>1)
            {
                map1.insert(arr[i-k],rem-1);
            }
            else
            {
                cur--;
                map1.erase(arr[i-k]);
            }
            res.push_back(cur);
        }
    }
    return res; 
}
int main()
{
    typedef string type1;
    typedef string type2;
    unordered_map<type1,type2> m1;
    int ch=1;
    type1 k;
    type2 v;
    while (ch<5)
    {
        cout<<"Enter"<<endl;
        cin>>ch;
        if(ch==0)
        {
            cin>>k>>v;
            m1.insert(k,v);
        }
        else if(ch==1)
        {
            cin>>k;
            m1.erase(k);
        }
        else if(ch==2)
        {
            cin>>k;
            if(m1.find(k))
                cout<<"present"<<endl;
            else
                cout<<"not present"<<endl;
        }
        else if(ch==3)
        {
            cin>>k;
            cout<<m1[k];
        }
        else if(ch==4)
        {
            int sz=0;
            cin>>sz;
            type1 x;
            vector<type1> arr;
            for(int j=0;j<sz;j++)
            {
                cin>>x;
                arr.push_back(x);
            }
            int ks;
            cin>>ks;
            vector<int> res;
            res=solve(arr,sz,ks);
            for(int j=0;j<res.size();j++)
                cout<<res[j]<<"\t";
            cout<<endl;
        }
    } 
    return 0;
}
