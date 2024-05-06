//user.cpp
#include "user.h"
#include <set>
#include <string>
#include <iostream>

//pre-condition: none
//post-condition: zero constructor which initialized everything to 0 or empty
User::User() : id_(0), name_(""), year_(0), zip_(0), friends_{}, path_("") {}

//pre-condition: none
//post-condition: creates object with values for id, name, zip, and friends set
User::User(int id, const std::string name, int year, int zip, std::set<int> friends, std::string path)
    : id_(id), name_(name), year_(year), zip_(zip), friends_(friends), path_(path) {}


//pre-condition:
//post-condition:
int User::getId() const{
    return id_;
}

//pre-condition:
//post-condition:
std::string User::getName() const{
    return name_;
}

//pre-condition:
//post-condition:
int User::getYear() const{
    return year_;
}

//pre-condition:
//post-condition:
int User::getZip() const{
    return zip_;
}

//pre-condition:
//post-condition:
std::set<int>& User::getFriends(){
    return friends_;
}

//pre-condition:
//post-condition:
void User::addFriend(int id){
        friends_.insert(id);
}

//pre-condition:
//post-condition:
void User::deleteFriend(int id){
    auto pos = friends_.find(id);
    if (pos != friends_.end()){
        friends_.erase(pos);
    }

}


//pre: post must be valid pointer
//post: adds post to messages_ vector
void User::addPost(Post* post){
    messages_.push_back(post);
}

//pre: none
//post: returns vector of all the posts of the user
std::vector<Post*> User::getPosts(){
    return messages_;
}


//pre: howMany > 0
//post: returns string of number of users and bool tells whether it shows public or private      
std::string User::getPostsString(int howMany, bool showOnlyPublic)  {
    std::string s;
    int count = 0;

    for (int i = messages_.size() - 1; i >= 0 && count < howMany; --i) {
         Post* p = messages_[i];  

        if (!showOnlyPublic || p->getIsPublic()) {
            s += p->toString() + "\n";
            count++;
        }
    }

    return s;
}   


//avatar functions

void User::setAvatarPath(std::string path){
    path_ = path;
}
std::string User::getAvatarPath(){
    return path_;
}