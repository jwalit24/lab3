#include <bits/stdc++.h>

using namespace std;

int d,t;
int leaf_count=0,index_count=0;
struct block{
    bool leaf;                // whether this is a leaf
    block * parent;           // parent pointer
    vector<int> data;         // stores the data in the block
    vector<block*> children;  // stores the children pointers, and is always one size greater
    explicit block(bool if_leaf){
        parent= nullptr;
        if(!if_leaf){
            leaf= false;
        }else{
            leaf= true;
        }
    }
};
int main() {
    cin>>d>>t;
    block * root= nullptr;
    while (true){
        int op;cin>>op;
        if(op==1){
            if(root== nullptr){      // if inserting for the first time
                root=new block (true);
                leaf_count++;
            }
            int val;cin>>val;
            block * now=root;      // now is the pointer which denotes the node i am currently examining
            while (now->leaf==0){  // in this while loop we go to the appropriate children till we reach leaf
                if(now->data[0]>val){// in case we need to go to the leftmost child
                    now=now->children[0];
                }else if(val>=now->data.back()){// in case we need to go to the rightmost child
                    now=now->children.back();
                }else{// in case any middle node
                    for(int i=0;i<now->data.size()-1;i++){
                        if(now->data[i]<=val && now->data[i+1]>val){
                            now=now->children[i+1];
                            break;
                        }
                    }
                }
            }
            // in this portion the node we are examining is leaf and we are inserting val at appropriate place
            deque<int> buff;
            while (!now->data.empty() && now->data.back()>val){
                buff.push_back(now->data.back());
                now->data.pop_back();
            }
            now->data.push_back(val);
            while (buff.size()>0){
                now->data.push_back(buff.back());
                buff.pop_back();
            }
            if(now->data.size()<=2*d){   // if after insertion there is no overflow of leaf
                continue;
            }else{
                block * sibling =new block(1); // we define a sibling of leaf
                leaf_count++;
                // we remove last d+1 elements from leaf to sibling and reverse to keep sibling in sorted order
                while (now->data.size()>d){
                    sibling->data.push_back(now->data.back());
                    now->data.pop_back();
                }
                reverse(sibling->data.begin(),sibling->data.end());
                sibling->parent=now->parent;
                int transfer=sibling->data[0];
                bool flg= false;
                while (now->parent != nullptr){ // we are pushing overflow towards root, if it is fixed in mid then well and good else we create a new root
                    deque<block*> buff; // buffer for children pointer
                    deque<int> buff2;   // buffer for data
                    block * curr=now->parent;
                    // in this part we are inserting sibling inside the children list of parent of now, we do it like this first put everything after now in buffer the fill sibling just after now then again fill buffer
                    while (curr->children.back()!=now){
                        buff.push_back(curr->children.back());
                        buff2.push_back(curr->data.back());
                        curr->children.pop_back();
                        curr->data.pop_back();
                    }
                    curr->children.push_back(sibling);
                    curr->data.push_back(transfer);
                    while (buff.size()>0){
                        curr->children.push_back(buff.back());
                        curr->data.push_back(buff2.back());
                        buff.pop_back();
                        buff2.pop_back();
                    }
                    if(curr->children.size()>2*t+2){ // if my parent size after sibling is greater than 2t+1 then split my parent and give them till "now" and "sibling"
                        index_count++;
                        sibling = new block(0);
                        sibling->parent=curr->parent;
                        
                        for(int i=0;i<t+1;i++){ // puting last t+1 data
                            sibling->data.push_back(curr->data.back());
                            curr->data.pop_back();
                        }
                        transfer=curr->data.back();// transfer t+1 th data one more level up
                        curr->data.pop_back();
                        for(int i=0;i<t+2;i++){ // last t+2 children pointer in sibling
                            sibling->children.push_back(curr->children.back());
                            curr->children.back()->parent=sibling;
                            curr->children.pop_back();
                        }
                        // reverse to maintain sorted order
                        reverse(sibling->children.begin(),sibling->children.end());
                        reverse(sibling->data.begin(),sibling->data.end());
                        now=curr;
                    }else{
                        flg= true;
                        break;
                    }
                }
                if(!flg){ // if we got till root and still splitted , then we create a new root
                    block * newrt =new block(0);
                    index_count++;
                    newrt->children.push_back(now);
                    newrt->children.push_back(sibling);
                    newrt->data.push_back(transfer);
                    now->parent=newrt;
                    sibling->parent=newrt;
                    root=newrt;
                }
            }
        }else if(op==2){
            cout<<index_count<<" "<<leaf_count<<endl;
            for(int i=0;i<root->data.size();i++){
                cout<<root->data[i]<<" ";
            }
            cout<<endl;
        }else{
            break;
        }
    }
    return 0;
}