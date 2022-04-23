/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef URLSCANNER_H
#define URLSCANNER_H


#include "AbstractScanner.h"
#include "src/utils/s3s.h"
#include "src/items/URLItem.h"

#include <QMutex>
#include <QQueue>

#include <Ultralight/Ultralight.h>
#include <AppCore/Platform.h>


namespace url {

struct ScanStat {  // scan statistics
    int targets = 0;
    int threads = 0;
    int resolved = 0;
    int failed = 0;
};

struct ScanConfig { // scan configurations
    int threads = 50;
    int timeout = 1000;
    QString scheme = "https";

    bool get_title = false;
    bool take_screenshots = false;
    bool follow_redirect = false;
    bool force_scheme = false;
    bool setTimeout = false;
    bool noDuplicates = false;
    bool autoSaveToProject = false;
};

struct ScanArgs { // scan arguments
    QMutex mutex;
    url::ScanConfig *config;
    QQueue<QString> targets;
    int progress;
};

///
/// ultralight for screenshots...
///

class Screenshot : public ultralight::LoadListener {

    ultralight::RefPtr<ultralight::Renderer> renderer_;
    ultralight::RefPtr<ultralight::View> view_;
    bool done_ = false;

public:
    Screenshot()
    {
        ultralight::Config config;
        config.device_scale = 2.0;
        config.font_family_standard = "Arial";

        config.use_gpu_renderer = false;
        ultralight::Platform::instance().set_config(config);
        ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());
        ultralight::Platform::instance().set_file_system(ultralight::GetPlatformFileSystem("."));
        renderer_ = ultralight::Renderer::Create();
        view_ = renderer_->CreateView(800, 800, false, nullptr);
        view_->set_load_listener(this);
    }
    virtual ~Screenshot() override {
        view_ = nullptr;
        renderer_ = nullptr;
    }

    void take_screenshot(QString url, const char* html){
        view_->LoadHTML(html);

        while (!done_) {
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
          renderer_->Update();
          renderer_->Render();
        }
        ultralight::BitmapSurface* bitmap_surface = static_cast<ultralight::BitmapSurface*>(view_->surface());
        ultralight::RefPtr<ultralight::Bitmap> bitmap = bitmap_surface->bitmap();
        bitmap->SwapRedBlueChannels();
        bitmap->WritePNG(url.append(".png").toUtf8());
        done_ = false;
    }

    virtual void OnFinishLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const ultralight::String& url) override {
        Q_UNUSED(caller);
        Q_UNUSED(frame_id);
        Q_UNUSED(url);
        if (is_main_frame)
          done_ = true;
    }
};

///
/// custom networkaccessmanager for URL engine...
///

class NetworkAccessManager: public QNetworkAccessManager {
    public:
        NetworkAccessManager(QObject *parent = nullptr, int timeout = 1000, bool use_timer = false): QNetworkAccessManager(parent),
            m_timeout(timeout),
            m_use_timer(use_timer)
        {
        }

    protected:
        QNetworkReply* createRequest(Operation op, const QNetworkRequest &request, QIODevice *data = nullptr)
        {
            QNetworkReply *reply = QNetworkAccessManager::createRequest(op, request, data);

            /* set timeout */
            if(m_use_timer)
                s3s_ReplyTimeout::set(reply, m_timeout);

            return reply;
        }

    private:
        int m_timeout;
        bool m_use_timer;
};

///
/// \brief The Scanner class
///

class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        explicit Scanner(url::ScanArgs *args);
        ~Scanner() override;

    private slots:
        void lookup() override;
        void lookupFinished(QNetworkReply *reply);

    signals:
        void next();
        void scanResult(s3s_struct::URL);

    private:
        url::ScanArgs *m_args;
        url::NetworkAccessManager *m_manager;
        void get_title(s3s_struct::URL &url, QNetworkReply *reply);
};

RETVAL getTarget(url::ScanArgs *args, QUrl &url);

}

#endif // URLSCANNER_H
