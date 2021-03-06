//
// Copyright (C) 2011 David Eckhoff <eckhoff@cs.fau.de>
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

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include <openssl/err.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include "omnetpp.h"

// #define DBG_APP std::cerr << "[" << simTime().raw() << "] " << getParentModule()->getFullPath() << " "

#ifndef DBG_APP
#define DBG_APP EV
 #define EV std::cout
#endif

using namespace Veins;

const simsignalwrap_t BaseWaveApplLayer::mobilityStateChangedSignal = simsignalwrap_t(MIXIM_SIGNAL_MOBILITY_CHANGE_NAME);
const simsignalwrap_t BaseWaveApplLayer::parkingStateChangedSignal = simsignalwrap_t(TRACI_SIGNAL_PARKING_CHANGE_NAME);

const char *rsa_test_key = "-----BEGIN RSA PRIVATE KEY-----\n"
"MIIEogIBAAKCAQEA2fHCV0sKLz+iS/DMpuP6GzUSBA5oohptor95maypRFdLW3GO\n"
"qHKTbfyNldF0aLMzZme/IDviFJpTUxSeel+Nl1TykQNpwAJspkOh/f5n0wghF4EI\n"
"IGQYAtmFXdimkBGFlNCkcsHBpatroz8Gk4A5AI8rrA2vQ9xxZoo8PpohApKKTS/S\n"
"+yrS/lLXiYmLjj8c2MtjAcKnULpQ8366xPragH8/qvAxWLgboycn+KiM6dA7axTY\n"
"pYGwH+BB0PD7Pu6nxNKm6fkU5xTD3keMv0rqRanuCAXbf9/gvpshcXJi4rJOUFQS\n"
"cx/VkAKtxlqNArSs14vRIa6AsfvffSMWXl7CkQIDAQABAoIBACDvmB75CCD7zNsN\n"
"GOBfs3xJGN/kDYvx0n+1z/qZTTs2tfyrvJNYPU9xyzPI436vFiVzqD6GzjOu3si/\n"
"ZPO5XPxxqfagPWr28tRthB2ORMJviBC1PzUl6zsDBFSaWRGMlFNY9IEjJtCSh0G0\n"
"VJoV1odvG3p01ve6aKjGWzRatoJcu3Sv0Qs1YBIkCCyGUmyHpWgumzvtcKmNSxsp\n"
"Q39c8qtkdKaeTLggPPElu1WGLfoiQEVlL8dSRcMJV+t7hxeOkaj5SavCkjsgvGOh\n"
"+sOY0CFIgiG77LLLre5Sa7kPMLAiAiASw/CkTZIrgx2XSLRt7BaP2NcY6mxOJSK2\n"
"AmW/G8ECgYEA9n3peniEaDrWKJsskPKvZIqhCQDmHVGVUjf5D22s5Yp8niK0iJGy\n"
"BCkLUEmUs7OsXsoXHZHnkPv9LFkm1skWPOPRVooOU2+dhNvRX47Ahpz058BVDKpd\n"
"oBdTNXSKKh3YpH5HhBrtttZgSuLc+uzsZ3lAg1LzfFwd1BDClQrrdSsCgYEA4lnx\n"
"amwmvvAL7PPHSy4eWgQIWbsoT4ClFNjFzq9iK1YwH2KFmiLqQGJ3VR5J74lDBjOq\n"
"4grdMgb10ywEY6gnlbH9HCJkZbaqVaSfkzKEFuM1+F4S3VeRH08ubzTe3wqshpz8\n"
"UrPLmc1AO8cZUuttRap/BAM1ZPnVUokJ79vjwTMCgYAlviPpB9kmaJPwRcpuK2rl\n"
"10BPM58IsDxQCZgOX7nQsALi5+eC3eRHNM2xroKhfYqQPRuLAJNXpO4gQ0pqKwJH\n"
"so6UVLFa30VcWZhWrHIYaqIpdk7PkKqjX87y6XRafJV2ootq1ADANVUrQk+Z8HtG\n"
"YM+bDDGSQjyjAyn4TmK9XwKBgAC36Fw+G6Ffgaou2+MRMmd2u9ANGAOolN2MEpSl\n"
"WdppW5294k6q7bNmnJpYjkOEXAIZB+HPxwPxobYZ3amzTuzas8LzGP55pfC/pQIC\n"
"lwi7j+BJ7CVnsb43sh4IPOzYS9Phi6t3hp3mWN3Xs+zfcmlWMTECEZlQcazhYnaw\n"
"a/ihAoGAdRrcsh6MRsHGrkNIPTrWPod3PFRUf5UznBDYA7EtqfwtUBM0zG4IJ2t1\n"
"ec1pjF14iAeXsH+FYRXisXp/aLSMglq5vMsa8B6y50M6W7dayEZJVUXImUVFBPqK\n"
"fTqejOrDOkrg1VJ2gaJVd5KYuRAtKVztkb/hZVyAZAaqJ/FmALE=\n"
"-----END RSA PRIVATE KEY-----";

