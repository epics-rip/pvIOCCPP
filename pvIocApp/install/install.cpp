/* pvDatabase.cpp */
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

#include "support.h"
#include "pvDatabase.h"
#include "install.h"

namespace epics { namespace pvData { 

AfterStart::AfterStart()
{
    throw std::logic_error(String("Not Implemented"));
}

AfterStart::~AfterStart()
{
    throw std::logic_error(String("Not Implemented"));
}

std::auto_ptr<AfterStartNode> AfterStart::createNode(
    AfterStartRequester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}



Install & Install::getInstall()
{
    throw std::logic_error(String("Not Implemented"));
}

Install::Install()
{
    throw std::logic_error(String("Not Implemented"));
}

Install::~Install()
{
    throw std::logic_error(String("Not Implemented"));
}


bool Install::installStructures(String xmlFile,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

bool Install::installStructures(PVDatabase &pvDatabase,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

bool Install::installStructure(PVStructure &pvStructure,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

bool Install::installRecords(String xmlFile,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

bool Install::installRecords(PVDatabase &pvDatabase,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}

bool Install::installRecord(PVRecord &pvRecord,Requester &requester)
{
    throw std::logic_error(String("Not Implemented"));
}


}}