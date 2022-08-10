#ifndef HTTPGD_WEB_TASK_H
#define HTTPGD_WEB_TASK_H

#include <memory>
#include <thread>
#include <mutex>
#include <unordered_set>
#include <unigd_api.h>
#include <crow.h>

namespace httpgd
{
    namespace web
    {
        constexpr int httpgd_client_id = 104;

        struct HttpgdServerConfig
        {
            std::string host;
            int port;
            std::string wwwpath;
            bool cors;
            bool use_token;
            std::string token;
            bool record_history;
            bool silent;
            std::string id;
        };
        
        class HttpgdLogHandler : public crow::ILogHandler {
            public:
                void log(std::string message, crow::LogLevel level) override;
            private:
                static std::string timestamp();
        };

        class WebServer
        {
        public:
            WebServer(const HttpgdServerConfig &t_config);

            bool attach(int devnum);

            void device_start();
            void device_close();
            void device_state_change();
            int device_client_id();

            std::string client_status();
            const HttpgdServerConfig &get_config();

            unsigned short port();
            void broadcast_state(const unigd_device_state &state);

        private:
            unigd_api_v1 *m_api = nullptr;
            UNIGD_HANDLE m_ugd_handle;
            unigd_graphics_client m_client;
            
            HttpgdServerConfig m_conf;
            crow::App<crow::CORSHandler> m_app;
            HttpgdLogHandler m_log_handler;
            std::mutex m_mtx_update_subs;
            std::unordered_set<crow::websocket::connection*> m_update_subs;
            int m_last_upid = -1;
            bool m_last_active = true;
            std::thread m_server_thread;

            void run();
        };
    } // namespace web
} // namespace httpgd

#endif