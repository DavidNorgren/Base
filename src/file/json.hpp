#pragma once

#include "file/filepath.hpp"
#include "util/data/list.hpp"
#include "util/data/size2d.hpp"
#include "util/data/size3d.hpp"
#include "util/data/vec2.hpp"
#include "util/data/vec3.hpp"

namespace Base
{
    /* Thrown whenever a some input JSON is badly formatted or
       an inappropriate get() method is called. */
    struct JsonException : public runtime_error
    {
        JsonException(const string& message) : runtime_error(message) {};
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
        "OBJECT",
        "ARRAY",
        "STRING",
        "NUMBER",
        "BOOL",
        "NULL"
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
        JsonString(const string& value) : value(value) {};
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
      public:
        // Add
        EXPORT JsonObject*  addObject();
        EXPORT JsonArray*   addArray();
        EXPORT JsonString*  addString(const string& value);
        EXPORT JsonNumber*  addNumber(float value);
        EXPORT JsonBool*    addBool(bool value);
        EXPORT JsonNull*    addNull();
        
        // Getters
        EXPORT JsonType     getType(uint index);
        EXPORT JsonObject*  getObject(uint index);
        EXPORT JsonArray*   getArray(uint index);
        EXPORT string       getString(uint index);
        EXPORT float        getNumber(uint index);
        EXPORT bool         getBool(uint index);

        template <typename T> inline Vec2<T> getVec2(uint index)
        {
            return Vec2<T>(
                getArray(index)->getNumber(0),
                getArray(index)->getNumber(1)
            );
        };

        template <typename T> inline Vec3<T> getVec3(uint index)
        {
            return Vec3<T>(
                getArray(index)->getNumber(0),
                getArray(index)->getNumber(1),
                getArray(index)->getNumber(2)
            );
        };

        template <typename T> Size2D<T> getSize2D(uint index)
        {
            return Size2D<T>(
                getArray(index)->getNumber(0),
                getArray(index)->getNumber(1)
            );
        };

        template <typename T> Size3D<T> getSize3D(uint index)
        {
            return Size2D<T>(
                getArray(index)->getNumber(0),
                getArray(index)->getNumber(1),
                getArray(index)->getNumber(2)
            );
        };

        EXPORT bool         isNull(uint index);
        EXPORT uint         getCount() { return values.size(); }
        EXPORT const List<JsonAny*>&  getValues() { return values; }

        JsonType            getType() { return JsonType::ARRAY; }

      friend class JsonFile;
      private:
        List<JsonAny*> values;
        JsonAny* add(JsonAny* any);
        JsonAny* get(uint index, JsonType type);
    };

    /* An object in a JSON file with a name->value mapping. */
    class JsonObject : public JsonAny
    {
      public:
        // Add
        EXPORT JsonObject*  addObject(const string& name);
        EXPORT JsonArray*   addArray(const string& name);
        EXPORT JsonString*  addString(const string& name, const string& value);
        EXPORT JsonNumber*  addNumber(const string& name, float value);
        EXPORT JsonBool*    addBool(const string& name, bool value);
        EXPORT JsonNull*    addNull(const string& name);

        // Getters
        EXPORT JsonType     getType(const string& name);
        EXPORT JsonObject*  getObject(const string& name);
        EXPORT JsonArray*   getArray(const string& name);
        EXPORT string       getString(const string& name);
        EXPORT float        getNumber(const string& name);
        EXPORT bool         getBool(const string& name);

        template <typename T> Vec2<T> getVec2(const string& name)
        {
            return Vec2<T>(
                getArray(name)->getNumber(0),
                getArray(name)->getNumber(1)
            );
        }

        template <typename T> Vec3<T> getVec3(const string& name)
        {
            return Vec3<T>(
                getArray(name)->getNumber(0),
                getArray(name)->getNumber(1),
                getArray(name)->getNumber(2)
            );
        }

        template <typename T> Size2D<T> getSize2D(const string& name)
        {
            return Size2D<T>(
                getArray(name)->getNumber(0),
                getArray(name)->getNumber(1)
            );
        }

        template <typename T> Size3D<T> getSize3D(const string& name)
        {
            return Size3D<T>(
                getArray(name)->getNumber(0),
                getArray(name)->getNumber(1),
                getArray(name)->getNumber(2)
            );
        }

        EXPORT bool         isNull(const string& name);
        EXPORT bool         getKeyExists(const string& name) { return values.find(name) != values.end(); }
        EXPORT uint         getCount() { return values.size(); }
        EXPORT const List<string>&  getKeys() { return keys; }

        JsonType            getType() { return JsonType::OBJECT; }
    
      friend class JsonFile;
      protected:
        List<string> keys;
        Map<string, JsonAny*> values;
    
      private:
        JsonAny* add(const string& name, JsonAny* any);
        JsonAny* get(const string& name, JsonType type);
    };

    /* An opened JSON file with a root object. Includes methods for parsing and generating a text file. */
    class JsonFile : public JsonObject
    {
      public:
        EXPORT      JsonFile(const FilePath& file);
        EXPORT      JsonFile(const string& json);
        EXPORT      JsonFile() {};
        EXPORT void save(const FilePath& filename);

      private:
        // Reading
        char *data, lastChar;
        uint size, position, column, line;

        void        readRoot();
        string      readString();
        float       readNumber();
        char        readCharacter();

        JsonAny*    readJsonAny();
        JsonObject* readJsonObject(bool isRoot = false);
        JsonArray*  readJsonArray();
        JsonString* readJsonString();
        JsonNumber* readJsonNumber();
        JsonAny*    readJsonWord();

        // Writing
        string saveStr;
        uint tabs;
        bool addComma;

        void writeTabs();
        void writeChar(char ch);
        void writeString(const string& str);
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