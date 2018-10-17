#include<bits/stdc++.h>
#include<windows.h>
using namespace std;
#define MAX 500
#define MST(A) memset( (A) , (0) ,sizeof (A) )
const string eps="~";
map<string,set<char> > first;
map<string,set<char> > follow;
string S="";
set<char> VT;

class Atom_Re
{
public:
    string left;
    set<string> right;
    Atom_Re(string ipt)
    {
        left=ipt;
    }
    void isrt(string rt)
    {
        right.insert(rt);
    }
    void debug_print()
    {
        cout<<"Left Part : "<<left<<" Right Part: ";
        if(!right.empty())
        {
            if(right.size()==1)
            {
                cout<<*right.begin();
            }
            else
            {
                auto it=right.begin();
                cout<<*it;
                for(; it!=right.end(); ++it)
                {
                    cout<<"|"<<*it;
                }
            }
            cout<<endl;
        }
        else
            cout<<"OOPS!It is empty here.\n";
    }
};


vector<Atom_Re> VN_set;
map<string,int> VN_dic;
bool vis[MAX];
void input(int& n)
{
    cin>>n;
    string tmp;
    VT.insert('#');
    for(int times=0; times<n; times++)
    {
        cin>>tmp;
        string LEFT="";
        string RIGHT="";
        auto it=tmp.begin();
        for(; it!=tmp.end(); ++it)
        {

            if(*it=='-')
                break;
            LEFT+=*it;
            if(*it!='\''&&!isupper(*it)&&*it!=eps[0])
            {
                VT.insert(*it);
            }
        }
        ++it;
        ++it;
        if(times==0)
        {
            S=LEFT;
            cout<<"The START SYMBOL is "<<S<<endl<<endl;
        }
        if(!VN_dic[LEFT])
        {
            VN_set.push_back (LEFT);
            //cout<<LEFT<<endl;
            VN_dic[LEFT] = VN_set.size();
        }

        for(; it!=tmp.end(); ++it)
        {
            if(*it!='\''&&!isupper(*it)&&*it!=eps[0])
            {
                VT.insert(*it);
            }
            RIGHT+=*it;
        }
        VN_set[VN_dic[LEFT]-1].isrt(RIGHT);
        //cout<<RIGHT<<endl;

    }

}

void DFS(int x)
{
    if(vis[x])
        return;
    vis[x]=1;
    string& left = VN_set[x].left;
    set<string>& right = VN_set[x].right;
    auto it = right.begin();
    for ( ; it!= right.end() ; it++ )
    {
        for ( int i=0 ; i<it->length() ; i++ )
        {
            if(!isupper(it->at(i))&&it->at(i)!='\'')
                first[left].insert(it->at(i));
            if(isupper(it->at(i)))
            {
                int idx=0;
                if(it->at(i+1)=='\''&&i!=it->length()-1)
                {
                    idx=VN_dic[it->substr(i,2)]-1;
                }
                else
                {
                    idx=VN_dic[it->substr(i,1)]-1;
                }
                string nxtLeft=VN_set[idx].left;
                DFS(idx);
                set<char>& tmp=first[nxtLeft];
                auto sc=tmp.begin();
                bool flag=0;
                for(; sc!=tmp.end(); ++sc)
                {
                    if(*sc==eps[0])
                    {
                        flag=1;
                    }
                    first[left].insert(*sc);
                }
                if(flag==0)
                    break;
            }
            else
                continue;

        }
    }

}

