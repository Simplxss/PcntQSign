#include "server.h"
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"

std::string Server::GetSign(const std::string_view &cmd, const std::string_view &src, const int seq)
{
    auto [signDataHex, extraDataHex, tokenDataHex] = Sign::Call(cmd, src, seq);
    counter++;

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject();
    writer.Key("value");
    writer.StartObject();
    writer.Key("sign");
    writer.String(signDataHex.c_str());
    writer.Key("extra");
    writer.String(extraDataHex.c_str());
    writer.Key("token");
    writer.String(tokenDataHex.c_str());
    writer.EndObject();
    writer.EndObject();

    return buffer.GetString();
}

void Server::Init()
{
    svr.Post("/sign", [this](const httplib::Request &req, httplib::Response &res)
             {
            try {
                rapidjson::Document doc;
                doc.Parse(req.body.c_str(), req.body.size());

                std::string_view cmd = doc["cmd"].GetString();
                std::string_view src = doc["src"].GetString();
                int seq = doc["seq"].GetInt();

                std::string buffer = GetSign(cmd, src, seq);

                res.set_content(buffer, "application/json");
            }
            catch (...) {
                res.set_content("Bad Request", "text/plain");
                res.status = httplib::StatusCode::BadRequest_400;
            } })
        .Get("/sign", [this](const httplib::Request &req, httplib::Response &res)
             {
            try {
                std::string cmd = req.get_param_value("cmd");
                std::string src = req.get_param_value("src");
                int seq = std::stoi(req.get_param_value("seq"));

                std::string buffer = GetSign(cmd, src, seq);

                res.set_content(buffer, "application/json");
            }
            catch (...) {
                res.set_content("Bad Request", "text/plain");
                res.status = httplib::StatusCode::BadRequest_400;
            } })
        .Get("/ping", [](const httplib::Request &req, httplib::Response &res)
             {
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

            writer.StartObject();
            writer.Key("code");
            writer.Int(0);
            writer.EndObject();

            res.set_content(buffer.GetString(), "application/json"); })
        .Get("/count", [this](const httplib::Request &req, httplib::Response &res)
             {
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

            writer.StartObject();
            writer.Key("count");
            writer.String(std::to_string(counter.load()).c_str());
            writer.EndObject();

            res.set_content(buffer.GetString(), "application/json"); });
}

bool Server::Run(const std::string &ip, int port)
{
    return svr.listen(ip.c_str(), port);
}
