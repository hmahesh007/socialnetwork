//network.cpp
#include "user.h"
#include "network.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <set>
#include <queue>
#include <map>
#include <stack>
#include <algorithm>

Network::Network(){}

//pre-condition: none
//post-condition: returns id by name and -1 if name doesn't exist
int Network::getId(const std::string& name){ 
    for (auto* user : users_){
        if (user->getName() == name){
            return user->getId();
        }
    }
    return -1;
}

//pre-condition: none
//post-condition: returns user object 
User* Network::getUser(int id){ 
    for (auto* user: users_){
        if (user->getId()==id){
            return user;
        }
    }

    return nullptr;
}

//pre-condition: none
//post-condition: add user to the n
void Network::addUser(User* user){
    users_.push_back(user);
}

//pre-condition: users exist and are not the same
//post-condition: adds friendship between the names provided
int Network::addConnection(const std::string s1, const std::string s2){
    int id1 = getId(s1);
    int id2 = getId(s2);

  
    if (id1==id2 || id2 == -1 || id1 == -1){
        return -1;
    }

    getUser(id1)->addFriend(id2); 
    getUser(id2)->addFriend(id1);

    return 0;
}

//pre-condition: names exist and are not the same
//post-condition: delete friendship connection
int Network::deleteConnection(const std::string s1, const std::string s2){
    int id1 = getId(s1);
    int id2 = getId(s2);

    if (id1==id2 || id2==-1 || id1==-1){
        return -1;
    }

    getUser(id1)->deleteFriend(id2);
    getUser(id2)->deleteFriend(id1);

    return 0;
}

//pre-condition: file is formatted correctly
//post-condition: reads users from file and adds to network
int Network::readUsers(const char* fname) {
    std::string myline;
    std::ifstream myfile(fname);
    int total;

    if (myfile.is_open()) {
        myfile >> total;
        std::getline(myfile, myline);

        while (std::getline(myfile, myline)) {           
            
            int id = std::stoi(myline);
            std::cout << "The id is" << id;

            std::getline(myfile,myline); 
            std::string name = myline.substr(1);


            std::getline(myfile, myline); 
            int year = std::stoi(myline);

            std::getline(myfile, myline);  
            int zip = std::stoi(myline);
            
            std::getline(myfile, myline); 
            std::istringstream iss(myline);
            std::set<int> friends;  
            int friendId;
            
            while (iss >> friendId) { 
                friends.insert(friendId);
            }

            std::getline(myfile, myline);
            std::string path = myline.substr(1);

            //std::cout << "The path to " << name << " is " << path << std::endl;

            User* newUser = new User(id, name, year, zip, friends, path);
            addUser(newUser);

        }
        myfile.close();
    } else {
        return -1;
    }

    return 0;
}

//pre-condition: none
//post-condition: write network user info to file
int Network::writeUsers(const char* fname) {
    std::ofstream myfile(fname);
    if (myfile.is_open()) {
        myfile << numUsers() << std::endl;
        for (auto* user : users_) {
            myfile << user->getId() << std::endl;
            myfile << "\t" << user->getName() << std::endl;
            myfile << "\t" << user->getYear() << std::endl;
            myfile << "\t" << user->getZip() << std::endl;

            std::set<int> fset = user->getFriends(); 
            myfile << "\t";
            for (auto friendId : fset) {
                myfile << friendId << " ";
            }
            myfile << std::endl;

            myfile << "\t" << user->getAvatarPath() << std::endl;
        }
        myfile.close();
        return 0;
    } else {
        return -1; 
    }
}

//pre-condition: none
//post-condition: returns total number of users in network
int Network::numUsers() const{
    return users_.size();
}

//pre-condition: none
//post-condition: prints user id and user name line by line
void Network::printUsers(){
    for (auto* user : users_){
        std::cout << user->getId() << " " << user->getName() << std::endl;
    }
    return;
}