void make_first()
{
    MST(vis);
    for(int i=0; i<VN_set.size(); i++)
        DFS(i);


///Output FIRST set:

    cout<<"\n\nFIRST:\n\n";
    map<string,set<char> >::iterator it = first.begin();
    for ( ; it != first.end() ; it++ )
    {
        cout<<internal<<left<<setw(15)<<"FIRST("+it->first+")" ;
        set<char>& temp=it->second;
        set<char>::iterator it1=temp.begin();
        bool flag=false;
        cout<<"{ ";
        for ( ; it1!=temp.end() ; it1++ )
        {
            if (flag)
                cout<<" , ";
            cout<<*it1 ;
            flag = true;
        }
        cout<<" }\n";
    }
}
void append ( const string& str1, const string& str2 )
{
    set<char>& from = follow[str1];
    set<char>& to = follow[str2];
    set<char>::iterator it = from.begin();
    for ( ; it!=from.end(); it++)
        to.insert (*it);
}
void make_follow()
{
    follow[S].insert('#');
    while(1)
    {


        bool loop_flag = false;
        for ( int i=0 ; i<VN_set.size(); i++ )
        {
            string& left=VN_set[i].left;
            set<string>&right =VN_set[i].right;
            auto it = right.begin();
            for ( ; it!= right.end() ; it++ )
            {
                bool flag = true;
                const string& str = *it;
                for ( int j = it->length()-1 ; j >= 0 ; j-- )
                {
                    if ( str[j] == '\''||isupper(str[j] ) )
                    {
                        int mns=0;
                        if(str[j]!='\'')
                            mns=-1;
                        string VN=it->substr(j-1-mns,2+mns);
                        int x = VN_dic[VN]-1;
                        if ( flag )
                        {
                            int tt = follow[VN].size();
                            append ( left, VN );
                            int tt1 = follow[VN].size();
                            if ( tt1 > tt )
                                loop_flag = true;
                            if ( !VN_set[x].right.count(eps) )
                                flag = false;
                        }
                        for ( int k = j+1 ; k < it->length() ; k++ )
                        {
                            if ( isupper(str[k]) )
                            {
                                string id;
                                if ( k != it->length()-1 && str[k+1] == '\'' )
                                    id = it->substr(k,2);
                                else
                                    id = it->substr(k,1);
                                set<char>& from = first[id];
                                set<char>& to = follow[VN];
                                int tt = to.size();
                                auto it1 = from.begin();
                                for ( ; it1 != from.end() ; it1++ )
                                    if ( *it1 != eps[0] )
                                        to.insert ( *it1 );
                                int tt1 = follow[VN].size();
                                if ( tt1 > tt )
                                    loop_flag = true;
                                if ( !VN_set[VN_dic[id]-1].right.count("~") )
                                    break;
                            }
                            else if ( str[k] != '\'' )
                            {
                                int tt = follow[VN].size();
                                follow[VN].insert ( str[k] );
                                int tt1 = follow[VN].size();
                                if ( tt1 > tt )
                                    loop_flag = true;
                                break;
                            }
                            else
                                continue;
                        }
                        j--;
                    }
                    else
                        flag=0;
                }
            }
        }
        if(!loop_flag)
            break;
    }

    puts ("\n\nFOLLOW:" );
    auto it = follow.begin();
    for ( ; it!=follow.end(); ++it)
    {
        cout<<internal<<left<<setw(15)<<"FOLLOW("+it->first+")";
        cout<<"{ ";
        set<char> & temp = it->second;
        //temp.insert('#');
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for ( ; it1!=temp.end() ; it1++ )
        {
            if ( flag )
                cout<<" , ";
            cout<<*it1;
            flag = true;
        }
        cout<<" }\n";
    }
    cout<<endl<<endl;
}
vector<map<char,string> > predict_table;
bool check_first ( const string& text , char ch )
{
    for ( int i = 0 ; i < text.length() ; i++ )
    {
        bool hasEmpty = false;
        if ( !isupper(text[i]) && text[i] != '\'' )
        {
            if ( text[i] != ch ) return false;
            else return true;
        }
        else if ( isupper(text[i] ) )
        {
            string temp;
            if ( i != text.length()-1 && text[i+1] == '\'' )
                temp = text.substr(i,2);
            else
                temp = text.substr(i,1);
            set<char>& dic = first[temp];
            set<char>::iterator it = dic.begin();
            for ( ; it != dic.end() ; it++ )
            {
                if ( *it == eps[0] ) hasEmpty = true;
                if ( *it == ch ) return true;
            }
            if ( !hasEmpty) break;
        }
        else continue;
    }
    return false;
}

