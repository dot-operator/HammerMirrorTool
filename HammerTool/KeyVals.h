#pragma once
#include <vector>
#include <memory>

using std::unique_ptr;
using std::vector;
using std::string;

class KeyVals
{
private:
	vector<unique_ptr<KeyVals>> children;
	vector<unique_ptr<KeyVals>>::iterator it;

	string key, value;

public:
	KeyVals* parent;
	KeyVals* addChild();
	void addChildExisting(KeyVals* kv);
	KeyVals* addChildTerminal(const string& key, const string& val);

	inline void removeLast() {
		children.pop_back();
	}
	inline KeyVals* getLast() {
		return children.back().get();
	}

	inline KeyVals* getFirst() {
		if (children.empty())
			return nullptr;

		it = children.begin();
		return children.begin()->get();
	}

	inline KeyVals* getNext() {
		if ((it != children.end()) && (++it != children.end())) {
			return it->get();
		}
		return nullptr;
	}
	inline bool hasNext() {
		return (it + 1) != children.end();
	}


	string getKey() {
		return key;
	};
	void setKey(const string& k) {
		key = k;
	};

	inline string getValString() {
		if (it == children.end()) {
			return value;
		}
		else return string();
	};
	void setValString(const string& v);

	KeyVals();
	KeyVals(string k, string v = "");
	KeyVals(const KeyVals& kv) = delete;
	KeyVals(KeyVals&& kv) = default;

	~KeyVals();
};