//pre-condition: user should be valid name
//post-condition: prints all friends of a specified user
void Network::printFriends(std::string& name){
    for (auto* user : users_){
        if (user->getName() == name){
            std::set<int> fset = user->getFriends();
            for (auto friendId : fset){
                User* friendUser = getUser(friendId);
                if (friendUser != nullptr){
                std::cout << friendId << " " << friendUser->getName() << std::endl;
                }
            }
        }
        
    }
    return;
}    
std::vector<int> Network::shortestPath(int from, int to){
    int n = numUsers();
    std::queue<int> q;
    std::vector<bool> visited (n, 0);
    std::vector<int> prev (n, -1);

    
    visited[from] = true;
    q.push(from);

    while (q.size() > 0){
        int cur = q.front();
        q.pop();
        std::set<int> adjList = getUser(cur)->getFriends(); //get adj list for cur user which is cur user's friends
        for (auto neighbor : adjList){
            if (!visited[neighbor]){
                prev[neighbor] = cur;
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    if (!visited[to]) {
        return std::vector<int>{};
    }
    std::vector<int> output;
    int cur=to;
    while (cur != -1){
        output.push_back(cur);
        cur = prev[cur];
    }

    std::reverse(output.begin(),output.end());

    return output;
}

std::vector<int> Network::distanceUser(int from, int& to, int distance){
    int n = numUsers();
    std::queue<int> q;
    std::vector<bool> visited (n, 0);
    std::vector<int> prev (n, -1);
    std::vector<int> dist (n,-1);

    if (getUser(from) == nullptr){
        to = -1;
        return std::vector<int> {};

    }
    

    dist[from] = 0;
    visited[from] = true;
    q.push(from);

    while (q.size() > 0){
        int cur = q.front();
        q.pop();
        std::set<int> adjList = getUser(cur)->getFriends(); //get adj list for cur user which is cur users friends
        for (auto neighbor : adjList){
            if (!visited[neighbor]){
                prev[neighbor] = cur;
                dist[neighbor] = dist[cur]+1;
                visited[neighbor] = true;
                q.push(neighbor);  

                if(dist[neighbor] == distance){
                    to = neighbor; // update **to** to neighbor
                    break;
                } 
            }
            
        }
    }
    std::vector<int> output;
    int cur = to;
    if (to == -1){
        return std::vector<int> {};
    } else{
    while (cur != -1){
        output.push_back(cur);
        cur = prev[cur];
    }
    

    std::reverse(output.begin(),output.end());


    return output;
    }
}

std::set<int> Network::setIntersection(const std::set<int>& set0, const std::set<int>& set1) {
    std::set<int> intersection;  //https://stackoverflow.com/questions/13448064/how-to-find-the-intersection-of-two-stl-sets
    for (const auto& value : set0)
        if (set1.find(value) != set1.end())
            intersection.insert(value);
    return intersection;
}

//pre: user exists
//post: returns vector of friends 2 away from user but not direct
std::vector<int> Network::suggestFriends(int who, int& score){
    User* user = getUser(who);
    std::set<int> Userset = user->getFriends();
    std::multimap<int, std::string> m;
    for(auto person : users_){
        auto check = Userset.find(person->getId());
        if (person != user && check == Userset.end()){
            std::set<int> sett = person->getFriends();
            std::set<int> intersection = setIntersection(Userset, sett);
            int size = intersection.size(); //score
            std::string name= person->getName();
            m.insert ( std::pair<int, std::string>(size,name) );
        }
    }


    std::vector<int> s;
    auto largest = m.rbegin()->first;
    if (largest==0){
        return s;
    } else{
        for (auto it = m.rbegin();it != m.rend() && it->first == largest;++it){
            s.push_back(getId(it->second));
        }

        score = largest;


        std::reverse(s.begin(),s.end());

        return s;

    }

}
//pre: none
//post: returns vector of vector of containing ids of each graph
std::vector<std::vector<int> > Network::groups(){
    std::vector<std::vector<int>> v;
    //int source = 0;
    int n = numUsers();
    
    std::vector<bool> visited(n,0);
    
    for(int i=0; i<n; i++){
        if (!visited[i]){
            std::vector<int> vv;
            visited[i] = true;
            std::stack<int> stck;
            stck.push(i);
            vv.push_back(i);
        

            while (!stck.empty()){
                int cur = stck.top();
                stck.pop();
                std::set<int> adjList = getUser(cur)->getFriends();
                for (auto neighbor :adjList){
                    if (!visited[neighbor]){
                        visited[neighbor] = true;
                        stck.push(neighbor);
                        vv.push_back(neighbor);

                    }          
                }
            }
            
          v.push_back(vv);
        }
        
    }

    return v;

}


//pre: parameters must be valid
//post: new post is added to the network 
void Network::addPost(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, bool isPublic) {
    User* owner = getUser(ownerId);


    if (isIncoming) {
        Post* newPost = new IncomingPost(numPosts(), ownerId, message, likes, isPublic, authorName);
        owner->User::addPost(newPost);
    } else {
        Post* newPost = new Post(numPosts(), ownerId, message, likes);
        owner->User::addPost(newPost);
    }

    
}

//pre: file name must be valid
//post: reads post from post file and adds to network
int Network::readPosts(char* fname){
    std::string myline;
    std::ifstream myfile(fname);
    int total;

    if (myfile.is_open()){
        myfile >> total;
        std::getline(myfile, myline);

        while(std::getline(myfile, myline)){
            int messageId = std::stoi(myline); 

            std::getline(myfile, myline);
            std::string message = myline.substr(1);

            std::getline(myfile, myline);
            int ownerId = std::stoi(myline);

            std::getline(myfile, myline);
            int likes = std::stoi(myline); 

            std::getline(myfile, myline); 
            myline.erase(std::remove(myline.begin(), myline.end(), '\t'), myline.end());
            std::string s = myline;

            std::getline(myfile, myline);
            myline.erase(std::remove(myline.begin(), myline.end(), '\t'), myline.end());
            std::string authorName = myline; 


            if (!s.empty()){
                if (s == "public"){
                    addPost(ownerId, message,likes, true, authorName, true);
                } else{
                    addPost(ownerId, message,likes, true, authorName, false);
                }

            } else{
                addPost(ownerId, message,likes, false, authorName, false);
            }
        
        }
        myfile.close();
    } else {
        return -1;
    }
    return 0;
}


//pre: none
//post: returns number of posts in file
int Network::numPosts() {
    int num = 0;
    for (auto i : users_) {
        num += i->User::getPosts().size();
        }
    return num;
}  


//pre: ownerId must be valid, and int howMany > 0
//post: return string of the specified number of users
std::string Network::getPostsString(int ownerId, int howMany, bool showOnlyPublic){

    User* user = getUser(ownerId);
    std::string s;
    s = user->getPostsString(howMany, showOnlyPublic);  

    return s;
}



//pre: valid pointers
//post: returns true if message id of p1 is less than messae id of p2
bool cmp(Post* p1, Post* p2){
    return p1->getMessageId() < p2->getMessageId();
}

//pre: valid file name
//post: writes posts from network to file
int Network::writePosts(char* fname) {
    std::ofstream myfile(fname);
    std::cout << "this is the write posts test " <<  std::endl;
    if (myfile.is_open()) {
        std::vector<Post*> bigPostVector;
        myfile << numPosts() << std::endl;
        for (auto user : users_) {
            std::vector<Post*> posts = user->User::getPosts();
            for (auto p :posts){
                bigPostVector.push_back(p);
            }
            
        }
        std::sort (bigPostVector.begin(), bigPostVector.end(), cmp);
            for (auto post : bigPostVector) {
                myfile << post->getMessageId() << std::endl;

                myfile << "\t" << post->getMessage() << std::endl;
                myfile << "\t" << post->getOwnerId() << std::endl;
                myfile << "\t" << post->getLikes() << std::endl;

                if (post->isIncoming()) {
                    if (post->getIsPublic()) {
                        myfile << "\tpublic" << std::endl;
                    } else {
                        myfile << "\tprivate" << std::endl;
                    }
                    myfile << "\t" << post->getAuthor() << std::endl;
                }

                if (!post->isIncoming()) {
                    // Two blank lines after regular post
                    myfile << std::endl;
                    myfile << std::endl;
                }
            }
            myfile.close();
        return 0;
        } else{
            return -1;
        }
        
}