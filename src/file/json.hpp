#pragma once

#include "file/file.hpp"

namespace Base
{
    /* Thrown whenever a some input JSON is badly formatted or
       an inappropriate get() method is called. */
    struct JsonException : public runtime_error
    {
        JsonException(string message) : runtime_error(message) {};
    };

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
    
    class JsonFile;
    class JsonObject;
    class JsonArray;

    /* A value in a JSON file. */
    class JsonAny
    {
      public:
        virtual JsonType getType() = 0;
    };

    /* A string of text in a JSON file. */
    class JsonString : public JsonAny
    {
      friend JsonFile;
      friend JsonObject;
      friend JsonArray;
      
      public:
        JsonType getType() { return JsonType::STRING; }

      protected:
        JsonString(string value) : value(value) {};
        string value;
    };

    /* A number (with or without decimals) in a JSON file. */
    class JsonNumber : public JsonAny
    {
      friend JsonFile;
      friend JsonObject;
      friend JsonArray;

      public:
        JsonType getType() { return JsonType::NUMBER; }

      protected:
        JsonNumber(float value) : value(value) {};
        float value;
    };

    /* A true/false boolean value in a JSON file. */
    class JsonBool : public JsonAny
    {
      friend JsonFile;
      friend JsonObject;
      friend JsonArray;

      public:
        JsonType getType() { return JsonType::BOOL; }
    
      protected:
        JsonBool(bool value) : value(value) {};
        bool value;
    };

    /* A null value in a JSON file. */
    class JsonNull : public JsonAny
    {
      public:
        JsonType getType() { return JsonType::NULLVALUE; }
    };

    /* An array in a JSON file with a list of values. */
    class JsonObject;
    class JsonArray : public JsonAny
    {
      friend JsonFile;

      public:
        // Add
        EXPORT JsonObject* addObject();
        EXPORT JsonArray* addArray();
        EXPORT JsonString* addString(string value);
        EXPORT JsonNumber* addNumber(float value);
        EXPORT JsonBool* addBool(bool value);
        EXPORT JsonNull* addNull();
        
        // Getters
        EXPORT JsonObject* getObject(uint index);
        EXPORT JsonArray* getArray(uint index);
        EXPORT string getString(uint index);
        EXPORT float getNumber(uint index);
        EXPORT bool getBool(uint index);
        EXPORT JsonType getType(uint index);
        EXPORT bool isNull(uint index);
        EXPORT uint getCount() { return values.size(); }

        JsonType getType() { return JsonType::ARRAY; }

      private:
        list<JsonAny*> values;
        JsonAny* add(JsonAny* any);
        JsonAny* get(uint index, JsonType type);
    };

    /* An object in a JSON file with a name->value mapping. */
    class JsonObject : public JsonAny
    {
      friend JsonFile;

      public:
        // Add
        EXPORT JsonObject* addObject(string name);
        EXPORT JsonArray* addArray(string name);
        EXPORT JsonString* addString(string name, string value);
        EXPORT JsonNumber* addNumber(string name, float value);
        EXPORT JsonBool* addBool(string name, bool value);
        EXPORT JsonNull* addNull(string name);

        // Getters
        EXPORT JsonType getType(string name);
        EXPORT JsonObject* getObject(string name);
        EXPORT JsonArray* getArray(string name);
        EXPORT string getString(string name);
        EXPORT float getNumber(string name);
        EXPORT bool getBool(string name);
        EXPORT bool isNull(string name);
        EXPORT uint getCount() { return values.size(); }

        JsonType getType() { return JsonType::OBJECT; }
    
      protected:
        list<string> keys;
        map<string, JsonAny*> values;
    
      private:
        JsonAny* add(string name, JsonAny* any);
        JsonAny* get(string name, JsonType type);
    };

    /* An opened JSON file with a root object. Includes methods for parsing and generating a text file. */
    class JsonFile : public JsonObject
    {
      public:
        EXPORT JsonFile(string filename);
        EXPORT JsonFile(File* filename);
        EXPORT JsonFile() {};
        EXPORT void save(string filename);

      private:
        // Reading
        char *data, lastChar;
        uint size, position, column, line;

        void readRoot();
        string readString();
        float readNumber();
        char readCharacter();

        JsonAny* readJsonAny();
        JsonObject* readJsonObject(bool isRoot = false);
        JsonArray* readJsonArray();
        JsonString* readJsonString();
        JsonNumber* readJsonNumber();
        JsonAny* readJsonWord();

        // Writing
        string saveStr;
        uint tabs;
        bool addComma;

        void writeTabs();
        void writeChar(char ch);
        void writeString(string str);
        void writeEndPrevious();
        void writeStartSet(char ch);
        void writeEndSet(char ch);

        void writeJsonAny(JsonAny* any);
        void writeJsonObject(Base::JsonObject* obj);
        void writeJsonArray(Base::JsonArray* arr);
        void writeJsonString(Base::JsonString* str);
        void writeJsonNumber(Base::JsonNumber* num);
        void writeJsonBool(Base::JsonBool* b);
        void writeJsonNull();
    };
};