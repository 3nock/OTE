#ifndef HYBRIDANALYSIS_H
#define HYBRIDANALYSIS_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct HybridAnalysis{
    QString name = "HybridAnalysis";
    QString url = "https://www.hybrid-analysis.com/";
    QString url_apiDoc = "https://www.hybrid-analysis.com/docs/api/v2";
    QString summary = "Detects and analyzes unknown threats using a unique Hybrid Analysis technology.";

    QMap<QString, QStringList> flags = {{"Url Quick-Scan",
                                         {PLACEHOLDERTEXT_URL, "Analyses Url"}}};

    QMap<int, QList<int>> input_output = {};
};
}

class HybridAnalysis: public AbstractOsintModule{

    public:
        HybridAnalysis(ScanArgs args);
        ~HybridAnalysis() override;

    public slots:
        void start() override;

    private:
        QString m_key;
};

#endif // HYBRIDANALYSIS_H
