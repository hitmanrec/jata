#include "Item.hpp"
#include "include/nlohmann_json.hpp"
#include <unordered_map>
#include <fstream>


using json = nlohmann::json;


class FileManager {
private:
	std::string filePath;
public:
	FileManager(const std::string& filePath) : filePath(filePath) {}

	json loadJson() {
		std::ifstream inputFile(filePath);
		if(inputFile.is_open()){
			json data;
			inputFile >> data;
			inputFile.close();
			return data;
		}
		return json::parse("[]");
	}

	int writeJson(const json& data, std::string outputFilePath = "") {
		std::ofstream outputFile(outputFilePath.empty() ? filePath : outputFilePath);
		if(outputFile.is_open()){
			outputFile << data.dump(2);
			outputFile.close();
			return 0;
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
							status = "incomplete";
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

	void fromJson(const json& data) {
		this->id = (unsigned int)data["id"];
		this->name = data["name"];
		this->items = std::unordered_map<unsigned int, Item>();
		for (const auto& item : data["items"]) {
			items.insert(std::make_pair((unsigned int)item["id"], Item(item)));
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

public:
	ItemsKeeper(const std::string& filePath) : fm(filePath) {
		init();
	}

	int init() {
		LoadJsonFull();
		return 0;
	}

	json get(const unsigned int categoryId, const std::vector<int> itemIds){
		json data;
		data["items"] = json::array();
		for(const auto& itemId : itemIds){
			data["items"].push_back(categories[categoryId].items[itemId].toJson());
		}
		return data;

	}

	int LoadJsonFull() {
		json data = fm.loadJson();
		nextCategoryId = data["nextCategoryId"];
		nextItemId = data["nextItemId"];
		for(const auto& category : data["categories"]){
			Category c;
			c.fromJson(category);
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