unsigned char sign[256] = {0};

void SignBasicSafetyMessage_unsafe(EVP_MD_CTX *mdctx, BasicSafetyMessage *bsm, EVP_PKEY *private_key) {
    unsigned char signature[512+1];
    size_t signature_length = 512;

    /* Initialize the DigestSign operation for RSA-SHA256 */
     if(1 != EVP_DigestSignInit(mdctx, NULL, EVP_sha256(), NULL, private_key)) {
         /* Log Error */
         bsm->setSignatureLength(0);
         return;
     }

     /* Digest the fields of the bsm */
     Coord senderPos = bsm->getSenderPos();
     if(1 != EVP_DigestSignUpdate(mdctx, (void*)(&senderPos), sizeof(Coord))) {
         /* Log Error */
         bsm->setSignatureLength(0);
         return;
     }

     /* Obtain the signature */
     if(1 != EVP_DigestSignFinal(mdctx, signature, &signature_length)) {
         /* Log Error */
         bsm->setSignatureLength(0);
         return;
     }

     bsm->setSignatureArraySize(signature_length);
     bsm->setSignatureLength(signature_length);
     bsm->setSignature(0, signature[0]);
     bsm->setSig((const char *)signature);
}

void SignBasicSafetyMessage(BasicSafetyMessage *bsm, EVP_PKEY *private_key) {
    EVP_MD_CTX *mdctx = NULL;

    /* Create the Message Digest Context */
    if(!(mdctx = EVP_MD_CTX_create())) {
        /* Log Error */
        bsm->setSignatureLength(0);
        return;
    }

    SignBasicSafetyMessage_unsafe(mdctx, bsm, private_key);
    int check = bsm->getSignatureLength();
    if (check == 0) std::cout << "sigfail" << std::endl;
    if(mdctx) EVP_MD_CTX_destroy(mdctx);
}

int VerifyBasicSafetyMessage_unsafe(EVP_MD_CTX *mdctx, BasicSafetyMessage *bsm, EVP_PKEY *pkey) {
    if (!(pkey)) {
        std::cout << "Missing public key" << std::endl;
        return -2;
    }
    if (!mdctx) {
        std::cout << "Missing mdctx" << std::endl;
        return -2;
    }
    if (1 != EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, (pkey)) ) {
        // error
        std::cout << "EVP_DigestVerifyInit err: " << ERR_reason_error_string(ERR_get_error()) << std::endl;
        ERR_print_errors_fp(stderr);
        return -2;
    }

    int siglen = bsm->getSignatureLength();
    if (!siglen) {
        std::cout << "siglen is 0" << std::endl;
        return -2;
    }
    unsigned char sig = bsm->getSignature(0);
    const char *new_sig = bsm->getSig();
    if (sig) {
        Coord senderPos = bsm->getSenderPos();
        if (1 != EVP_DigestVerifyUpdate(mdctx, (void *)&senderPos, sizeof(Coord))) {
            // err
            std::cout << ERR_reason_error_string(ERR_get_error()) << std::endl;
            return -2;
        }
        if (1 == EVP_DigestVerifyFinal(mdctx, (const unsigned char *)new_sig, siglen)) {
            // success
            return 1;
        } else {
            // error
            return 0;
        }
    }
    // no sig to verify
    return -1;
}

