#include "OsintEnumerator.h"

/*****************************************************************
 *                      Enumerator Class
 ****************************************************************/

Enumerator::Enumerator(char *_domainName, QStringList _choosenEngines, QListWidget *_listWidget_subdomains, QLabel *_label_subdomainsCount, int *_subdomainsCount){
    domainName = _domainName;
    choosenEngines = _choosenEngines;
    listWidget_subdomains = _listWidget_subdomains;
    label_subdomainsCount = _label_subdomainsCount;
    subdomainsCount = _subdomainsCount;
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
    for(int i = 0; i != choosenEngines.count(); i++){
        pModule = PyImport_ImportModule("main");
        pFunction = PyObject_GetAttrString(pModule, choosenEngines[i].toUtf8());
        pArguments = PyTuple_Pack(1, PyUnicode_FromString(domainName));
        pReturnValue = PyObject_CallObject(pFunction, pArguments);
        // processing results...
        listSize = PyList_Size(pReturnValue);
        if(listSize > 0){
            firstEntry = PyUnicode_AsUTF8(PyList_GetItem(pReturnValue, 0));
            if(firstEntry.startsWith("[")){ // if an exception occured...
                emit scanLogs(firstEntry);
            }else{
                for(Py_ssize_t j = 0; j < listSize; j++){
                    listWidget_subdomains->addItem(PyUnicode_AsUTF8(PyList_GetItem(pReturnValue, j)));
                    *subdomainsCount += 1;
                }
                label_subdomainsCount->setNum(*subdomainsCount);
                emit scanLogs("[*] Enumerated Subdomains By "+choosenEngines[i]+", SIZE: "+QString::number(listSize));
            }
        }else{
            emit scanLogs("[*] Enumerated Subdomains By "+choosenEngines[i]+", SIZE: 0");
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
    emit enumerationComplete();
    emit quitThread();
}

void Enumerator::onStopEnumeration(){
    stopEnumeration = true;
}
