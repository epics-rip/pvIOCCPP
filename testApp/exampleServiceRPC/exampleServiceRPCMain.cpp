/*exampleServiceRPCMain.cpp */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <iostream>

#include <pv/CDRMonitor.h>
#include <epicsExit.h>
#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>

#include <epicsExport.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/serverContext.h>

#include <pv/pvDatabase.h>
#include <pv/pvServiceProvider.h>
#include <exampleServiceRPC.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvIOC;

void example()
{
    PVServiceChannelCTX::shared_pointer myCTX
        = PVServiceChannelCTX::shared_pointer(new PVServiceChannelCTX());
    ExampleServiceRPC::shared_pointer example
        = ExampleServiceRPC::shared_pointer(new ExampleServiceRPC());
    ServiceChannelRPC::shared_pointer serviceChannelRPC
        = ServiceChannelRPC::shared_pointer(
            new ServiceChannelRPC("serviceRPC",example));
    cout << "serviceRPC\n";
    string str;
    while(true) {
        cout << "Type exit to stop: \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;
        
    }
}

int main(int argc,char *argv[])
{
    example();
printf("calling epicsExitCallAtExits\n");
    epicsExitCallAtExits();
printf("calling epicsThreadSleep\n");
    epicsThreadSleep(1.0);
printf("calling CDRMonitor::get\n");
    CDRMonitor::get().show(stdout,true);
    return 0;
}

