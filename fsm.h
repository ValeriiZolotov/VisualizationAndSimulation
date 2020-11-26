#ifndef FSM_H
#define FSM_H
#include <QObject>
#include <QDebug>
#include "npc.h"
#include "bsplinecurve.h"

enum game_state {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};
enum npc_state
{
    PATROL,
    LEARN
};
class FSM :public QObject
{
    Q_OBJECT
public:
    FSM();
    void assignNPC(NPC* npc){mNPC = npc;}
    void assignCurve(BSplineCurve* curve){mCurve = curve;}
    void run();
    npc_state mNPCState;
    game_state mGameState;
private:
    NPC* mNPC;
    BSplineCurve *mCurve;
public slots:
    void printFSMMessage(QString m){qDebug()<<m;} // for debug purpose , helps to report of the FSM state
    void patrol();
    void learn();
    void setGState(game_state state);
    void setNPCState(npc_state state);
    void updateControllPoints(int index);
signals:
    void sendFSMMEssage(QString m);
    void itemTaken(int index);
    void pathWalked();
    void sendGState(game_state state);
    void sendNPCState(npc_state state);

};


#endif // FSM_H
