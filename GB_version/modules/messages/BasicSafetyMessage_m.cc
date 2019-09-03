//
// Generated file, do not edit! Created by nedtool 5.5 from veins/modules/messages/BasicSafetyMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "BasicSafetyMessage_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(BasicSafetyMessage)

BasicSafetyMessage::BasicSafetyMessage(const char *name, short kind) : ::WaveShortMessage(name,kind)
{
    signature_arraysize = 0;
    this->signature = 0;
    this->signatureLength = 0;
}

BasicSafetyMessage::BasicSafetyMessage(const BasicSafetyMessage& other) : ::WaveShortMessage(other)
{
    signature_arraysize = 0;
    this->signature = 0;
    copy(other);
}

BasicSafetyMessage::~BasicSafetyMessage()
{
    delete [] this->signature;
}

BasicSafetyMessage& BasicSafetyMessage::operator=(const BasicSafetyMessage& other)
{
    if (this==&other) return *this;
    ::WaveShortMessage::operator=(other);
    copy(other);
    return *this;
}

void BasicSafetyMessage::copy(const BasicSafetyMessage& other)
{
    this->senderPos = other.senderPos;
    this->senderSpeed = other.senderSpeed;
    delete [] this->signature;
    this->signature = (other.signature_arraysize==0) ? nullptr : new char[other.signature_arraysize];
    signature_arraysize = other.signature_arraysize;
    for (unsigned int i=0; i<signature_arraysize; i++)
        this->signature[i] = other.signature[i];
    this->signatureLength = other.signatureLength;
    this->public_key = other.public_key;
}

void BasicSafetyMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::WaveShortMessage::parsimPack(b);
    doParsimPacking(b,this->senderPos);
    doParsimPacking(b,this->senderSpeed);
    b->pack(signature_arraysize);
    doParsimArrayPacking(b,this->signature,signature_arraysize);
    doParsimPacking(b,this->signatureLength);
    doParsimPacking(b,this->public_key);
}

void BasicSafetyMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::WaveShortMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->senderPos);
    doParsimUnpacking(b,this->senderSpeed);
    delete [] this->signature;
    b->unpack(signature_arraysize);
    if (signature_arraysize==0) {
        this->signature = 0;
    } else {
        this->signature = new char[signature_arraysize];
        doParsimArrayUnpacking(b,this->signature,signature_arraysize);
    }
    doParsimUnpacking(b,this->signatureLength);
    doParsimUnpacking(b,this->public_key);
}

Coord& BasicSafetyMessage::getSenderPos()
{
    return this->senderPos;
}

void BasicSafetyMessage::setSenderPos(const Coord& senderPos)
{
    this->senderPos = senderPos;
}

Coord& BasicSafetyMessage::getSenderSpeed()
{
    return this->senderSpeed;
}

void BasicSafetyMessage::setSenderSpeed(const Coord& senderSpeed)
{
    this->senderSpeed = senderSpeed;
}

void BasicSafetyMessage::setSignatureArraySize(unsigned int size)
{
/*
    unsigned char *signature2 = (size==0) ? nullptr : new unsigned char[size];
    unsigned int sz = signature_arraysize < size ? signature_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        signature2[i] = this->signature[i];
    for (unsigned int i=sz; i<size; i++)
        signature2[i] = 0;
    signature_arraysize = size;
    delete [] this->signature;
    this->signature = signature2;
*/
}

void BasicSafetyMessage::setSignature(char* sig)
{
	this->signature = sig;
	this->signatureLength = strlen(sig);
}

char* BasicSafetyMessage::getSignature()
{
	return this->signature;
}

unsigned int BasicSafetyMessage::getSignatureArraySize() const
{
    return signature_arraysize;
}

unsigned char BasicSafetyMessage::getSignature(unsigned int k) const
{
    if (k>=signature_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", signature_arraysize, k);
    return this->signature[k];
}

void BasicSafetyMessage::setSignature(unsigned int k, unsigned char signature)
{
    if (k>=signature_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", signature_arraysize, k);
    this->signature[k] = signature;
}

unsigned int BasicSafetyMessage::getSignatureLength() const
{
    return this->signatureLength;
}

void BasicSafetyMessage::setSignatureLength(unsigned int signatureLength)
{
    this->signatureLength = signatureLength;
}

const char * BasicSafetyMessage::getPublic_key() const
{
    return this->public_key.c_str();
}

void BasicSafetyMessage::setPublic_key(const char * public_key)
{
    this->public_key = public_key;
}

class BasicSafetyMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    BasicSafetyMessageDescriptor();
    virtual ~BasicSafetyMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(BasicSafetyMessageDescriptor)

BasicSafetyMessageDescriptor::BasicSafetyMessageDescriptor() : omnetpp::cClassDescriptor("BasicSafetyMessage", "WaveShortMessage")
{
    propertynames = nullptr;
}

BasicSafetyMessageDescriptor::~BasicSafetyMessageDescriptor()
{
    delete[] propertynames;
}

bool BasicSafetyMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BasicSafetyMessage *>(obj)!=nullptr;
}

const char **BasicSafetyMessageDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *BasicSafetyMessageDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int BasicSafetyMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int BasicSafetyMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *BasicSafetyMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "senderPos",
        "senderSpeed",
        "signature",
        "signatureLength",
        "public_key",
    };
    return (field>=0 && field<5) ? fieldNames[field] : nullptr;
}

int BasicSafetyMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderPos")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderSpeed")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "signature")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "signatureLength")==0) return base+3;
    if (fieldName[0]=='p' && strcmp(fieldName, "public_key")==0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *BasicSafetyMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "Coord",
        "Coord",
        "unsigned char",
        "unsigned int",
        "string",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : nullptr;
}

const char **BasicSafetyMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *BasicSafetyMessageDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int BasicSafetyMessageDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    BasicSafetyMessage *pp = (BasicSafetyMessage *)object; (void)pp;
    switch (field) {
        case 2: return pp->getSignatureArraySize();
        default: return 0;
    }
}

const char *BasicSafetyMessageDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BasicSafetyMessage *pp = (BasicSafetyMessage *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BasicSafetyMessageDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    BasicSafetyMessage *pp = (BasicSafetyMessage *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getSenderPos(); return out.str();}
        case 1: {std::stringstream out; out << pp->getSenderSpeed(); return out.str();}
        case 2: return ulong2string(pp->getSignature(i));
        case 3: return ulong2string(pp->getSignatureLength());
        case 4: return oppstring2string(pp->getPublic_key());
        default: return "";
    }
}

bool BasicSafetyMessageDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    BasicSafetyMessage *pp = (BasicSafetyMessage *)object; (void)pp;
    switch (field) {
        case 2: pp->setSignature(i,string2ulong(value)); return true;
        case 3: pp->setSignatureLength(string2ulong(value)); return true;
        case 4: pp->setPublic_key((value)); return true;
        default: return false;
    }
}

const char *BasicSafetyMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(Coord));
        case 1: return omnetpp::opp_typename(typeid(Coord));
        default: return nullptr;
    };
}

void *BasicSafetyMessageDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    BasicSafetyMessage *pp = (BasicSafetyMessage *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getSenderPos()); break;
        case 1: return (void *)(&pp->getSenderSpeed()); break;
        default: return nullptr;
    }
}


