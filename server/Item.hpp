#include "include/nlohmann_json.hpp"

using json = nlohmann::json;

class Item {
private:
	unsigned int id;
    json data;
	bool removed = false;
    
public:
	Item() {}

	Item(const json& data) : id((unsigned int)data["id"]), data(data), removed((bool)data["removed"]) {}

	json toJson() const {
		return data;
	}

	void fromJson(const json& data) {
		this->data = data;
		this->id = (unsigned int)data["id"];
		this->removed = (bool)data["removed"];
	}

	unsigned int getId() const { return id; }
	json getData() const { return data; }

	void setId(const unsigned int& id) { this->id = id; this->data["id"] = id; }
	void setData(const json& data) { this->data = data; this->id = (unsigned int)data["id"]; this->removed = (bool)data["removed"]; }

	void remove() { this->removed = true; this->data["removed"] = true; }
	void restore() { this->removed = false; this->data["removed"] = false; }
	bool isRemoved() const { return removed; }
};