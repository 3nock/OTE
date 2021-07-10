#include "IpEnumerator.h"

/*************************************************************************************************
                                     Ip_SUBDOMAINS ENUMERATOR
**************************************************************************************************/
IpEnumerator::IpEnumerator(ScanConfig *scanConfig, ScanArguments_Ip *scanArguments)
    : m_scanConfig(scanConfig),
      m_scanArguments(scanArguments),
      //...
      hostInfo(new QHostInfo)
{
    //...
    connect(this, SIGNAL(performAnotherLookup()), this, SLOT(lookup()));
}
IpEnumerator::~IpEnumerator(){
    delete hostInfo;
}

void IpEnumerator::enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

///
/// TODO: test the results if its is not simply an address using regular expression...
///
void IpEnumerator::lookupFinished(QHostInfo info){
    ///
    /// check the results of the lookup if no error occurred save the results
    /// if error occurred emit appropriate response...
    ///
    if(info.error() ==  QHostInfo::NoError)
    {
        if(!info.hostName().isNull() && !info.addresses().isEmpty()){
            emit scanResult(info.hostName(), info.addresses()[0].toString());
        }
    }
    ///
    /// scan progress...
    ///
    m_scanArguments->progress++;
    emit progress(m_scanArguments->progress);
    //...
    emit performAnotherLookup();
}

void IpEnumerator::lookup(){
    m_currentTargetToEnumerate = m_scanArguments->currentTargetToEnumerate;
    m_scanArguments->currentTargetToEnumerate++;
    if(m_currentTargetToEnumerate < m_scanArguments->targetList->count())
    {
        hostInfo->lookupHost(m_scanArguments->targetList->item(m_currentTargetToEnumerate)->text(), this, SLOT(lookupFinished(QHostInfo)));
    }
    else
    {
        ///
        /// at the end of the targetList, signal the thread to Quit...
        ///
        emit quitThread();
    }
}

void IpEnumerator::onStop(){
    emit quitThread();
}
