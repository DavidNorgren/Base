#pragma once

#include "file/file.hpp"

namespace Base
{
    enum JsonType
    {
        BOOL,
        NULLVALUE,
        NUMBER,
        STRING,
        ARRAY,
        OBJECT
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
    };

    /* A true/false boolean value in a JSON file. */
    struct JsonBool : public JsonAny
    {
        JsonBool(bool value) : value(value) {};
        JsonType getType() { return JsonType::BOOL; }
        bool value;
    };

    /* A number (with or without decimals) in a JSON file. */
    struct JsonNumber : public JsonAny
    {
        JsonNumber(float value) : value(value) {};
        JsonType getType() { return JsonType::NUMBER; }
        float value;
    };

    /* A string of text in a JSON file. */
    struct JsonString : public JsonAny
    {
        JsonString(string value) : value(value) {};
        JsonType getType() { return JsonType::STRING; }
        string value;
    };

    /* A null value in a JSON file. */
    struct JsonNull : public JsonAny
    {
        JsonType getType() { return JsonType::NULLVALUE; }
    };

    /* An array in a JSON file with a list of values. */
    class JsonObject;
    class JsonArray : public JsonAny
    {
      public:
        EXPORT JsonType getType(uint index);
        EXPORT JsonObject* getObject(uint index);
        EXPORT JsonArray* getArray(uint index);
        EXPORT string getString(uint index);
        EXPORT float getNumber(uint index);
        EXPORT bool getBool(uint index);

        JsonType getType() { return JsonType::ARRAY; }
        list<JsonAny*> values;

      private:
        JsonAny* get(uint index, JsonType type);
    };

    /* An object in a JSON file with a name->value mapping. */
    class JsonObject : public JsonAny
    {
      public:
        EXPORT JsonType getType(string name);
        EXPORT JsonObject* getObject(string name);
        EXPORT JsonArray* getArray(string name);
        EXPORT string getString(string name);
        EXPORT float getNumber(string name);
        EXPORT bool getBool(string name);
    
        JsonType getType() { return JsonType::OBJECT; }
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
        EXPORT ~JsonFile() {};
    
      private:
        enum Character
        {
            CURLY_BEGIN     = '{',
            CURLY_END       = '}',
            SQUARE_BEGIN    = '[',
            SQUARE_END      = ']',
            COMMA           = ',',
            COLON           = ':',
            QUOTE           = '"',
            SPACE           = ' ',
            TAB             = '\t',
            NEW_LINE        = '\n',
            RETURN          = '\r',
            POINT           = '.',
            MINUS           = '-',
            PLUS            = '+',
            BACKSLASH       = '\\',
            E               = 'e',
            CAPITAL_E       = 'E',
            F               = 'f',
            N               = 'n',
            T               = 't',
            U               = 'u',
            NUM_0           = '0',
            NUM_9           = '9'
        };

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