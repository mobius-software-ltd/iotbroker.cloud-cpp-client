/**
 * Mobius Software LTD
 * Copyright 2015-2018, Mobius Software LTD
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database/accountmanager.h"
#include "iot-protocols/iotprotocol.h"
#include "loadingform.h"
#include "loginform.h"
#include "generalform.h"
#include "accountlistform.h"
#include <QSslSocket>

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

    IotProtocol *iotProtocol;
    AccountManager *accountManager;

    AccountListForm *accountListForm;
    GeneralForm *generalForm;
    LoadingForm *loadingForm;
    LoginForm *loginForm;

    QTimer *progressTimer;

    void init();
    void startWithAccount(AccountEntity account);
    void setSizeToWindowWithCentralPosition(QSize size);
    void saveTopic(QString topicName, int qos, QByteArray content, bool isRetain, bool isDub, bool isIncoming);
    bool isTopicAlreadyExistForCurrentAccount(QString topic);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:

    // AccountListForm
    void accountDidSelect(AccountEntity* account);
    void deleteAccount(AccountEntity* account);
    void newAccountDidClick();

    // GeneralForm
    void willSubscribeToTopic(TopicEntity topicEntity);
    void willUnsubscribeFromTopic(TopicEntity topicEntity);
    void willPublish(MessageEntity message);
    void topicsTabDidClick();
    void messagesTabDidCLick();
    void logoutTabDidCLick();

    // LoginForm
    void loginWithAccount(AccountEntity account);
    void securityKeyCellDidClick();
    void needToResizeLoginForm(LoginForm *form);

    //Iot protocol
    void connackReceived(IotProtocol*,int);
    void publishReceived(IotProtocol*,QString,int,QByteArray,bool,bool);
    void pubackReceived(IotProtocol*,QString,int,QByteArray,bool,bool,int);
    void subackReceived(IotProtocol*,QString,int,int);
    void unsubackReceived(IotProtocol*,QString);
    void disconnectReceived(IotProtocol*);
    void timeout(IotProtocol*);
    void errorReceived(IotProtocol*,QString);

    // QProgressBar
    void timeoutProgressBar();

};

#endif // MAINWINDOW_H
