#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <sstream>
#include "user.h"
#include "network.h"

int main(int argc, char* argv[]) {


    Network network;

    if (network.readUsers(argv[1]) != 0) {
        std::cerr << "Error reading users" << std::endl;
        return -1;
    }

    network.readPosts(argv[2]);

    int choice;
    std::string line;
    do {

        std::cout << "Menu:" << std::endl;
        std::cout << "1. Add a user" << std::endl;
        std::cout << "2. Add friend connection" << std::endl;
        std::cout << "3. Delete friend connection" << std::endl;
        std::cout << "4. Print users" << std::endl;
        std::cout << "5. Print friends" << std::endl;
        std::cout << "6. Write to file" << std::endl;
        std::cout << "7. Shortes path between two users" << std::endl;
        std::cout << "8. User at a given Distance" << std::endl;
        std::cout << "9. Friend Suggestions" << std::endl;
        std::cout << "10. Groups with DFS" << std::endl;
        std::cout << "11. Display User Posts" << std::endl;
        std::cout << "12. Exit" << std::endl;
        

        std::getline(std::cin, line);
        std::stringstream ss(line);
        ss >> choice;

        switch (choice) {
            case 1: {
                int total = network.numUsers();
                std::string fname, lname, name, yearStr, zip;
                ss >> fname >> lname >> yearStr >> zip;
                name = fname + " " + lname;
                std::string avatarPath = "./image.png";


                User* u = new User(total, name, std::stoi(yearStr), std::stoi(zip), std::set<int> {}, avatarPath);
                network.addUser(u);
                break;
            }

            case 2: {
                std::string fname1, lname1, fname2, lname2, name1, name2;
                ss >> fname1 >> lname1 >> fname2 >> lname2;
                name1 = fname1 + " " + lname1;
                name2 = fname2 + " " + lname2;
                network.addConnection(name1, name2);
                break;
            }
            case 3: {
                std::string fname1, lname1, fname2, lname2, name1, name2;
                ss >> fname1 >> lname1 >> fname2 >> lname2;
                name1 = fname1 + " " + lname1;
                name2 = fname2 + " " + lname2;
                network.deleteConnection(name1, name2);
                break;
            }
            case 4: {
                network.printUsers();
                break;
            }
            case 5: {
                std::string fname,lname, name;
                ss >> fname >> lname;
                name = fname + " " + lname;
                network.printFriends(name); 
                break;
            }
            case 6: {
                std::string filename;
                ss >> filename;
                if (network.writeUsers(filename.c_str()) != 0){
                    std::cerr << "Error writing to " << filename;
                }
                std::cout << network.numUsers() << std::endl;
                break;
            }
            case 7: {
                std::string fname1, lname1, fname2, lname2, name1, name2;
                ss >> fname1 >> lname1 >> fname2 >> lname2;
                name1 = fname1 + " " + lname1;
                name2 = fname2 + " " + lname2;

                int id1 = network.getId(name1);
                int id2 = network.getId(name2);

                if (id1 == id2) {
                    std::cout << "Distance: 0" << std::endl;
                    std::cout << name1 << std::endl;
                } else {
                    std::vector<int> path = network.shortestPath(id1, id2);

                    if (path.empty()) {
                        std::cout << "None" << std::endl;
                    } else {
                        int distance = path.size() - 1;
                        std::cout << "Distance: " << distance << std::endl;

                        for (int i = 0; i < path.size(); i++) {
                            std::cout << network.getUser(path[i])->getName();
                            if (i < path.size() - 1) {
                                std::cout << " -> ";
                            }
                        }
                        std::cout << std::endl;
                    }
                }
                break;
            }

            case 8:{
                std::string fname,lname, name, dist;
                ss >> fname >> lname >> dist;
                name = fname + " " + lname;
                int from = network.getId(name);
                
                
                int to = -1;
                int distInt = std::stoi(dist);   

                std::vector<int> path = network.distanceUser(from, to, distInt);
                if (path.size()!=0){
                std::cout << network.getUser(path.back())->getName() << ": ";
                for (int i =0; i < path.size(); i++){
                    std::cout << network.getUser(path[i])->getName();
                    if (i < path.size()-1){
                        std::cout << " -> " ;
                    }
                   
                }
                std::cout << std::endl;
                }
                 
                break;
                
                

            }
            case 9:{
                std::string fname,lname, name;
                ss >> fname >> lname;
                name = fname + " " + lname;
                int who = network.getId(name);
                int score = 0;
                std::vector<int> flist = network.suggestFriends(who, score);
                if (score== 0){
                    std::cout << "None" << std::endl;
                } else{
                std::cout << "The suggested friend(s) is/are: " << std::endl;
                for (auto sf: flist){
                    std::cout << network.getUser(sf)->getName() << " Score: " << score << std::endl;
                }
                }
                break;
            }
            case 10:{
                std::vector<std::vector<int>> v;
                v = network.groups();
                for (int i = 0; i < v.size(); i++) {
                    std::cout << "Set " << i+1 << " => ";
                    for (int j = 0; j < v[i].size(); j++) {
                        std::string name = (network.getUser(v[i][j]))->getName();
                        std::cout << name;
                        if (j < v[i].size()-1){
                            std::cout << ", ";
                        }
                    }
                    std::cout << std::endl; 
                }
                break;

            }

            case 11: {
                std::string fname, lname, name, num;
                ss >> fname >> lname >> num;
                name = fname + " " + lname;
                int numInt;
                numInt = std::stoi(num);

                User* user = network.getUser(network.getId(name));
                if (user != nullptr) {  //do get posts strings with the howmany parameter
                    std::vector<Post*> posts = user->getPosts();
                    std::string postsString = user->getPostsString(numInt, false); // Display both public and private posts
                    std::cout << postsString;
                    if (!postsString.empty()){
                            std::cout << std::endl;
                        }
                } 
                
                break;
            }

            case 12:
                std::cout << "Exiting program." << std::endl;
                break;
            default:
                std::cerr << "Invalid choice." << std::endl;
                choice = 12;
                break;
        }
    } while (choice !=12);

    return 0;
}