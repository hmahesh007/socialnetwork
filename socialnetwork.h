
#ifndef SOCIALNETWORK_H
#define SOCIALNETWORK_H

#include <QMainWindow>
#include "QtCore/qtconfigmacros.h"
#include "QtWidgets/qtablewidget.h"
#include <QFileDialog>
#include "user.h"
#include "network.h"
#include "post.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class socialnetwork;
}
QT_END_NAMESPACE

class socialnetwork : public QMainWindow
{
    Q_OBJECT

public:
    socialnetwork(QWidget *parent = nullptr);
    ~socialnetwork();

    void myButtonClick();
    void FriendClick(int row, int column);
    void suggestedFriendClick(int row, int column);
    void addFriendClick();
    void displayUserProfile();
    void displayPosts();
    void displayFriendSuggestions();
    void displayFriendList();
    void displayRecentPosts();
    void backButtonClick();
    void showProfileWidgets();

    void displayAvatar();
    void avatarButtonClick();

    void hideAll();




private:
    Ui::socialnetwork *ui;
    Network n;
    User* loggedInUser;
    User* currentUser;


};
#endif // SOCIALNETWORK_H
