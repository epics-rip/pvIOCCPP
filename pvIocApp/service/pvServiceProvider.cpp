/* pvServiceProvider.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#include <string>
#include <stdexcept>
#include <memory>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/noDefaultMethods.h>
#include <pv/lock.h>
#include <epicsExit.h>
#include <dbAccess.h>

#include <pv/pvServiceProvider.h>

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

static ChannelBaseProvider::shared_pointer pvServiceProvider;

static void unregister(void *)
{
printf("PVServiceProvider::unregister\n");
     unregisterChannelProvider(pvServiceProvider);
     pvServiceProvider.reset();
}

static void initStatic(void *)
{
    epicsAtExit(&unregister,0);
}

static epicsThreadOnceId initOnce = EPICS_THREAD_ONCE_INIT;

class ServicePVTopBase
{
public:
    ServicePVTopBase(ServicePVTop::shared_pointer servicePVTop)
    : servicePVTop(servicePVTop){}
    ServicePVTop::shared_pointer servicePVTop;
    std::set<epics::pvAccess::Channel::shared_pointer> channelList;
};

ChannelBaseProvider::shared_pointer PVServiceProvider::getPVServiceProvider()
{
    static Mutex mutex;
    Lock xx(mutex);

    if(pvServiceProvider.get()==0) {
        epicsThreadOnce(&initOnce, &initStatic, 0);
        pvServiceProvider = ChannelBaseProvider::shared_pointer(
            new PVServiceProvider());
        pvServiceProvider->init();
    }
    return pvServiceProvider;
}

PVServiceProvider::PVServiceProvider()
: ChannelBaseProvider("pvService")
{
}

PVServiceProvider::~PVServiceProvider()
{
printf("PVServiceProvider::~PVServiceProvider\n");
destroy();
pvServiceProvider.reset();
}

void PVServiceProvider::destroy()
{
    Lock xx(mutex);
    ServicePVTopBaseList::iterator iter;
    for(iter = topList.begin(); iter!=topList.end(); ++iter) {
        ServicePVTopBasePtr top = *iter;
        top->servicePVTop->destroy();
    }
    topList.clear();
}

ChannelFind::shared_pointer PVServiceProvider::channelFind(
    String name,
    ChannelFindRequester::shared_pointer const &channelFindRequester)
{
    ServicePVTopBaseList::iterator iter;
    bool result = false;
    for(iter = topList.begin(); iter!=topList.end(); ++iter) {
        ServicePVTopBasePtr topBase = *iter;
        ServicePVTopPtr top = topBase->servicePVTop;
        if(top->getName().compare(name)==0) {
            result = true;
            break;
        }
    }
    channelFound(result,channelFindRequester);
    return ChannelFind::shared_pointer();
}

Channel::shared_pointer PVServiceProvider::createChannel(
    String channelName,ChannelRequester::shared_pointer  const &channelRequester,
    short priority,
    String address)
{
    ServicePVTopBaseList::iterator iter;
    for(iter = topList.begin(); iter!=topList.end(); ++iter) {
        ServicePVTopBasePtr topBase = *iter;
        ServicePVTopPtr top = topBase->servicePVTop;
        if(top->getName().compare(channelName)==0) {
            ChannelProvider::shared_pointer xxx = getPtrSelf();
            ChannelBase::shared_pointer channel =
                 top->createChannel(channelRequester,xxx);
            channelCreated(channel);
            topBase->channelList.insert(channel);
            return channel;
        }
    }
//    TopListNode *node = topList.getHead();
//    while(node!=0) {
//        ServicePVTopBase &pvTopBase = node->getObject();
//        ServicePVTop::shared_pointer pvTop = pvTopBase.servicePVTop;
//        if((pvTop->getName().compare(channelName)==0)) {
//            ChannelBase::shared_pointer channel =
//                 pvTop->createChannel(channelRequester,
//                      static_cast<ChannelProvider::shared_pointer>(
//                          getPtrSelf()));
//            channelCreated(channel);
//            return channel;
//        }
//        node = topList.getNext(*node);
//    }
    ChannelBaseProvider::channelNotCreated(channelRequester);
    return Channel::shared_pointer();
}

void PVServiceProvider::addRecord(
    ServicePVTop::shared_pointer servicePVTop)
{
//printf("PVServiceProvider::addRecord\n");
    Lock xx(mutex);
    ServicePVTopBasePtr topBase(new ServicePVTopBase(servicePVTop));
    topList.insert(topBase);
}

void PVServiceProvider::removeRecord(
    ServicePVTop::shared_pointer servicePVTop)
{
    Lock xx(mutex);
    // TODO maybe static_pointer_cast is OK here...
    topList.erase(std::tr1::dynamic_pointer_cast<ServicePVTopBase>(servicePVTop));
}

}}
