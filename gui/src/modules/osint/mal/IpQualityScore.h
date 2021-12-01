#ifndef IPQUALITYSCORE_H
#define IPQUALITYSCORE_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
    struct IpQualityScore{
        QString name = "IpQualityScore";
        QString url = "https://www.ipqualityscore.com/";
        QString url_apiDoc = "https://www.ipqualityscore.com/documentation/overview";
        QString summary = "IPQualityScore offers a variety of different risk analysis APIs designed to Proactively Prevent Fraud™ for your company, website, or app. "
                          "From small and medium sized website owners to enterprise companies and the internet's most popular sites, IPQS has the right solutions to solve your challenges with online fraud prevention.";

        QMap<QString, QStringList> flags = {{"Proxy & VPN Detection",
                                             {PLACEHOLDERTEXT_IP, "IPQualityScore's Proxy Detection API allows you to Proactively Prevent Fraud™ via a simple API that provides over 25 data points for risk analysis, geo location, and IP intelligence. "
                                                                  "This endpoint allows on-demand lookups using our IP reputation API to accurately identify threats, bots, and compromised connections."}},
                                            {"Email Validation",
                                             {PLACEHOLDERTEXT_EMAIL, "IPQualityScore's Email Validation API estimates deliverability rates by detecting invalid mailboxes as well as disposable and fraudulent email addresses, spam traps, and "
                                                                    "honeypots via our simple to use Email Reputation API. Verify email addresses and identify abusive emails in real-time with a low latency API request."}},
                                            {"Phone Number Validation",
                                             {PLACEHOLDERTEXT_PHONE, "IPQualityScore's Phone Number Validation API enables quick user verification and chargeback defense on a global scale by analyzing phone numbers to verify their risk score, "
                                                                    "country of origin, carrier, validity, and line connection status while also detecting fraudulent activity in real time. The IPQS phone number lookup API can enrich landline and cellular numbers in over 150 countries."}},
                                            {"Malicious URL Scanner",
                                             {PLACEHOLDERTEXT_URL, "IPQualityScore's Malicious URL Scanner API scans links in real-time to detect suspicious URLs. Accurately identify phishing links, malware URLs and viruses, parked domains, and suspicious URLs with real-time risk scores."}}};

        QMap<int, QList<int>> input_output = {};
    };
}

class IpQualityScore: public AbstractOsintModule{

    public:
        IpQualityScore(ScanArgs *args);
        ~IpQualityScore() override;

    public slots:
        void start() override;

    private:
        QString m_key = nullptr;
};

#endif // IPQUALITYSCORE_H
