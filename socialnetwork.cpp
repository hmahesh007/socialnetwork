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
    ui->avatarButton->setVisible(false);

    connect(ui->myLoginButton, &QPushButton::clicked, this, &socialnetwork::myButtonClick);
    connect(ui->friendTable, &QTableWidget::cellClicked, this, &socialnetwork::FriendClick);
    connect(ui->suggestFriendsTable, &QTableWidget::cellClicked, this, &socialnetwork::suggestedFriendClick);
    connect(ui->addFriendButton, &QPushButton::clicked, this, &socialnetwork::addFriendClick);
    connect(ui->backButton, &QPushButton::clicked, this, &socialnetwork::backButtonClick);
    connect(ui->avatarButton, &QPushButton::clicked, this, &socialnetwork::avatarButtonClick);

}



socialnetwork::~socialnetwork()
{
    delete ui;
}

void socialnetwork::avatarButtonClick(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (QString::compare(filename, QString()) !=0 ){
            QImage image;
            bool valid = image.load(filename);

            if (valid){
                currentUser->setAvatarPath(filename.toStdString());

                int w = ui->labelImage->width ();
                int h = ui->labelImage->height ();
                ui->labelImage->setPixmap(QPixmap::fromImage(image).scaled(w,h, Qt::KeepAspectRatio, Qt::SmoothTransformation));

                n.writeUsers("users.txt");
            } else {
                std:: cout << "error";
            }
        }
}

void socialnetwork::displayAvatar() {
    std::string avatarPath = currentUser->getAvatarPath();

    std::cout << avatarPath << std::endl;
    QImage image;
    if (image.load(QString::fromStdString(avatarPath))) {

        int w = ui->labelImage->width ();
        int h = ui->labelImage->height ();
        ui->labelImage->setPixmap(QPixmap::fromImage(image).scaled(w,h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }



}

void socialnetwork::myButtonClick()
{
    QString username = ui->myLoginLine->text();
    std::string usernameStr = username.toStdString();
    int userId = n.getId(usernameStr);

    if (userId != -1)
    {
        loggedInUser = n.getUser(userId);
        currentUser = n.getUser(userId);
        displayUserProfile();
    }

}

void socialnetwork::displayUserProfile() {
    if (loggedInUser->getId() == -1) {
        return;
    }

    if (loggedInUser->getId() == currentUser->getId()) {
        ui->myProfileLabel->setText("My Profile");
    } else {
        ui->myProfileLabel->setText(QString::fromStdString(currentUser->getName() + "'s Profile"));

    }



    displayAvatar();
    displayFriendList();
    displayRecentPosts();
    displayFriendSuggestions();


    showProfileWidgets();
}




void socialnetwork::displayFriendList() {



    std::set<int> friendIds = currentUser->getFriends();
    int size = friendIds.size();


    ui->friendTable->setRowCount(size);
    ui->friendTable->setColumnCount(1);



    int row = 0;
    for (int id : friendIds) {
        User* friendUser = n.getUser(id);

        QTableWidgetItem *Item = new QTableWidgetItem(QString::fromStdString(friendUser->getName()));
        ui->friendTable->setItem(row, 0, Item);
        row++;

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
    if (item) {
        QString friendName = item->text();
        std::string friendNameStr = friendName.toStdString();
        int friendId = n.getId(friendNameStr);

        User* friendUser = n.getUser(friendId);
        currentUser = n.getUser(friendId);

        currentUser = friendUser;
        displayUserProfile();

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

            std::string loggedInUserName = loggedInUser->getName();

            n.addConnection(loggedInUserName, clickedUserName);


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
        //n.writeUsers("users.txt");
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
        ui->avatarButton->show();
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
        ui->avatarButton->setVisible(false);

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
