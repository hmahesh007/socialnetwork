//post.cpp

#include "post.h"
#include <string>
#include "user.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <set>
#include <queue>
#include <map>
#include <stack>


//pre: none
//post: 0 constructor
Post::Post() : messageId_(0), ownerId_(0), message_(""), likes_(0){}


//pre: none
//post: initializing variables
Post::Post(int messageId, int ownerId, std::string message, int likes) 
    : messageId_(messageId), ownerId_(ownerId), message_(message), likes_(likes) {}


//pre: none
//post: prints post data
std::string Post::toString(){
    std::string s;
    s = message_ + " Liked by " + std::to_string(likes_) + " people";
    
    return s;
}

//pre: none
//post: Post is labeled as not Incoming  Post
bool Post::isIncoming(){
    return false;
}


//pre: none
//post: return message id
int Post::getMessageId(){
    return messageId_;
}

//pre: none
//post: return owner id
int Post::getOwnerId(){
    return ownerId_;
}

//pre: none
//post: return post message string
std::string Post::getMessage(){
    return message_;
}

//pre: none
//post: return int number of likes for post
int Post::getLikes(){
    return likes_;
}

//pre: none
//post: return empty string 
std::string Post::getAuthor(){
    std::string s = "";
    return s;
}

//pre: none
//post: return true for post is public
bool Post::getIsPublic(){
    return true;
}


//IP------------------------------------IP

//pre: none
//post: 0 constructor for Incoming Post
IncomingPost::IncomingPost() : author_(""), isPublic_(false)  {}

//pre: none
//post: initializing constructor for Incoming Post
IncomingPost::IncomingPost(int messageId, int ownerId, std::string message, int likes, bool isPublic, std::string author)
    : Post(messageId, ownerId, message, likes), author_(author), isPublic_(isPublic) {}

//pre: none
//post: return true if post is IncomingPost type
bool IncomingPost::isIncoming(){
    return true;
}

//pre: none
//post: disply contents of Incoming Post
std::string IncomingPost::toString(){
    std::string s;
    if (isPublic_ == true){
    s = author_ + " wrote: " + Post::toString();
    } else {
        s = author_ + " wrote (private)" + ": " + Post::toString();
    }

    return s;
}



//pre: none
//post: return author name
std::string IncomingPost::getAuthor(){
    return author_;
}

//pre: none
//post: return if IncomingPost is public or not
bool IncomingPost::getIsPublic(){
    return isPublic_;
}


