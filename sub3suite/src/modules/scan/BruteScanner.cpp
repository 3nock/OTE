#include "BruteScanner.h"


brute::Scanner::Scanner(brute::ScanArgs *args):
    m_args(args)
{
    m_dns = new QDnsLookup(this);
    qDebug() << "QDnsLookup object created...";

    m_dns->setNameserver(QHostAddress("1.1.1.1"));
    qDebug() << "Nameserver added...";

    m_dns->setType(QDnsLookup::A);
    qDebug() << "Type added...";

    connect(m_dns, SIGNAL(finished()), this, SLOT(lookupFinished()));
    qDebug() << "Connected finish lookup";

    /* get the first target */
    m_args->currentTarget = m_args->targets.dequeue();
    qDebug() << "current target added";

    m_args->currentWordlist = 0;
    qDebug() << "wordlist set to zero";
}
brute::Scanner::~Scanner(){
    delete m_dns;
}

void brute::Scanner::lookupFinished(){
    qDebug() << "lookup finished";

    switch(m_dns->error()){
    case QDnsLookup::NotFoundError:
        break;

    case QDnsLookup::NoError:
        emit result(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString());
        break;

    default:
        //emit errorLog(m_dns->errorString()+" HOSTNAME: "+m_dns->name()+"  NAMESERVER: "+m_dns->nameserver().toString());
        break;
    }

    /* scan progress */
    m_args->progress++;

    qDebug() << "next lookup";

    /* send results and continue scan */
    emit scanProgress(m_args->progress);
    emit next();
}

void brute::Scanner::lookupSubdomain(){
    try {
        qDebug() << "lookupSubdomain...";

        brute::ReturnVal retVal = brute::lookupSubdomain(m_dns, m_args);

        if(retVal.lookup){
            qDebug() << "lookup...";
            m_dns->lookup();
        }
        if(retVal.next){
            qDebug() << "Next...";
            emit next();
        }
        if(retVal.quit){
            qDebug() << "quitThread...";
            emit quitThread();
        }
    }
    catch (std::exception &e) {
        qDebug() << e.what();
    }
    catch(...){
        qDebug() << "Crashed";
    }
}

void brute::Scanner::lookupTLD(){
    brute::ReturnVal retVal = brute::lookupTLD(m_dns, m_args);

    if(retVal.lookup)
        m_dns->lookup();
    if(retVal.next)
        emit next();
    if(retVal.quit)
        emit quitThread();
}

brute::ReturnVal brute::lookupSubdomain(QDnsLookup *dns, brute::ScanArgs *args){
    brute::ReturnVal retVal;

    /* lock */
    QMutex mutex;
    mutex.lock();

    if(args->currentWordlist < args->wordlist.length())
    {
        /* append to target then set the name */
        dns->setName(args->wordlist.at(args->currentWordlist)+"."+args->currentTarget);

        /* lookup */
        retVal.lookup = true;

        /* next wordlist */
        args->currentWordlist++;

        qDebug() << "appending new wordlist";
    }
    /* Reached end of the wordlist */
    else
    {
         /* next target */
        if(args->targets.isEmpty()){
            retVal.quit = true;

            qDebug() << "target empty so quit";
        }
        else{
            args->currentWordlist = 0;
            args->currentTarget = args->targets.dequeue();
            retVal.next = true;

            qDebug() << "next target and wordlist = 0";
        }
    }

    /* unlock */
    mutex.unlock();
    return retVal;
}

brute::ReturnVal brute::lookupTLD(QDnsLookup *dns, brute::ScanArgs *args){
    brute::ReturnVal retVal;

    /* lock */
    QMutex mutex;
    mutex.lock();

    if(args->currentWordlist < args->wordlist.length())
    {
        /* append to target then set the name */
        dns->setName(args->currentTarget+"."+args->wordlist.at(args->currentWordlist));

        /* lookup */
        retVal.lookup = true;

        /* next wordlist */
        args->currentWordlist++;
    }
    /* Reached end of the wordlist */
    else
    {
         /* next target */
        if(args->targets.isEmpty())
            retVal.quit = true;
        else{
            args->currentWordlist = 0;
            args->currentTarget = args->targets.dequeue();
            retVal.next = true;
        }
    }

    /* unlock */
    mutex.unlock();
    return retVal;
}
