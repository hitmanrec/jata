#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00  // Windows 10
#endif
#ifndef WINVER
#define WINVER 0x0A00  // Windows 10
#endif
#endif

#include <iostream>
#include <string>
#include "include/nlohmann_json.hpp"
#include "include/httplib.h"
#include "ItemsKeeper.hpp"

using json = nlohmann::json;

const std::string DEFAULT_STORAGE = "test_save.json";

int main(int argc, char* argv[]) {
    std::string inputFile = DEFAULT_STORAGE;
    std::string outputFile  = DEFAULT_STORAGE;
    int debugLevel = 0;
    if(argc > 1){
        for(int i = 1; i < argc; i++){
            if(strcmp(argv[i], "--debug") == 0){
                debugLevel = 3;
            }
            if(strcmp(argv[i], "--input") == 0){
                inputFile = argv[i+1];
                i++;
            }
            if(strcmp(argv[i], "--output") == 0){
                outputFile = argv[i+1];
                i++;
            }
        }
    }
    
    
    ItemsKeeper ik(inputFile);
    
    httplib::Server svr;

    // Enable CORS for frontend requests
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    // Handle CORS preflight requests
    svr.Options(".*", [](const httplib::Request&, httplib::Response& res) {
        return;
    });

    svr.Post("/api/categories", [&ik](const httplib::Request& req, httplib::Response& res) {
        try {
            json response;
            response = ik.get(json::parse(req.body));
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(json({{"error", e.what()}}).dump(), "application/json");
        }
    });

    svr.Post("/api/categories/add", [&ik](const httplib::Request& req, httplib::Response& res) {
        try {
            json response;
            response = ik.addCategory(json::parse(req.body));
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(json({{"error", e.what()}}).dump(), "application/json");
        }
    });

    svr.Post("/api/categories/update", [&ik](const httplib::Request& req, httplib::Response& res) {

        try {
            json response;
            response = ik.updateCategory(json::parse(req.body));
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(json({{"error", e.what()}}).dump(), "application/json");
        }
    });

    svr.Post("/api/categories/delete", [&ik](const httplib::Request& req, httplib::Response& res) {
        try {
            json response;
            response = ik.removeCategory(json::parse(req.body));
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(json({{"error", e.what()}}).dump(), "application/json");
        }
    });

    svr.Post("/api/items/add", [&ik](const httplib::Request& req, httplib::Response& res) {
        try {
            json response;
            response = ik.addItem(json::parse(req.body));
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(json({{"error", e.what()}}).dump(), "application/json");
        }
    });

    svr.Post("/api/items/update", [&ik](const httplib::Request& req, httplib::Response& res) {

        try {
            json response;
            response = ik.updateItem(json::parse(req.body));
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(json({{"error", e.what()}}).dump(), "application/json");
        }
    });

    svr.Post("/api/items/delete", [&ik](const httplib::Request& req, httplib::Response& res) {
        try {
            json response;
            response = ik.deleteItem(json::parse(req.body));
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(json({{"error", e.what()}}).dump(), "application/json");
        }
    });
    
    std::cout << "Server started at http://localhost:8081" << std::endl;
    svr.listen("0.0.0.0", 8081);
    
    return 0;
}

