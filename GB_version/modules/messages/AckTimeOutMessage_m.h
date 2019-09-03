//
// Generated file, do not edit! Created by nedtool 5.5 from veins/modules/messages/AckTimeOutMessage.msg.
//

#ifndef __ACKTIMEOUTMESSAGE_M_H
#define __ACKTIMEOUTMESSAGE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>veins/modules/messages/AckTimeOutMessage.msg:21</tt> by nedtool.
 * <pre>
 * message AckTimeOutMessage
 * {
 *     // The corresponding WSM's tree id
 *     unsigned long wsmId = -1;
 *     // Access category on which the AckTimer is set
 *     int ac = -1;
 * }
 * </pre>
 */
class AckTimeOutMessage : public ::omnetpp::cMessage
{
  protected:
    unsigned long wsmId;
    int ac;

  private:
    void copy(const AckTimeOutMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const AckTimeOutMessage&);

  public:
    AckTimeOutMessage(const char *name=nullptr, short kind=0);
    AckTimeOutMessage(const AckTimeOutMessage& other);
    virtual ~AckTimeOutMessage();
    AckTimeOutMessage& operator=(const AckTimeOutMessage& other);
    virtual AckTimeOutMessage *dup() const override {return new AckTimeOutMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual unsigned long getWsmId() const;
    virtual void setWsmId(unsigned long wsmId);
    virtual int getAc() const;
    virtual void setAc(int ac);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const AckTimeOutMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, AckTimeOutMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef __ACKTIMEOUTMESSAGE_M_H

