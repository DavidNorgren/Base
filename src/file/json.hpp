#pragma once

#include "file/file.hpp"


namespace Base
{
    struct JsonException : public std::logic_error
    {
        using std::logic_error::logic_error;
    };

    struct JsonAny
    {

    };

    struct JsonBool : public JsonAny
    {
        bool value;
    };

    struct JsonNumber : public JsonAny
    {
        float value;
    };

    struct JsonString : public JsonAny
    {
        public:
            JsonString(string value);
            string value;
    };

    struct JsonNull : public JsonAny
    {

    };

    struct JsonArray : public JsonAny
    {
        list<JsonAny> values;
        JsonAny get(int index);
    };

    struct JsonObject : public JsonAny
    {
        map<string, JsonAny*> values;
        JsonObject* get(string name);
       // JsonArray* get(string name);
        
    };

    class JsonFile
    {
        public:
            EXPORT JsonFile(string filename);
            EXPORT JsonFile(File* filename);
            EXPORT ~JsonFile();
            EXPORT JsonAny get(string name);
            EXPORT string getLastError();

        private:
            JsonObject* root;
            char* data;
            char lastChar;
            uint size, position, column, line;

            void readRoot();
            char readCharacter();
            string readString();
            JsonObject* readObject();
            JsonAny* readAny();
            
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
    };
};