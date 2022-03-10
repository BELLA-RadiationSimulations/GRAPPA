/* Copyright 2022 Davide Terzani
 *
 * This file is part of GRAPPA.
 *
 * License: BSD-3-Clause
 */

#include <MyActionInitialization.hpp>

MyActionInitialization::MyActionInitialization(HistandNTupleManager* myanalysismanager)
    : G4VUserActionInitialization()
{
    m_HistandNTupleManager = myanalysismanager;
}

MyActionInitialization::~MyActionInitialization()
{

}

void MyActionInitialization::BuildForMaster() const
{
    MyRunAction* runAction = new MyRunAction(m_HistandNTupleManager);
    SetUserAction(runAction);
}

void MyActionInitialization::Build() const
{
    MyPrimaryGeneratorAction* primarygeneratoraction =
        new MyPrimaryGeneratorAction();
    SetUserAction(primarygeneratoraction);

    MyRunAction* runAction = new MyRunAction(m_HistandNTupleManager);
    SetUserAction(runAction);

    MyTrackingAction* trackingAction = new MyTrackingAction(m_HistandNTupleManager);
    SetUserAction(trackingAction);

}