bool check_follow ( const string& text , char ch )
{
    set<char>& dic = follow[text];
    set<char>::iterator it = dic.begin();
    for ( ; it != dic.end() ; it++ )
        if ( *it == ch ) return true;
    return false;
}
/*string find_re(string chc_str,char x,bool& conflict_flag)
{
    auto it=VN_set[VN_dic[chc_str]-1].right.begin();
    conflict_flag=0;
    innerflag=0;
    string res="";
    for(; it!=VN_set[VN_dic[chc_str]-1].right.end(); ++it)
    {
        string ans=*it;
        for(int it_str=0; it_str<ans.size(); ++it_str)
        {
            if(*it_str==x&&innerflag==0)
            {
                innerflag=1;
                res=ans;
            }
            else if(isupper(*it_str))
            {
                if(it_str)
            }
            else if(*it_str==x&&innerflag==1)
            {
                conflict_flag=1;
                return res;
            }
        }
    }
   return res;
}*/
/*void TABLE()
{
    auto it=VN_set.begin();
    map<char,string> table_frame;
    bool conflict=0;

    VT.clear();
    for(; it!=VN_set.end()&&conflict==0; ++it)
    {
        string head=it->left;
        table_frame.clear();
        for(auto vt=VT.begin(); vt!=VT.end()&&conflict==0; ++vt)
        {
            if(first[head].find(*vt))
            {
                table_frame[*vt]=find_re(head,*vt,conflict);
                if(conflict)
                {
                    cerr<<"Conflict Detected!\nThe program will be terminated shortly after.";
                    return;
                }
            }
        }
        if(follow[head].find(eps[0]))
        {

        }
    }


    //for()
}*/
void TABLE()
{
    map<char,string> temp;
    vector<char> letter;
    bool vis[500];
    memset ( vis , 0 , sizeof ( vis ) );
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        set<string>& right = VN_set[i].right;
        set<string>::iterator it = right.begin();
        for ( ; it != right.end() ; it++ )
            for ( int j = 0 ; j < it->length() ; j++ )
                if ( !isupper(it->at(j)) && it->at(j) != '\'' )
                {
                    if ( vis[it->at(j)] ) continue;
                    vis[it->at(j)] = true;
                    letter.push_back ( it->at(j) );
                }
    }
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        temp.clear();
        string& left = VN_set[i].left;
        set<string>& right = VN_set[i].right;
        set<string>::iterator it = right.begin();
        for ( ; it != right.end() ; it++ )
            for ( int j = 0 ; j < letter.size() ; j++ )
            {
                //cout << *it << " " <<  letter[j] << endl;
                if ( check_first ( *it , letter[j] ) )
                {
                    //cout << "YES" << endl;
                    temp[letter[j]] = *it;
                }
                if ( it->at(0) == '~' && check_follow ( left, letter[j] ))
                    temp[letter[j]] = *it;
            }
        predict_table.push_back ( temp );
    }

    cout<<"Table for Prediction:\n";
    for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
        cout<<"-";
    cout<<endl;

    cout<<"|";
    cout<<setw(9)<<right<<"|";
    for ( int i = 0 ; i < letter.size() ; i++ )
        cout<<setw(5)<<right<<letter[i]<<setw(5)<<"|";
    cout<<endl;
    for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
        cout<<"-";
    cout<<endl;
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
       cout<<"|"<<right<<setw(6)<<VN_set[i].left<<internal<<setw(3)<<"|";
        for ( int j = 0 ; j < letter.size() ; j ++ )
            if ( predict_table[i].count(letter[j] ) )
                cout<<internal<<setw(7)<<predict_table[i][letter[j]]<<internal<<setw(3)<<"|";
            else cout<<right<<setw(10)<<"|";
        cout<<endl;
        for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
            cout<<"-";
       cout<<endl;
    }
     cout<<endl<<endl;
}

void print ( int steps , stack<string> stk , string src , string wf , int x )
{
    cout<<fixed<<left<<setw(10)<<steps;
    string out = "";
    while ( !stk.empty() )
    {
        out = stk.top()+out;
        stk.pop();
    }
    cout<<fixed<<left<<setw(9)<<out;

    out ="";
    for (int i = x ; i<src.size();i++)
        out+=src[i];
    cout<<fixed<<left<<setw(10)<<out+"#";

    cout<<fixed<<left<<setw(10)<<wf<<endl;

}

void analyse ( const string& src )
{
    cout<<"Analogue Process of the Analysis:\n";
    stack<string> stk;
    stk.push(S);
    int steps=0;
    int idx=0;
    cout<<fixed<<left<<setw(10)<<"Steps";
    cout<<fixed<<left<<setw(9)<<"Stack";
    cout<<fixed<<left<<setw(10)<<"In_str";
    cout<<fixed<<left<<setw(10)<<"Generator"<<endl;
    while ( !stk.empty() )
    {
        string u = stk.top();
        string tmp="";
        stk.pop();
        if (!isupper(u[0]))
        {
            if (idx==src.length()&&u[0]==eps[0]);
            else if (src[idx]==u[0])
                idx++;
        }
        else
        {
            int x=VN_dic[u]-1;
            tmp=predict_table[x][src[idx]];
            for( int i =tmp.length()-1 ; i>=0 ;i--)
            {
                if ( tmp[i] == '\'' )
                {
                    string v = tmp.substr(i-1,2);
                    stk.push ( v );
                    i--;
                }
                else
                {
                    string v = tmp.substr(i,1);
                    stk.push( v );
                }
            }
            tmp=u+"->"+tmp;
        }
        print(steps++,stk,src,tmp,idx);
    }
}
int main()
{
    freopen("test.txt","r",stdin);
    int n=0;
    input(n);
    cout<<eps<<" as epsilon";
    make_first();
    make_follow();
    /*for(auto y =VT.begin();y!=VT.end();++y)
    {
        cout<<*y<<" , ";
    }*/
    TABLE();
    string str_test="i*i+i";
    analyse(str_test);
    Sleep(1000000);
    return 0;
}


