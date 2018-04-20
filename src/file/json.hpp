#pragma once

#include "file/file.hpp"

namespace Base
{
    /* A JSON type that holds a value or named/unnamed set of other JSON types. */
    enum class JsonType
    {
        OBJECT,
        ARRAY,
        STRING,
        NUMBER,
        BOOL,
        NULLVALUE
    };

    static const string JsonTypeName[] = {
        "BOOL",
        "NULL",
        "NUMBER",
        "STRING",
        "ARRAY",
        "OBJECT"
    };

    /* Thrown whenever a some input JSON is badly formatted or
       an inappropriate get() method is called. */
    struct JsonException : public runtime_error
    {
        JsonException(string message) : runtime_error(message) {};
    };

    /* A value in a JSON file. */
    struct JsonAny
    {
        virtual JsonType getType() = 0;
        virtual void write() = 0;
    };

    /* A true/false boolean value in a JSON file. */
    struct JsonBool : public JsonAny
    {
        JsonBool(bool value) : value(value) {};
        JsonType getType() { return JsonType::BOOL; }
        void write();
        bool value;
    };

    /* A number (with or without decimals) in a JSON file. */
    struct JsonNumber : public JsonAny
    {
        JsonNumber(float value) : value(value) {};
        JsonType getType() { return JsonType::NUMBER; }
        void write();
        float value;
    };

    /* A string of text in a JSON file. */
    struct JsonString : public JsonAny
    {
        JsonString(string value) : value(value) {};
        JsonType getType() { return JsonType::STRING; }
        void write();
        string value;
    };

    /* A null value in a JSON file. */
    struct JsonNull : public JsonAny
    {
        JsonType getType() { return JsonType::NULLVALUE; }
        void write();
    };

    /* An array in a JSON file with a list of values. */
    class JsonObject;
    class JsonArray : public JsonAny
    {
      public:
        // Add
        EXPORT void addObject(JsonObject* obj);
        EXPORT void addArray(JsonArray* arr);
        EXPORT void addString(string value);
        EXPORT void addNumber(float value);
        EXPORT void addBool(bool value);
        EXPORT void addNull();
        
        // Getters
        EXPORT JsonObject* getObject(uint index);
        EXPORT JsonArray* getArray(uint index);
        EXPORT string getString(uint index);
        EXPORT float getNumber(uint index);
        EXPORT bool getBool(uint index);
        EXPORT JsonType getType(uint index);
        EXPORT bool isNull(uint index);

        JsonType getType() { return JsonType::ARRAY; }
        void write();
        list<JsonAny*> values;

      private:
        JsonAny* get(uint index, JsonType type);
    };

    /* An object in a JSON file with a name->value mapping. */
    class JsonObject : public JsonAny
    {
      public:
        // Add
        EXPORT void addObject(string name, JsonObject* obj);
        EXPORT void addArray(string name, JsonArray* arr);
        EXPORT void addString(string name, string value);
        EXPORT void addNumber(string name, float value);
        EXPORT void addBool(string name, bool value);
        EXPORT void addNull(string name);

        // Getters
        EXPORT JsonType getType(string name);
        EXPORT JsonObject* getObject(string name);
        EXPORT JsonArray* getArray(string name);
        EXPORT string getString(string name);
        EXPORT float getNumber(string name);
        EXPORT bool getBool(string name);
        EXPORT bool isNull(string name);

        JsonType getType() { return JsonType::OBJECT; }
        void write();
        list<string> keys;
        map<string, JsonAny*> values;
    
      private:
        JsonAny* get(string name, JsonType type);
    };

    /* An opened JSON file with a root object. Includes methods for parsing and generating a text file. */
    class JsonFile : public JsonObject
    {
      public:
        EXPORT JsonFile(string filename);
        EXPORT JsonFile(File* filename);
        EXPORT JsonFile() {};
        EXPORT ~JsonFile() {};
        EXPORT void save(string filename);

      private:

        char* data;
        char lastChar;
        uint size, position, column, line;

        void readRoot();
        string readString();
        float readNumber();
        char readCharacter();

        JsonAny* readJsonAny();
        JsonObject* readJsonObject(bool isRoot = false);
        JsonArray* readJsonArray();
        JsonNumber* readJsonNumber();
        JsonString* readJsonString();
        JsonAny* readJsonWord();
    };
};