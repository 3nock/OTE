#include "LevelEnumerator.h"

LevelEnumerator::LevelEnumerator(ScanArguments_level *scanArguments, ScanResults_level *scanResults)
    : m_scanArguments(scanArguments), m_scanResults(scanResults)
{
    connect(m_dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
    connect(this, SIGNAL(performAnotherLookup()), this, SLOT(lookup()));
}
LevelEnumerator::~LevelEnumerator(){

}

void LevelEnumerator::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void LevelEnumerator::lookup(){

}

void LevelEnumerator::onLookupFinished(){

}

void LevelEnumerator::onStop(){
    quitThread();
}
