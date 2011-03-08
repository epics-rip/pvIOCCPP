/* v3ChannelProvider.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#include <string>
#include <stdexcept>
#include <memory>

#include <pvIntrospect.h>
#include <pvData.h>
#include <noDefaultMethods.h>
#include <lock.h>
#include <epicsExit.h>
#include <dbAccess.h>

#include "support.h"
#include "pvDatabase.h"
#include "v3Channel.h"

namespace epics { namespace pvIOC { 

using namespace epics::pvData;
using namespace epics::pvAccess;

static Status okStatus;
static Status notFoundStatus(Status::STATUSTYPE_ERROR,String("pv not found"));

static V3ChannelProvider *channelProvider = 0;

static void myDestroy(void*)
{
    static Mutex mutex;
    Lock xx(mutex);
    if(channelProvider==0) return;
    channelProvider->destroy();
    channelProvider = 0;
}

V3ChannelProvider &V3ChannelProvider::getChannelProvider()
{
    static Mutex mutex;
    Lock xx(mutex);
    if(channelProvider==0){
        channelProvider = new V3ChannelProvider();
        epicsAtExit(&myDestroy,0);
    }
    return *channelProvider;

}

V3ChannelProvider::V3ChannelProvider()
{
}

V3ChannelProvider::~V3ChannelProvider()
{
}

void V3ChannelProvider::destroy()
{
    delete channelProvider;
}

String V3ChannelProvider::getProviderName()
{
    return String("v3Channel");
}


ChannelFind *V3ChannelProvider::channelFind(
    String channelName,
    ChannelFindRequester *channelFindRequester)
{
    struct dbAddr dbaddr;
    long result = dbNameToAddr(channelName.c_str(),&dbaddr);
    if(result!=0) {
        channelFindRequester->channelFindResult(notFoundStatus,0,false);
        return 0;
    }
printf("after dbNameToAddr pfield %p\n",dbaddr.pfield);
    channelFindRequester->channelFindResult(okStatus,0,true);
    return 0;
}

Channel *V3ChannelProvider::createChannel(
    String channelName,
    ChannelRequester *channelRequester,
    short priority)
{
    return createChannel(channelName,channelRequester,priority,0);
}

Channel *V3ChannelProvider::createChannel(
    String channelName,
    ChannelRequester *channelRequester,
    short priority,
    String address)
{
    struct dbAddr dbaddr;
    long result = dbNameToAddr(channelName.c_str(),&dbaddr);
    if(result!=0) {
        channelRequester->channelCreated(notFoundStatus,0);
        return 0;
    }
printf("after dbNameToAddr pfield %p\n",dbaddr.pfield);
    std::auto_ptr<DbAddr> addr(new DbAddr());
printf("after dbNameToAddr pfield %p\n",addr.get()->pfield);
    memcpy(addr.get(),&dbaddr,sizeof(dbaddr));
printf("after dbNameToAddr pfield %p\n",addr.get()->pfield);
    V3Channel *v3Channel = new V3Channel(*channelRequester,channelName,addr);
    channelRequester->channelCreated(okStatus,v3Channel);
    return v3Channel;
}

}}
