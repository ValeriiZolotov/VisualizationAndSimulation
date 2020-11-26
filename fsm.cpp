#include "fsm.h"

FSM::FSM()
{

}

void FSM::patrol()
{
    //qDebug()<<"patrol runns";
    if(mNPC->patrol()>=1.f)// checks if the NPC has arrived to the final point
        mNPCState = LEARN;

}

void FSM::learn()
{
    qDebug()<<"learn runns";

    mNPCState = PATROL;

}
void FSM::setGState(game_state gs)
{
    mGameState = gs;
}
void FSM::setNPCState(npc_state npcs)
{
    mNPCState = npcs;
}
void FSM::run()
{
    if(mGameState==GAME_ACTIVE)
    {
        if (mNPCState == PATROL) patrol();
        if (mNPCState == LEARN) learn();
    }
}
void FSM::updateControllPoints(int index)
{
    mCurve->deletePoint(index);
    std::vector<vec3> points = mCurve->mControllPoints;
    mCurve->reevaluetaBSpline(points,2);
}
