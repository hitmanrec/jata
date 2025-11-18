#include "include/nlohmann_json.hpp"
#include <iostream>

using json = nlohmann::json;

class Item {
private:
	unsigned int id;
    json data;
	bool removed = false;
    
public:
	Item() {}

	Item(const json& data) : id(data["id"].get<unsigned int>()), data(data), removed(data["removed"].get<bool>()) {}

	json toJson() const {
		return data;
	}

	void fromJson(const json& data) {
		try{
			this->data = data;
			this->id = data["id"].get<unsigned int>();
			this->removed = data["removed"].get<bool>();
		}catch(const std::exception& e){
			std::cerr << "Error parsing item data: " << e.what() << std::endl;
			throw std::runtime_error("Error parsing item data");
		}
	}

	unsigned int getId() const { return id; }
	json getData() const { return data; }

	void setId(const unsigned int& id) { this->id = id; this->data["id"] = id; }
	void setData(const json& data) { this->data = data; this->id = data["id"].get<unsigned int>(); this->removed = data["removed"].get<bool>(); }

	void remove() { this->removed = true; this->data["removed"] = true; }
	void restore() { this->removed = false; this->data["removed"] = false; }
	bool isRemoved() const { return removed; }
};