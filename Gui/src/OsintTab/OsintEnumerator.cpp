#include "OsintEnumerator.h"

/*****************************************************************
 *                      Enumerator Class
 ****************************************************************/

Enumerator::Enumerator(ScanArguments_Osint *_scanArguments, ScanResults_Osint *_scanResults){
    scanArguments = _scanArguments;
    scanResults = _scanResults;
}

// called from osint, connects to the QThread...
void Enumerator::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(worker()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

// the main function runned on the QThread...
void Enumerator::worker(){
    // initialize python intepreter...
    Py_Initialize();
    // setting path to osint module...
    QString path = "sys.path.append('"+QDir::currentPath()+"/osint/')";
    PyRun_SimpleString("import sys");
    PyRun_SimpleString(path.toUtf8());
    //...
    PyObject *pModule;
    PyObject *pFunction;
    PyObject *pArguments;
    PyObject *pReturnValue;
    //...
    QString firstEntry;
    Py_ssize_t listSize;
    // loop to perform the enumeration on every choosen Osint Engine...
    QString osintEngine;
    for(int i = 0; i != scanArguments->choosenOptions.count(); i++){
        osintEngine = scanArguments->choosenOptions[i];
        pModule = PyImport_ImportModule("main");
        pFunction = PyObject_GetAttrString(pModule, osintEngine.toUtf8());
        pArguments = PyTuple_Pack(1, PyUnicode_FromString(scanArguments->targetDomain));
        pReturnValue = PyObject_CallObject(pFunction, pArguments);
        // processing results...
        listSize = PyList_Size(pReturnValue);
        if(listSize > 0){
            firstEntry = PyUnicode_AsUTF8(PyList_GetItem(pReturnValue, 0));
            if(firstEntry.startsWith("[")){ // if an exception occured...
                emit scanLogs(firstEntry);
            }else{
                for(Py_ssize_t j = 0; j < listSize; j++){
                    scanResults->results_model->setItem(*scanResults->resultsCount, 0, new QStandardItem(PyUnicode_AsUTF8(PyList_GetItem(pReturnValue, j))));
                    *scanResults->resultsCount += 1;
                }
                scanResults->label_subdomainsCount->setNum(*scanResults->resultsCount);
                emit scanLogs("[*] Enumerated Subdomains By "+osintEngine+", SIZE: "+QString::number(listSize));
            }
        }else{
            emit scanLogs("[*] Enumerated Subdomains By "+osintEngine+", SIZE: 0");
        }
        // dereferencing the python objects...
        Py_DecRef(pModule);
        Py_DecRef(pReturnValue);
        Py_DecRef(pFunction);
        Py_DecRef(pArguments);
        if(stopEnumeration){
            break;
        }
    }
    // finalizing the python interpreter...
    Py_Finalize();
    // clearing the options...
    scanArguments->choosenOptions.clear();
    emit enumerationComplete();
    emit quitThread();
}

void Enumerator::onStopEnumeration(){
    stopEnumeration = true;
}
