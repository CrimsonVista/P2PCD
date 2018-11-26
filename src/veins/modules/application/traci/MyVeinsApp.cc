//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "veins/modules/application/traci/MyVeinsApp.h"

using namespace Veins;
using Veins::TraCIMobility;
using Veins::TraCIMobilityAccess;

Define_Module(Veins::MyVeinsApp);

void MyVeinsApp::initialize(int stage)
{
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        // Initializing members and pointers of your application goes here
        EV << "Initializing " << par("appName").stringValue() << std::endl;

        /*mobility = TraCIMobilityAccess().get(getParentModule());
        traci = mobility->getCommandInterface();
        traciVehicle = mobility->getVehicleCommandInterface();*/

    }
    else if (stage == 1) {
        // Initializing members that require initialized other modules goes here

        EV << "Trying to get TRACI for module " << getParentModule() << std::endl;
        mobility = TraCIMobilityAccess().get(getParentModule());

        if (mobility) {
            EV << "Got TRACI " << mobility << " at stage " << stage <<". Time to set speed mode to 0" << std::endl;

            traci = mobility->getCommandInterface();
            traciVehicle = mobility->getVehicleCommandInterface();

            traciVehicle->setSpeedMode(0); // should disable collision checks
            EV << "set speed mode to 0" << std::endl;
        } else {
            EV << "Even at stage " << stage << ", mobility was null" << std::endl;
        }
    }
}

void MyVeinsApp::finish()
{
    BaseWaveApplLayer::finish();
    // statistics recording goes here
}

void MyVeinsApp::onBSM(BasicSafetyMessage* bsm)
{
    // Your application has received a beacon message from another car or RSU
    // code for handling the message goes here
    EV << "BSM received at ID " << myId << std::endl;
    EV << " BSM from other vehcile with position " << bsm->getSenderPos() << std::endl;
    EV << " BSM from other vehicle with speed " << bsm->getSenderSpeed() << std::endl;
    EV << " BSM from other vehicle with acceleration" << bsm->getSenderAcceleration() << std::endl;
    EV << " BSM from other vehicle with direction " << bsm->getSenderDirection() << std::endl;
    EV << " BSM from other vehicle with orientation " << bsm->getSenderOrientation() << std::endl;

    /* Do rear end check. If rear end is imminent, change lanes */
    /* Determine if my direction is the same direction */
    bool sameDirection = (bsm->getSenderDirection() == this->curDirection);
    if (sameDirection) {
        EV << "Other vehicle moving in the same direction. Check rear-end collision?" << std::endl;
        bool collisionCourse = false;

        /* Determine simple collisions only in one dimension (x or y) */
        if (this->curDirection.x != 0) {
            EV << "Movement in X. Check if x will hit and if y is the same" << std::endl;
            bool xClosing = (this->curDirection.x > 0 ? (this->curPosition.x > bsm->getSenderPos().x) : (this->curPosition.x < bsm->getSenderPos().x));
            double yDiff = (this->curPosition.y - bsm->getSenderPos().y);
            if (xClosing && (yDiff < 1.0 || yDiff > -1.0)) {
                EV << "Potential rear-end collision course on y axis" << std::endl;
                collisionCourse = true;
            }
        } else if (this->curDirection.y != 0) {
            EV << "Movement in Y. Check if y will hit and if x is the same" << std::endl;
            bool yClosing = (this->curDirection.y > 0 ? (this->curPosition.y > bsm->getSenderPos().y) : (this->curPosition.y < bsm->getSenderPos().y));
            double xDiff = (this->curPosition.x - bsm->getSenderPos().x);
            if (yClosing && (xDiff < 1.0 || xDiff > -1.0)) {
                EV << "Potential rear-end collision course on x axis" << std::endl;
                collisionCourse = true;
            }
        }

        if (collisionCourse) {
            EV << "Vehicle on a rear-end collision course. Is it moving faster than I am?" << std::endl;
            if (bsm->getSenderAcceleration() > this->curAcceleration) {
                EV << "Other vehicle is accelerating faster than we are on a collision course. Get Off the Road!" << std::endl;
                //traciVehicle->remove(1); // 1 is for "parking"
            }
        }
    }

}

void MyVeinsApp::onWSM(WaveShortMessage* wsm)
{
    // Your application has received a data message from another car or RSU
    // code for handling the message goes here, see TraciDemo11p.cc for examples
}

void MyVeinsApp::onWSA(WaveServiceAdvertisment* wsa)
{
    // Your application has received a service advertisement from another car or RSU
    // code for handling the message goes here, see TraciDemo11p.cc for examples
}

void MyVeinsApp::handleSelfMsg(cMessage* msg)
{
    BaseWaveApplLayer::handleSelfMsg(msg);
    // this method is for self messages (mostly timers)
    // it is important to call the BaseWaveApplLayer function for BSM and WSM transmission
}

void MyVeinsApp::handlePositionUpdate(cObject* obj)
{
    BaseWaveApplLayer::handlePositionUpdate(obj);
    // the vehicle has moved. Code that reacts to new positions goes here.
    // member variables such as currentPosition and currentSpeed are updated in the parent class
}
