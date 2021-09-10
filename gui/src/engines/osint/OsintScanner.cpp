#include "OsintScanner.h"
//...
#undef slots
#include "Python.h"
#define slots
//...
#include <QDir>

osint::Scanner::Scanner(osint::ScanArguments *scanArguments, osint::ScanResults *scanResults)
    : m_scanArguments(scanArguments), m_scanResults(scanResults)
{
    // nothing yet...
}
osint::Scanner::~Scanner(){
    // nothing yet...
}

///
/// called from osint, connects to the QThread...
///
void osint::Scanner::startScan(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(worker()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

///
///  the main function runned on the QThread...
///
void osint::Scanner::lookup(){
    ///
    /// initialize python intepreter Enviroment...
    ///
    Py_Initialize();
    ///
    /// setting path to osint module...
    ///
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
    ///
    /// loop to perform the enumeration on every choosen Osint Engine...
    ///
    QString osintEngine;
    for(int i = 0; i != m_scanArguments->choosenOptions.count(); i++){
        osintEngine = m_scanArguments->choosenOptions[i];
        pModule = PyImport_ImportModule("main");
        pFunction = PyObject_GetAttrString(pModule, osintEngine.toUtf8());
        pArguments = PyTuple_Pack(1, PyUnicode_FromString(m_scanArguments->targetDomain));
        pReturnValue = PyObject_CallObject(pFunction, pArguments);
        ///
        /// processing results...
        ///
        listSize = PyList_Size(pReturnValue);
        if(listSize > 0){
            firstEntry = PyUnicode_AsUTF8(PyList_GetItem(pReturnValue, 0));
            ///
            /// if an exception occured while running python code...
            ///
            if(firstEntry.startsWith("[")){
                emit scanLog(firstEntry);
            }
            ///
            /// if no Exception occurred while running python code...
            ///
            else{
                for(Py_ssize_t j = 0; j < listSize; j++){
                    m_scanResults->results_model->setItem(*m_scanResults->resultsCount, 0, new QStandardItem(PyUnicode_AsUTF8(PyList_GetItem(pReturnValue, j))));
                    *m_scanResults->resultsCount += 1;
                }
                m_scanResults->label_subdomainsCount->setNum(*m_scanResults->resultsCount);
                emit scanLog("[*] Enumerated Subdomains By "+osintEngine+", SIZE: "+QString::number(listSize));
            }
        }
        else{
            emit scanLog("[*] Enumerated Subdomains By "+osintEngine+", SIZE: 0");
        }
        ///
        /// dereferencing the python objects...
        ///
        Py_DecRef(pModule);
        Py_DecRef(pReturnValue);
        Py_DecRef(pFunction);
        Py_DecRef(pArguments);
        //...
        if(m_stopEnumeration){
            break;
        }
    }
    ///
    /// finalizing the python interpreter...
    ///
    Py_Finalize();
    ///
    /// clearing the options...
    ///
    m_scanArguments->choosenOptions.clear();
    emit quitThread();
}

void osint::Scanner::stopScan(){
    m_stopEnumeration = true;
}