void BaseWaveApplLayer::verifyBasicSafetyMessage(BasicSafetyMessage *bsm) {
    EVP_MD_CTX *mdctx = EVP_MD_CTX_create();

    int len = bsm->getPublic_key_length();
    if (!len) {
        std::cout << "pk len is 0" << std::endl;
        return;
    }

    const char *pkk = bsm->getPk();

    BIO* bo = BIO_new_mem_buf(pkk, len);
    if (!bo) {
        std::cout << "Err getting bio from bsm to use for signature verification" << std::endl;
        return;
    }

    EVP_PKEY *epk = NULL;
    PEM_read_bio_PUBKEY(bo, &epk, 0, NULL);

    if (epk == NULL) {
        std::cout << "No public key in this bsm to use for signature verification" << std::endl;
        ERR_print_errors_fp(stderr);
        //std::cout << ERR_reason_error_string(ERR_get_error()) << std::endl;
        return;
    }

    int ret = VerifyBasicSafetyMessage_unsafe(mdctx, bsm, epk);
    if (ret == 1) std::cout << "!$!$ sigver success" << std::endl;
    else if (ret == 0) std::cout << "sigver failure" << std::endl;
    else if (ret == -1) std::cout << "no sig to verify" << std::endl;
    else std::cout << "other err" << std::endl;

}

void BaseWaveApplLayer::initialize(int stage)
{
    BaseApplLayer::initialize(stage);

    if (stage==0) {

        //initialize pointers to other modules
        if (FindModule<TraCIMobility*>::findSubModule(getParentModule())) {
            mobility = TraCIMobilityAccess().get(getParentModule());
            traci = mobility->getCommandInterface();
            traciVehicle = mobility->getVehicleCommandInterface();
        }
        else {
            traci = NULL;
            mobility = NULL;
            traciVehicle = NULL;
        }

        annotations = AnnotationManagerAccess().getIfExists();
        ASSERT(annotations);

        mac = FindModule<WaveAppToMac1609_4Interface*>::findSubModule(
                getParentModule());
        assert(mac);

        myId = getParentModule()->getId();

        //read parameters
        headerLength = par("headerLength").longValue();
        sendBeacons = par("sendBeacons").boolValue();
        beaconLengthBits = par("beaconLengthBits").longValue();
        beaconUserPriority = par("beaconUserPriority").longValue();
        beaconInterval =  par("beaconInterval");

        dataLengthBits = par("dataLengthBits").longValue();
        dataOnSch = par("dataOnSch").boolValue();
        dataUserPriority = par("dataUserPriority").longValue();

        wsaInterval = par("wsaInterval").doubleValue();
        communicateWhileParked = par("communicateWhileParked").boolValue();
        currentOfferedServiceId = -1;

        isParked = false;


        findHost()->subscribe(mobilityStateChangedSignal, this);
        findHost()->subscribe(parkingStateChangedSignal, this);

        sendBeaconEvt = new cMessage("beacon evt", SEND_BEACON_EVT);
        sendWSAEvt = new cMessage("wsa evt", SEND_WSA_EVT);

        generatedBSMs = 0;
        generatedWSAs = 0;
        generatedWSMs = 0;
        receivedBSMs = 0;
        receivedWSAs = 0;
        receivedWSMs = 0;

        /* BEGIN OPENSSL INIT */
        EV << "Init openssl algorithms" << std::endl;
        OpenSSL_add_all_algorithms();

        EV << "Init openssl algorithms" << std::endl;
        BIO* bo = BIO_new( BIO_s_mem() );
        BIO_write( bo, rsa_test_key,strlen(rsa_test_key));

        EV << "Create private key " << std::endl;
        this->private_key = PEM_read_bio_PrivateKey( bo, 0, 0, 0 );

        BIO_free(bo);

        std::cout << "Create public key from private key: " << this->private_key << std::endl;
        if (this->private_key == 0) {
            std::cout << "EMD ERROR: No Private Key. Public key ignored" << std::endl;
        } else {
            std::cout << "EMD pub_key: " << this->public_key << " priv_key " << this->private_key << std::endl;
            this->public_key = EVP_PKEY_new();
            std::cout << "EMD pub_key: " << this->public_key << " priv_key " << this->private_key << std::endl;
            EVP_PKEY_assign_RSA(this->public_key, RSAPublicKey_dup(EVP_PKEY_get1_RSA(this->private_key)));
        }
        EV << "OpenSSL initiated." << std::endl;
        /* END OPENSSL INIT   */
    }
    else if (stage == 1) {

        // store MAC address for quick access
        //myId = mac->getMACAddress();
        myId = getParentModule()->getId(); // ellie arbitrary // getMACAddress();
        std::cout << "EMD in stage 1" << std::endl;
        //simulate asynchronous channel access

        if (dataOnSch == true && !mac->isChannelSwitchingActive()) {
            dataOnSch = false;
            std::cerr << "App wants to send data on SCH but MAC doesn't use any SCH. Sending all data on CCH" << std::endl;
        }
        simtime_t firstBeacon = simTime();

        if (par("avoidBeaconSynchronization").boolValue() == true) {

            simtime_t randomOffset = dblrand() * beaconInterval;
            firstBeacon = simTime() + randomOffset;

            if (mac->isChannelSwitchingActive() == true) {
                if ( beaconInterval.raw() % (mac->getSwitchingInterval().raw()*2)) {
                    std::cerr << "The beacon interval (" << beaconInterval << ") is smaller than or not a multiple of  one synchronization interval (" << 2*mac->getSwitchingInterval() << "). "
                            << "This means that beacons are generated during SCH intervals" << std::endl;
                }
                firstBeacon = computeAsynchronousSendingTime(beaconInterval, type_CCH);
            }

            if (sendBeacons) {
                scheduleAt(firstBeacon, sendBeaconEvt);
            }
        }
    }
}

