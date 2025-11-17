#include "Item.hpp"
#include "include/nlohmann_json.hpp"
#include <unordered_map>
#include <fstream>
#include <iostream>


using json = nlohmann::json;


class FileManager {
private:
	std::string filePath;
public:
	FileManager(const std::string& _filePath) : filePath(_filePath) {}

	json loadJson() {
		std::ifstream inputFile(filePath);
		if(inputFile.is_open()){
			try{
			json data;
				inputFile >> data;
				inputFile.close();
				return data;
			}catch(const std::exception& e){
				std::cerr << "Error reading from file: " << e.what() << std::endl;
				return json::parse("[]");
			}
		}
		return json::parse("[]");
	}

	int writeJson(const json& data, std::string outputFilePath = "") {
		std::ofstream outputFile(outputFilePath.empty() || outputFilePath == "" ? filePath : outputFilePath);
		if(outputFile.is_open()){
			try{
				outputFile << data.dump(2);
				outputFile.close();
				return 0;
			}catch(const std::exception& e){
				std::cerr << "Error writing to file: " << e.what() << std::endl;
				return -1;
			}
		}
		return -1;
	}

	json txt2json(std::string outputFilePath, unsigned int& nextCategoryId, unsigned int& nextItemId){
		bool writeToFile = !outputFilePath.empty();

		std::ifstream inputFile(filePath);
		if(inputFile.is_open()){
			std::string line;
			json data;
			data["categories"] = json::array();
			data["nextCategoryId"] = nextCategoryId;
			data["nextItemId"] = nextItemId;

			std::string categoryName = "";

			while(std::getline(inputFile, line)){
				std::string title = "";
				std::string status = "";
				std::string description = "";
				if(line[0] != '-' && line[0] != '+'){
					categoryName = line;
					data["categories"].push_back({
						{"id", nextCategoryId}, 
						{"name", categoryName}, 
						{"removed", false}, 
						{"items", json::array()}
					});
					nextCategoryId++;
					data["nextCategoryId"] = nextCategoryId;
				}else{
					for(int i = 0; i < line.size(); i++){
						if(line[i] == '-'){
							status = "planned";
						}else if(line[i] == '+'){
							status = "complete";
						}else if(line[i] == '('){
							for(i++; i < line.size(); i++){
								if(line[i] == ')'){
									break;
								}
								description += line[i];
							}
						}else{
							title += line[i];
						}
					}
					data["categories"].back()["items"].push_back({
						{"id", nextItemId}, 
						{"title", title}, 
						{"status", status}, 
						{"description", description}, 
						{"removed", false},
						{"rating", ""},
						{"expectation", ""},
						{"creationTime", ""},
						{"completionTime", ""},
						{"tags", json::array()}
					});
					nextItemId++;
					data["nextItemId"] = nextItemId;
				}
			}
			inputFile.close();
			if(writeToFile){
				std::ofstream outputFile(outputFilePath);
				if(outputFile.is_open()){
					outputFile << data.dump(2);
					outputFile.close();
				}
			}
			return data;
		}
		return json::parse("[]");
	}
};

struct Category {
	std::string name;
	std::unordered_map<unsigned int, Item> items;
	unsigned int id;
	bool removed = false;

	json toJson() const {
		json data;
		data["id"] = id;
		data["name"] = name;
		data["items"] = json::array();
		for (const auto& item : items) {
			data["items"].push_back(item.second.toJson());
		}
		data["removed"] = removed;
		return data;
	}

	void fromJson(const json& data, int debugLevel = 0) {
		this->id = (unsigned int)data["id"];
		this->name = data["name"];
		this->items = std::unordered_map<unsigned int, Item>();

		if(data.contains("items")){
			for (const auto& item : data["items"]) {
				items.insert(std::make_pair((unsigned int)item["id"], Item(item)));
			}
		}
		this->removed = (bool)data["removed"];
	}
};

class ItemsKeeper {
private:
	std::unordered_map<unsigned int, Category> categories;
	unsigned int nextItemId;
	unsigned int nextCategoryId;
	FileManager fm;
	int debugLevel = 0;

public:
	ItemsKeeper(const std::string& filePath, int debugLevel = 0) : fm(filePath), debugLevel(debugLevel) {
		init();
	}

	int init() {
		LoadJsonFull();
		return 0;
	}

