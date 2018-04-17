#pragma once
#include <map>

#include "file/filefunc.hpp"

namespace Base
{
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

    struct JsonNull : public JsonAny
    {

    };

    struct JsonArray : public JsonAny
    {
        std::vector<JsonAny> values;
        JsonAny get(int index);
    };

    struct JsonObject : public JsonAny
    {
        std::map<string, JsonAny*> values;
        JsonAny get(string name);
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
            size_t size, position, column, line;

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