simtime_t BaseWaveApplLayer::computeAsynchronousSendingTime(simtime_t interval, t_channel chan) {

    /*
     * avoid that periodic messages for one channel type are scheduled in the other channel interval
     * when alternate access is enabled in the MAC
     */

    simtime_t randomOffset = dblrand() * beaconInterval;
    simtime_t firstEvent;
    simtime_t switchingInterval = mac->getSwitchingInterval(); //usually 0.050s
    simtime_t nextCCH;

    /*
     * start event earliest in next CCH (or SCH) interval. For alignment, first find the next CCH interval
     * To find out next CCH, go back to start of current interval and add two or one intervals
     * depending on type of current interval
     */

    if (mac->isCurrentChannelCCH()) {
        nextCCH = simTime() - SimTime().setRaw(simTime().raw() % switchingInterval.raw()) + switchingInterval*2;
    }
    else {
        nextCCH = simTime() - SimTime().setRaw(simTime().raw() %switchingInterval.raw()) + switchingInterval;
    }

    firstEvent = nextCCH + randomOffset;

    //check if firstEvent lies within the correct interval and, if not, move to previous interval

    if (firstEvent.raw()  % (2*switchingInterval.raw()) > switchingInterval.raw()) {
        //firstEvent is within a sch interval
        if (chan == type_CCH) firstEvent -= switchingInterval;
    }
    else {
        //firstEvent is within a cch interval, so adjust for SCH messages
        if (chan == type_SCH) firstEvent += switchingInterval;
    }

    return firstEvent;
}

