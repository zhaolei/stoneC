#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
    
    string xx; 
    string *pp;
    vector<string *> tt;
    for(int i=0; i<argc; i++) {
        xx = argv[i];
        pp = new string(argv[i]);
        cout<<"<"<<i<<"> :"<<xx<<endl;
        cout<<"<"<<i<<"> :"<<pp<<endl;
        tt.push_back(pp);
        //delete pp;
    }

    string::size_type ttl = tt.size();
    cout<<ttl<<endl;
    cout<<tt[0]<<endl;

    cout << "over" <<endl;
}
