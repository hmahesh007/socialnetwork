#include "socialnetwork.h"
#include "ui_socialnetwork.h"
#include <iostream>

socialnetwork::socialnetwork(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::socialnetwork)
{
    ui->setupUi(this);
    n.readUsers("users.txt");
    n.readPosts("posts.txt");
    ui->addFriendButton->setVisible(false);
    ui->backButton->setVisible(false);
    ui->friendTable->setVisible(false);
    ui->myProfileLabel->setVisible(false);
    ui->postsLabel->setVisible(false);
    ui->suggestFriendsTable->setVisible(false);

    connect(ui->myLoginButton, &QPushButton::clicked, this, &socialnetwork::myButtonClick);
    connect(ui->friendTable, &QTableWidget::cellClicked, this, &socialnetwork::FriendClick);
    connect(ui->suggestFriendsTable, &QTableWidget::cellClicked, this, &socialnetwork::suggestedFriendClick);
    connect(ui->addFriendButton, &QPushButton::clicked, this, &socialnetwork::addFriendClick);
    connect(ui->backButton, &QPushButton::clicked, this, &socialnetwork::backButtonClick);

}

socialnetwork::~socialnetwork()
{
    delete ui;
}

void socialnetwork::myButtonClick()
{
    QString username = ui->myLoginLine->text();
    std::string usernameStr = username.toStdString();
    int userId = n.getId(usernameStr);

    if (userId != -1)
    {
        std::cout << "Logged in Successfully" << std::endl;
        loggedInUser = n.getUser(userId);
        currentUser = n.getUser(userId);
    }
    else
    {
        std::cout << "Invalid username" << std::endl;
    }
   displayUserProfile();
}

void socialnetwork::displayUserProfile() {
    if (loggedInUser->getId() == -1) {
        std::cout << "Invalid" << std::endl;
        return;
    }

    if (loggedInUser->getId() == currentUser->getId()) {
        ui->myProfileLabel->setText("My Profile");
    } else {
        ui->myProfileLabel->setText(QString::fromStdString(currentUser->getName() + "'s Profile"));

    }

    displayFriendList();
    displayRecentPosts();
    displayFriendSuggestions();


    showProfileWidgets();
}




void socialnetwork::displayFriendList() {



    std::set<int> friendIds = currentUser->getFriends();
    int size = friendIds.size();

    //std::cout << size;

    ui->friendTable->setRowCount(size);
    ui->friendTable->setColumnCount(1);



    int row = 0;
    for (int id : friendIds) {
        User* friendUser = n.getUser(id);
        if (friendUser != nullptr) {
            QTableWidgetItem *Item = new QTableWidgetItem(QString::fromStdString(friendUser->getName()));
            ui->friendTable->setItem(row, 0, Item);
            row++;
        }
    }
}


void socialnetwork::displayRecentPosts() {

    if (loggedInUser == currentUser){
    std::string recentPostsString = n.getPostsString(currentUser->getId(), 5, false);
    ui->postsLabel->setText(QString::fromStdString(recentPostsString));
    } else if (loggedInUser != currentUser){
        std::string recentPostsString = n.getPostsString(currentUser->getId(), 5, true);
        ui->postsLabel->setText(QString::fromStdString(recentPostsString));
    }
}

void socialnetwork::FriendClick(int row, int column) {
    QTableWidgetItem* item = ui->friendTable->item(row, column);
    std::cout << row << std::endl;
    std::cout << column << std::endl;
    if (item) {
        QString friendName = item->text();
        std::string friendNameStr = friendName.toStdString();
        int friendId = n.getId(friendNameStr);

        User* friendUser = n.getUser(friendId);
        currentUser = n.getUser(friendId);
        if (friendUser != nullptr) {
            currentUser = friendUser;
            displayUserProfile();
        }
    }
}

void socialnetwork::displayFriendSuggestions() {

    std::vector<int> suggestedFriendIds;
    int score;
    suggestedFriendIds = n.suggestFriends(currentUser->getId(), score);

    int size = suggestedFriendIds.size();

    ui->suggestFriendsTable->setRowCount(size);
    ui->suggestFriendsTable->setColumnCount(1);



    int row = 0;
    for (int id : suggestedFriendIds) {
        User* suggestedFriendUser = n.getUser(id);
        std::cout << id << std::endl;
        if (suggestedFriendUser != nullptr) {
            QTableWidgetItem *Item = new QTableWidgetItem(QString::fromStdString(suggestedFriendUser->getName()));
            ui->suggestFriendsTable->setItem(row, 0, Item);
            row++;
        }
    }
}

void socialnetwork::suggestedFriendClick(int row, int column) {
    if (column == 0) {
        QTableWidgetItem* item = ui->suggestFriendsTable->item(row, column);
        if (item) {
            QString userName = item->text();
            std::string clickedUserName = userName.toStdString();
            std::cout << "Clicked username: " << clickedUserName << std::endl;

            std::string loggedInUserName = loggedInUser->getName();

            n.addConnection(loggedInUserName, clickedUserName);
            std::cout << "Connection added between " << loggedInUserName << " and " << clickedUserName << std::endl;

            n.writeUsers("users.txt");

            displayFriendSuggestions();
            displayFriendList();


        }
    }
}

void socialnetwork::addFriendClick(){

        std::string loggedInUserName = loggedInUser->getName();
        std::string currentUserName = currentUser->getName();
        if (loggedInUserName != currentUserName){
        n.addConnection(loggedInUserName, currentUserName);
            n.writeUsers("users.txt");
        std::cout << "Friendship Connection added betwen " << loggedInUser->getName() << " and " << currentUser->getName() << std::endl;
        } else{
            std::cout << "Failed to add friendship";
        }


}

void socialnetwork::backButtonClick(){
    hideAll();

    currentUser = loggedInUser;

    displayUserProfile();


}

void socialnetwork::showProfileWidgets() {
    std::string loggedInUserName = loggedInUser->getName();
    std::string currentUserName = currentUser->getName();
    if (loggedInUserName== currentUserName){
    ui->label->setVisible(false);
    ui->myLoginButton->setVisible(false);
    ui->myLoginLine->setVisible(false);

    ui->friendTable->show();
    ui->myProfileLabel->show();
    ui->suggestFriendsTable->show();
    ui->postsLabel->show();
    ui->addFriendButton->setVisible(false);
    ui->backButton->setVisible(false);
    } else {
        ui->label->setVisible(false);
        ui->myLoginButton->setVisible(false);
        ui->myLoginLine->setVisible(false);

        ui->friendTable->show();
        ui->myProfileLabel->show();
        ui->suggestFriendsTable->setVisible(false);
        ui->postsLabel->show();
        ui->addFriendButton->show();
        ui->backButton->show();

    }


}

void socialnetwork::hideAll(){
    ui->addFriendButton->setVisible(false);
    ui->backButton->setVisible(false);
    ui->friendTable->setVisible(false);
    ui->label->setVisible(false);
    ui->myLoginButton->setVisible(false);
    ui->myLoginLine->setVisible(false);
    ui->myProfileLabel->setVisible(false);
    ui->postsLabel->setVisible(false);
    ui->suggestFriendsTable->setVisible(false);
}
