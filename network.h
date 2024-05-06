//network.h
#ifndef NETWORK_H
#define NETWORK_H

#include "user.h"


class Network{
public:

    Network();
    //~Network();
    User* getUser(int id);
    void addUser(User* user);
    int addConnection(const std::string s1, const std::string s2);
    int deleteConnection(const std::string s1, const std::string s2);
    int getId(const std::string& name);
    int readUsers(const char* fname);
    int numUsers() const;
    void printUsers();
    void printFriends(std::string& name);
    int writeUsers(const char* fname);

    std::vector<int>shortestPath(int from, int to);
    std::vector<int>distanceUser(int from, int& to, int distance);
    std::vector<int>suggestFriends(int who, int& score);
    std::set<int> setIntersection(const std::set<int>& set0, const std::set<int>& set1);
    std::vector<std::vector<int>> groups();

    int numPosts();
    void addPost(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, bool isPublic);
    std::string getPostsString(int ownerId, int howMany, bool showOnlyPublic);
    int readPosts(char* fname);
    int writePosts(char* fname);



private:
    std::vector<User*> users_;

};

#endif