void BaseWaveApplLayer::populateWSM(WaveShortMessage* wsm, int rcvId, int serial) {

    wsm->setWsmVersion(1);
    wsm->setTimestamp(simTime());
    wsm->setSenderAddress(myId);
    wsm->setRecipientAddress(rcvId);
    wsm->setSerial(serial);
    wsm->setBitLength(headerLength);


    if (BasicSafetyMessage* bsm = dynamic_cast<BasicSafetyMessage*>(wsm) ) {
        bsm->setSenderPos(curPosition);
        bsm->setSenderPos(curPosition);
        bsm->setSenderSpeed(curSpeed);
        bsm->setSenderAcceleration(curAcceleration);
        bsm->setSenderDirection(curDirection);
        bsm->setSenderOrientation(curOrientation);
        bsm->setPsid(-1);
        bsm->setChannelNumber(Channels::CCH);
        bsm->addBitLength(beaconLengthBits);
        wsm->setUserPriority(beaconUserPriority);

        SignBasicSafetyMessage(bsm, this->private_key);
        BIO* bo = BIO_new( BIO_s_mem() );
        int ret = PEM_write_bio_PUBKEY(bo, this->public_key);
        if (!ret) {
            std::cout << "##### pem write bio pubkey broken" << std::endl;
        }

        BUF_MEM *bptr;
        BIO_get_mem_ptr(bo, &bptr);
        int length = bptr->length;
        unsigned char publicKeyPem[length];
        int data_amt = BIO_read(bo, publicKeyPem, length);
        if (!data_amt) {
            std::cout << "######## bio err no key" << std::endl;
        } else {
            ERR_print_errors_fp(stderr);
            bsm->setPublic_keyArraySize(data_amt);
            bsm->setPublic_key_length(data_amt);
            bsm->setPublic_key(0, publicKeyPem[0]);
            bsm->setPk((const char *)publicKeyPem);
        }
    }
    else if (WaveServiceAdvertisment* wsa = dynamic_cast<WaveServiceAdvertisment*>(wsm)) {
        wsa->setChannelNumber(Channels::CCH);
        wsa->setTargetChannel(currentServiceChannel);
        wsa->setPsid(currentOfferedServiceId);
        wsa->setServiceDescription(currentServiceDescription.c_str());
    }
    else {
        if (dataOnSch) wsm->setChannelNumber(Channels::SCH1); //will be rewritten at Mac1609_4 to actual Service Channel. This is just so no controlInfo is needed
        else wsm->setChannelNumber(Channels::CCH);
        wsm->addBitLength(dataLengthBits);
        wsm->setUserPriority(dataUserPriority);
    }
}

void BaseWaveApplLayer::receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj, cObject* details) {
    Enter_Method_Silent();
    if (signalID == mobilityStateChangedSignal) {
        handlePositionUpdate(obj);
    }
    else if (signalID == parkingStateChangedSignal) {
        handleParkingUpdate(obj);
    }
}

void BaseWaveApplLayer::handlePositionUpdate(cObject* obj) {
    ChannelMobilityPtrType const mobility = check_and_cast<ChannelMobilityPtrType>(obj);
    curPosition = mobility->getCurrentPosition();
    curSpeed = mobility->getCurrentSpeed();
    curDirection = mobility->getCurrentDirection();
    curOrientation = mobility->getCurrentOrientation();

    if (traciVehicle) {
        curAcceleration = traciVehicle->getAccel();
    } else {
        curAcceleration = 0.0;
    }

    if (traciVehicle && traciVehicle->getTypeId() == "CarWrecker") {
        EV << "CarWrecker got a position update. curSpeed is " << curSpeed << std::endl;
        traciVehicle->setSpeedMode(6);
        traciVehicle->setSpeed(traciVehicle->getMaxSpeed());
    }
}

void BaseWaveApplLayer::handleParkingUpdate(cObject* obj) {
    //this code should only run when used with TraCI
    isParked = mobility->getParkingState();
    if (communicateWhileParked == false) {
        if (isParked == true) {
            (FindModule<BaseConnectionManager*>::findGlobalModule())->unregisterNic(this->getParentModule()->getSubmodule("nic"));
        }
        else {
            Coord pos = mobility->getCurrentPosition();
            (FindModule<BaseConnectionManager*>::findGlobalModule())->registerNic(this->getParentModule()->getSubmodule("nic"), (ChannelAccess*) this->getParentModule()->getSubmodule("nic")->getSubmodule("phy80211p"), &pos);
        }
    }
}

