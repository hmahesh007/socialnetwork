//user.h
#ifndef USER_H
#define USER_H

#include "post.h"
#include <set>
#include <string>
#include <vector>



class User{
public:
    User();
    User(int id, const std::string name, int year, int zip, std::set<int> friends, std::string path);

    int getId() const;
    std::string getName() const;
    int getYear() const;
    int getZip() const;
    std::set<int>& getFriends();

    void addFriend(int id);
    void deleteFriend(int id);

    void addPost(Post* post);
    std::vector<Post*> getPosts();
    std::string getPostsString(int howMany, bool showOnlyPublic); 

    void setAvatarPath(std::string path);
    std::string getAvatarPath();
private:
    int id_;
    std::string name_;
    int year_;
    int zip_;
    std::set<int> friends_;
    std::vector<Post*> messages_;
    std::string path_;


};









#endif