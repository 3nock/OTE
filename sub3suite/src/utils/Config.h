/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.
*/


#ifndef SRC_UTILS_CONFIG_H
#define SRC_UTILS_CONFIG_H

#include <QMap>
#include <QString>

class Config {
public:
    enum DNS {
        A,
        AAAA,
        MX,
        NS,
        CNAME,
        TXT,
        ANY,
        SRV
    };

    QMap<QString,QString> recents;

    /**
     * @brief General configurations
     */
    struct {
        QString build_date;
        QString theme;
        QString s3s;
        bool isFirstRun;
        int font;
    }general;

    struct {
        QList<QPair<QString, QString>> subdomain;
        QList<QPair<QString, QString>> tld;
    }wordlist;

    struct {
        QMap<QString,QString> authors;
        QMap<QString,QString> donors;
        QMap<QString,QString> modules;
    }about;

    struct {
        int timeout;
        bool useTimeout;
        bool noDuplicates;
        bool autoSaveResults;
    }enums;

    struct {
        struct {
            int timeout;
            int maxPages;
            bool useTimeout;
            bool noDuplicates;
            bool autoSaveResults;
        }osint;
        struct {
            int timeout;
            int maxPages;
            bool useTimeout;
            bool noDuplicates;
            bool autoSaveResults;
        }raw;
        struct {
            DNS record;
            int threads;
            int timeout;
            bool useTimeout;
            bool wildcard;
            bool noDuplicates;
            bool autoSaveResults;
            QMap<QString,bool> nameservers;
            QString nameserver;
        }brute;
        struct {
            DNS record;
            int threads;
            int timeout;
            bool useTimeout;
            bool noDuplicates;
            bool autoSaveResults;
            QMap<QString,bool> nameservers;
            QString nameserver;
            struct{
                int ttl = 128;
                int bytes = 32;
            }ping;
        }host;
        struct {
            DNS record;
            int threads;
            int timeout;
            bool useTimeout;
            bool noDuplicates;
            bool autoSaveResults;
            QMap<QString,bool> nameservers;
            QString nameserver;
            struct{
                int ttl = 128;
                int bytes = 32;
            }ping;
        }ip;
        struct {
            int threads;
            int timeout;
            bool useTimeout;
            bool noDuplicates;
            bool autoSaveResults;
            QMap<QString,bool> nameservers;
            QString nameserver;
        }dns;
        struct {
            int threads;
            int timeout;
            bool useTimeout;
            bool noDuplicates;
            bool autoSaveResults;
        }ssl;
        struct {
            int threads;
            int timeout;
            bool getTitle;
            bool forceScheme;
            bool followRedirects;
            bool useTimeout;
            bool noDuplicates;
            bool autoSaveResults;
            QString scheme;
        }url;
    }tools;

    /**
     * @brief API Keys.
     */
    struct {
        QString C99;
        QString CertSpotter;
        QString Github;
        QString Hunter;
        QString IntelX;
        QString ProjectDiscovery;
        QString SecurityTrails;
        QString Shodan;
        QString Spyse;
        QString ThreatBook;
        QString VirusTotal;
        QString BinaryEdge;
        QString IpInfo;
        QString WhoIsXMLAPI;
        QString ZoomEye;
        QString IpApi;
        QString ViewDns;
        QString HackerTarget;
        QString WebResolver;
        QString Mnemonic;
        QString Robtex;
        QString Otx;
        QString IpRegistry;
        QString BuiltWith;
        QString Detectify;
        QString DNSlytics;
        QString Onyphe;
        QString FullHunt;
        QString SpamHaus;
        QString Debounce;
        QString HaveIBeenPawned;
        QString Host;
        QString JsonWhois;
        QString LeakIX;
        QString NetworksDB;
        QString Seon;
        QString SpyOnWeb;
        QString WhatCMS;
        QString Whoxy;
        QString EmailCrawlr;
        QString EmailFormat;
        QString EmailRep;
        QString Snov;
        QString IpData;
        QString IpGeoLocation;
        QString IpStack;
        QString AbuseIPDB;
        QString HybridAnalysis;
        QString IpQualityScore;
        QString LeakLookup;
        QString Wappalyzer;
        QString ZETAlytics;
        QString Ipfy;

        struct {
            QString user;
            QString pass;
        }fraudguard;

        struct {
            QString uid;
            QString key;
        }neutrinoapi;

        struct {
            QString username;
            QString key;
        }DomainTools;

        struct {
            QString user;
            QString pwd;
        }Circl;

        struct {
            QString key;
            QString secret;
        }RiskIQ;

        struct {
            QString id;
            QString secret;
        }Censys;

        struct {
            QString key;
            QString username;
        }PassiveTotal;
    } keys;

    void fromConfigFile();
    void toConfigFile();
};

extern Config gConfig;

#endif // SRC_UTILS_CONFIG_H