void BaseWaveApplLayer::handleLowerMsg(cMessage* msg) {

    WaveShortMessage* wsm = dynamic_cast<WaveShortMessage*>(msg);
    ASSERT(wsm);

    if (BasicSafetyMessage* bsm = dynamic_cast<BasicSafetyMessage*>(wsm)) {
        receivedBSMs++;
        verifyBasicSafetyMessage(bsm);
        onBSM(bsm);
    }
    else if (WaveServiceAdvertisment* wsa = dynamic_cast<WaveServiceAdvertisment*>(wsm)) {
        receivedWSAs++;
        onWSA(wsa);
    }
    else {
        receivedWSMs++;
        onWSM(wsm);
    }

    delete(msg);
}

void BaseWaveApplLayer::handleSelfMsg(cMessage* msg) {
    switch (msg->getKind()) {
    case SEND_BEACON_EVT: {
        BasicSafetyMessage* bsm = new BasicSafetyMessage();
        populateWSM(bsm);
        sendDown(bsm);
        scheduleAt(simTime() + beaconInterval, sendBeaconEvt);
        break;
    }
    case SEND_WSA_EVT:   {
        WaveServiceAdvertisment* wsa = new WaveServiceAdvertisment();
        populateWSM(wsa);
        sendDown(wsa);
        scheduleAt(simTime() + wsaInterval, sendWSAEvt);
        break;
    }
    default: {
        if (msg)
            DBG_APP << "APP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
        break;
    }
    }
}

void BaseWaveApplLayer::finish() {
    recordScalar("generatedWSMs",generatedWSMs);
    recordScalar("receivedWSMs",receivedWSMs);

    recordScalar("generatedBSMs",generatedBSMs);
    recordScalar("receivedBSMs",receivedBSMs);

    recordScalar("generatedWSAs",generatedWSAs);
    recordScalar("receivedWSAs",receivedWSAs);
}

BaseWaveApplLayer::~BaseWaveApplLayer() {
    cancelAndDelete(sendBeaconEvt);
    cancelAndDelete(sendWSAEvt);
    findHost()->unsubscribe(mobilityStateChangedSignal, this);
}

void BaseWaveApplLayer::startService(Channels::ChannelNumber channel, int serviceId, std::string serviceDescription) {
    if (sendWSAEvt->isScheduled()) {
        error("Starting service although another service was already started");
    }

    mac->changeServiceChannel(channel);
    currentOfferedServiceId = serviceId;
    currentServiceChannel = channel;
    currentServiceDescription = serviceDescription;

    simtime_t wsaTime = computeAsynchronousSendingTime(wsaInterval, type_CCH);
    scheduleAt(wsaTime, sendWSAEvt);

}

void BaseWaveApplLayer::stopService() {
    cancelEvent(sendWSAEvt);
    currentOfferedServiceId = -1;
}

void BaseWaveApplLayer::sendDown(cMessage* msg) {
    checkAndTrackPacket(msg);
    BaseApplLayer::sendDown(msg);
}

void BaseWaveApplLayer::sendDelayedDown(cMessage* msg, simtime_t delay) {
    checkAndTrackPacket(msg);
    BaseApplLayer::sendDelayedDown(msg, delay);
}

void BaseWaveApplLayer::checkAndTrackPacket(cMessage* msg) {
    if (isParked && !communicateWhileParked) error("Attempted to transmit a message while parked, but this is forbidden by current configuration");

    if (dynamic_cast<BasicSafetyMessage*>(msg)) {
        DBG_APP << "sending down a BSM" << std::endl;
        generatedBSMs++;
    }
    else if (dynamic_cast<WaveServiceAdvertisment*>(msg)) {
        DBG_APP << "sending down a WSA" << std::endl;
        generatedWSAs++;
    }
    else if (dynamic_cast<WaveShortMessage*>(msg)) {
        DBG_APP << "sending down a wsm" << std::endl;
        generatedWSMs++;
    }
}
