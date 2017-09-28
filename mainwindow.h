/**
 * Mobius Software LTD
 * Copyright 2015-2017, Mobius Software LTD
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
#include "logindialog.h"
#include "loadingdialog.h"
#include "accountslistdialog.h"
#include "database/accountmanager.h"
#include "iot-protocols/mqtt/classes/will.h"
#include "iot-protocols/mqtt/mqtt.h"
#include "iot-protocols/mqtt-sn/mqttsn.h"
#include "iot-protocols/coap/coap.h"
#include "iot-protocols/amqp/classes/amqp.h"

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
    LogInDialog *login;
    AccountsListDialog *accountsListDialog;
    AccountEntity currentAccount;
    QTimer *progressTimer;
    LoadingDialog *loading;

    void startNewSession();
    void initMainWindow();
    bool showLogIn();
    void showLoading();
    void saveTopic(QString topicName, int qos, QByteArray content, bool isRetain, bool isDub, bool isIncoming);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:

    //Iot protocol
    void connackReceived(IotProtocol*,int);
    void publishReceived(IotProtocol*,QString,int,QByteArray,bool,bool);
    void pubackReceived(IotProtocol*,QString,int,QByteArray,bool,bool,int);
    void subackReceived(IotProtocol*,QString,int,int);
    void unsubackReceived(IotProtocol*,QString);
    void pingrespReceived(IotProtocol*);
    void disconnectReceived(IotProtocol*);
    void timeout(IotProtocol*);
    void errorReceived(IotProtocol*,QString);
    //

    // Accounts list dialog
    void deleteAccountSlot(AccountEntity *account);
    void accountDidSelectSlot(AccountEntity *account);
    void newAccountDidClickSlot();
    //

    //Account list dialog
    void saveNewAccount(AccountEntity account);
    //

    // Topic list tab
    void willSubscribeSlot(TopicEntity topic);
    void willUnsubscribeSlot(TopicEntity topic);
    //

    // Send message tab
    void willPublishSlot(MessageEntity message);
    //

    // QTabWidget
    void logoutSlot(int tabIndex);
    //

    // QProgressBar
    void timeoutProgressBar();
    //
};

#endif // MAINWINDOW_H
