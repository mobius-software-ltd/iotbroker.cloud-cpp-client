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
#include <mqtt/mqttmanager.h>
#include <database/accountmanager.h>
#include <logindialog.h>
#include <loadingdialog.h>
#include <accountslistdialog.h>
#include <timer/timersmap.h>

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

    MQTTManager *mqtt;
    AccountManager *accountManager;
    LogInDialog *login;
    AccountsListDialog *accountsListDialog;
    AccountEntity currentAccount;
    TimersMap *timerMap;
    QTimer *progressTimer;

    void startNewSession();
    void initMainWindow();
    bool showLogIn();
    void showLoading();
    void saveTopic(Publish *publish, bool isIncoming);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:

    // Accounts list dialog
    void deleteAccountSlot(AccountEntity *account);
    void accountDidSelectSlot(AccountEntity *account);
    void newAccountDidClickSlot();
    //

    //Account list dialog
    void saveNewAccount(AccountEntity account);
    //

    // Topic list tab
    void willSubscribeSlot(QString topicName, int qos);
    void willUnsubscribeSlot(QString topicName, int qos);
    //

    // Send message tab
    void willPublishSlot(Publish *publish);
    //

    // MQTT Manager
    void openConnectionSlot();
    void closeConnectionSlot();
    void connackDidReceiveSlot(Connack *connack);
    void publishDidReceiveSlot(Publish *publish);
    void pubackDidReceiveSlot(Puback *puback);
    void pubrecDidReceiveSlot(Pubrec *pubrec);
    void pubrelDidReceiveSlot(Pubrel *pubrel);
    void pubcompDidReceiveSlot(Pubcomp *pubcomp);
    void subackDidReceiveSlot(Suback *suback);
    void unsubackDidReceiveSlot(Unsuback *unsuback);
    void pingrespDidReceiveSlot(Pingresp *pingresp);
    void errorMessageDidReceiveSlot(QString *error);
    void errorSlot(QString *error);
    //

    // QTabWidget
    void logoutSlot(int tabIndex);
    //

    // QProgressBar
    void timeoutProgressBar();
    //
};

#endif // MAINWINDOW_H
