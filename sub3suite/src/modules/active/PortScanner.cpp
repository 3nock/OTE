/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "PortScanner.h"


/*
 * TODO:
 *      this is a very simplified port scanner which is connection based,
 *      modify to SYN scanner
 */
port::Scanner::Scanner(active::ScanArgs *args): AbstractScanner(nullptr),
    m_args(args),
    m_socket(new QTcpSocket(this))
{
}
port::Scanner::~Scanner(){
    delete m_socket;
}

void port::Scanner::lookup(){
    m_target = getTarget(m_args);

    while(m_target != nullptr)
    {
        s3s_struct::HOST host;
        host.host = m_target;

        foreach(const quint16 &port, m_args->ports)
        {
            m_socket->connectToHost(m_target, port);
            if(m_socket->waitForConnected(m_args->config->timeout))
            {
                /* close connection */
                m_socket->close();

                /* add port to list of open ports */
                host.ports.append(port);
            }
        }
        /* scan progress */
        m_args->progress++;
        emit scanProgress(m_args->progress);

        /* send results if open ports found */
        if(!host.ports.isEmpty())
            emit scanResult(host);

        /* next target */
        m_target = getTarget(m_args);
    }

    emit quitThread();
}

QString port::getTarget(active::ScanArgs *args){
    QMutexLocker(&args->mutex);

    if(!args->targets.isEmpty())
        return args->targets.dequeue();
    else
        return nullptr;
}