	json get(json request){
		json response;
		bool getRemoved = request.contains("removed") && request["removed"];
		if(request.contains("categoryIds")){
			response["categories"] = json::array();
			if(request["categoryIds"] == "all"){
				for(const auto& category : categories){
					if(!getRemoved && category.second.removed){
						continue;
					}
					response["categories"].push_back({
						{"id", category.second.id}, 
						{"name", category.second.name}, 
						{"removed", category.second.removed}
					});
				}
			}else{
				for(const auto& categoryId : request["categoryIds"]){
					if(!getRemoved && categories[categoryId].removed){
						continue;
					}
					if(request.contains("withItems") && request["withItems"]){
						response["categories"].push_back(categories[categoryId].toJson());
					}else{
						response["categories"].push_back({
							{"id", categoryId},
							{"name", categories[categoryId].name},
							{"removed", categories[categoryId].removed}
						});
					}
				}
			}
		}
		if(request.contains("nextIds") && request["nextIds"]){
			response["nextCategoryId"] = nextCategoryId;
			response["nextItemId"] = nextItemId;
		}
		return response;

	}

	json addCategory(json request){
		json response;
		Category c;
		try{
			c.fromJson(request["category"]);
			if(request["nextCategoryId"] != nextCategoryId){
				nextCategoryId = request["nextCategoryId"].get<unsigned int>();
			}
			categories.insert(std::make_pair(c.id, c));
			WriteJsonFull();
			response["category"] = json::object({
				{"id", c.id},
				{"name", c.name}
			});
			response["nextCategoryId"] = nextCategoryId;
			return response;
		}catch(const std::exception& e){
			std::cerr << "Error adding category: " << e.what() << std::endl;
			response["error"] = e.what();
			return response;
		}
	}

	json updateCategory(json request){
		json response;
		Category c;
		try{
			c.fromJson(json::object({{"id", request["category"]["id"]}, {"name", request["category"]["name"]}, {"removed", false}, {"items", json::array()}}));
			categories[c.id].name = c.name;
			WriteJsonFull();
			response["category"] = json::object({
				{"id", c.id},
				{"name", c.name}
			});
			return response;
		}catch(const std::exception& e){
			std::cerr << "Error updating category: " << e.what() << std::endl;
			response["error"] = e.what();
			return response;
		}
	}

	json removeCategory(json request){
		json response;
		int categoryId = request["categoryId"];
		try{
			if(categories.find(categoryId) != categories.end()){
				categories[categoryId].removed = true;
			}else{
				throw std::runtime_error("Category not found");
			}
			response["category"] = json::object({
				{"id", categoryId},
				{"name", categories[categoryId].name},
				{"removed", categories[categoryId].removed}
			});
			WriteJsonFull();
			return response;
		}
		catch(const std::exception& e){
			std::cerr << "Error removing category: " << e.what() << std::endl;
			response["error"] = e.what();
			return response;
		}
	}

	json addItem(json request){
		json response;
		try{
			Item i;
			i.fromJson(request["item"]);
			if(request["nextItemId"] != nextItemId){
				nextItemId = request["nextItemId"].get<unsigned int>();
			}
			categories[request["categoryId"]].items.insert(std::make_pair(request["item"]["id"], i));
			response["categoryId"] = request["categoryId"];
			WriteJsonFull();
			response["item"] = i.toJson();
			response["nextItemId"] = nextItemId;
			return response;
		}catch(const std::exception& e){
			std::cerr << "Error adding item: " << e.what() << std::endl;
			response["error"] = e.what();
			return response;
		}
	}

	json updateItem(json request){
		json response;
		int categoryId = request["categoryId"];
		try {
			Item i;
			i.fromJson(request["item"]);
			categories[categoryId].items[i.getId()] = i;
			response["categoryId"] = categoryId;
			WriteJsonFull();
			response["item"] = i.toJson();
			return response;
		} catch(const std::exception& e){
			std::cerr << "Error updating item: " << e.what() << std::endl;
			response["error"] = e.what();
			return response;
		}
	}

	json deleteItem(json request){
		json response;
		int categoryId = request["categoryId"];
		int itemId = request["itemId"];
		try{
			if(categories[categoryId].items.find(itemId) != categories[categoryId].items.end()){
				categories[categoryId].items[itemId].remove();
			}else{
				throw std::runtime_error("Item not found");
			}
			WriteJsonFull();
			response["itemId"] = itemId;
			return response;
		}catch(const std::exception& e){
			std::cerr << "Error deleting item: " << e.what() << std::endl;
			response["error"] = e.what();
			return response;
		}
	}

	int LoadJsonFull() {
		json data = fm.loadJson();
		nextCategoryId = data["nextCategoryId"];
		nextItemId = data["nextItemId"];
		for(const auto& category : data["categories"]){
			Category c;
			c.fromJson(category, debugLevel);
			categories.insert(std::make_pair(c.id, c));
		}
		return 0;
	}

	int WriteJsonFull(std::string outputFilePath = "") {
		json data;
		data["nextCategoryId"] = nextCategoryId;
		data["nextItemId"] = nextItemId;
		data["categories"] = json::array();
		for(const auto& category : categories){
			data["categories"].push_back(category.second.toJson());
		}
		return fm.writeJson(data, outputFilePath);
	}

	//categories management


	//items management


};
