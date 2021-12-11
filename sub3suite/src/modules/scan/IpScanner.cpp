 #include "IpScanner.h"

ipEngine::Scanner::Scanner(ScanConfig *scanConfig, ipEngine::ScanArguments *scanArguments)
    : m_scanConfig(scanConfig),
      m_scanArguments(scanArguments),
      //...
      hostInfo(new QHostInfo)
{
    //...
    connect(this, SIGNAL(anotherLookup()), this, SLOT(lookup()));
}
ipEngine::Scanner::~Scanner(){
    delete hostInfo;
}

///
/// TODO: test the results if its is not simply an address using regular expression...
///
void ipEngine::Scanner::lookupFinished(QHostInfo info){
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
    emit scanProgress(m_scanArguments->progress);
    //...
    emit anotherLookup();
}

void ipEngine::Scanner::lookup(){
    m_currentTargetToEnumerate = m_scanArguments->currentTargetToEnumerate;
    m_scanArguments->currentTargetToEnumerate++;
    if(m_currentTargetToEnumerate < m_scanArguments->targetList.count())
    {
        hostInfo->lookupHost(m_scanArguments->targetList.at(m_currentTargetToEnumerate), this, SLOT(lookupFinished(QHostInfo)));
    }
    else
    {
        ///
        /// at the end of the targetList, signal the thread to Quit...
        ///
        emit quitThread();
    }
}

/*
void IpEnumerator::lookup(){
    socket->connectToHost("whois.cymru.com", 43);
    if(socket->waitForConnected(3000)){
        socket->write("8.8.8.8");
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);
        scanLog(QString::number(socket->bytesAvailable()));
        socket->close();
    }
    quitThread();
}
*/
