#include "common.hpp"
#include "file/json.hpp"
#include "file/filefunc.hpp"
#include "util/stringfunc.hpp"

EXPORT Base::JsonFile::JsonFile(string filename)
{
    string json = fileGetContents(filename);
    data = &json[0];
    size = json.length();
    readRoot();
}

EXPORT Base::JsonFile::JsonFile(File* file)
{
    data = file->rawData;
    size = file->size;
    readRoot();
}

void Base::JsonFile::readRoot()
{
    position = 0;
    column = 0;
    line = 1;

    try
    {
        if (readCharacter() != Character::CURLY_BEGIN)
            throw JsonException("No root object found");
        
        readJsonObject(true);
    }
    catch (JsonException& ex)
    {
        throw JsonException("JSON parse error: " + toString(ex.what()) + " at line " + toString(line) + ", column " + toString(column));
    }
}

string Base::JsonFile::readString()
{
    uint stringStart = position;

    while (true)
    {
        // Check EOF
        if (position >= size)
            throw JsonException("Unexpected end of file");
        
        // Advance
        lastChar = data[position++];
        column++;
        
        // Check end of string
        if (lastChar == Character::QUOTE)
            break;

        // Check invalid linebreak
        if (lastChar == Character::RETURN || lastChar == Character::NEW_LINE)
            throw JsonException("Unexpected linebreak in string");

        // Read special character
        if (lastChar == Character::BACKSLASH)
        {
            // TODO
        }
    }

    uint stringEnd = position - 1;

    readCharacter();

    return string(&data[stringStart], stringEnd - stringStart);
}

float Base::JsonFile::readNumber()
{
    string numStr = "";

    do
    {
        // Check EOF
        if (position >= size)
            throw JsonException("Unexpected end of file");

        // Add to final string
        numStr += lastChar;

        // Advance
        lastChar = data[position++];
        column++;
    }
    while ((lastChar >= Character::NUM_0 && lastChar <= Character::NUM_9) ||
           lastChar == Character::MINUS || lastChar == Character::PLUS ||
           lastChar == Character::E || lastChar == Character::CAPITAL_E ||
           lastChar == Character::POINT);

    // Skip whitespace
    if (lastChar == Character::TAB ||
        lastChar == Character::SPACE ||
        lastChar == Character::NEW_LINE ||
        lastChar == Character::RETURN)
        readCharacter();

    // Convert from string
    try
    {
        return std::stof(numStr);
    }
    catch (std::logic_error& ex)
    {
        throw JsonException("Invalid number " + numStr);
    }
}

char Base::JsonFile::readCharacter()
{
    // Check EOF
    if (position >= size)
        throw JsonException("Unexpected end of file");

    do
    {
        // Advance
        lastChar = data[position++];

        // Column and line counter
        if (lastChar == Character::TAB)
            column += 4;
        else if (lastChar == Character::NEW_LINE)
        {
            line++;
            column = 0;
        }
        else
            column++;
    }
    // Skip blank characters
    while (lastChar == Character::TAB ||
           lastChar == Character::SPACE ||
           lastChar == Character::NEW_LINE ||
           lastChar == Character::RETURN ||
           lastChar > 127);

    return lastChar;
}

Base::JsonAny* Base::JsonFile::readJsonAny()
{
    if (lastChar == Character::CURLY_BEGIN)
        return readJsonObject();
    else if (lastChar == Character::SQUARE_BEGIN)
        return readJsonArray();
    else if (lastChar == Character::QUOTE)
        return readJsonString();
    else if ((lastChar >= Character::NUM_0 && lastChar <= Character::NUM_9) || lastChar == Character::MINUS)
        return readJsonNumber();
    else
        return readJsonWord();
}

Base::JsonObject* Base::JsonFile::readJsonObject(bool isRoot)
{
    JsonObject* obj = isRoot ? this : new JsonObject();

    while (readCharacter() != Character::CURLY_END)
    {
        string name = readString();

        // Check Colon
        if (lastChar != Character::COLON)
            throw JsonException("Expected :");
        
        readCharacter();

        // Read value
        obj->values[name] = readJsonAny();

        // End of values
        if (lastChar == Character::CURLY_END)
            break;

        // Look for value separator
        if (lastChar != Character::COMMA)
            throw JsonException("Expected ,");
    }

    if (!isRoot)
        readCharacter();

    return obj;
}

Base::JsonArray* Base::JsonFile::readJsonArray()
{
    JsonArray* arr = new JsonArray();

    while (readCharacter() != Character::SQUARE_END)
    {
        arr->values.push_back(readJsonAny());
        
        // End of values
        if (lastChar == Character::SQUARE_END)
            break;

        // Look for value separator
        if (lastChar != Character::COMMA)
            throw JsonException("Expected ,");
    }

    readCharacter();

    return arr;
}

Base::JsonNumber* Base::JsonFile::readJsonNumber()
{
    return new JsonNumber(readNumber());
}

Base::JsonString* Base::JsonFile::readJsonString()
{
    return new JsonString(readString());
}

Base::JsonAny* Base::JsonFile::readJsonWord()
{
    switch (lastChar)
    {
        // "true"
        case Character::T:
        {
            repeat (4)
                readCharacter();
            
            return new JsonBool(true);
        }
        
        // "false"
        case Character::F:
        {
            repeat (5)
                readCharacter();
            
            return new JsonBool(false);
        }

        // "null"
        case Character::N:
        {
            repeat (4)
                readCharacter();

            return new JsonNull();
        }
    }

    throw JsonException("Unrecognized word");
}

EXPORT Base::JsonType Base::JsonArray::getType(uint index)
{
    if (index < 0 || index >= values.size())
        throw JsonException("Index " + toString(index) + " out of range [0, " + toString(values.size() - 1) + "]");

    return values[index]->getType();
}

Base::JsonAny* Base::JsonArray::get(uint index, JsonType type)
{
    try
    {
        JsonType vType = getType(index);
        if (vType != type)
            throw JsonException("Unexpected type " + JsonTypeName[vType] + ", expected " + JsonTypeName[type]);

        return values[index];
    }
    catch (JsonException& ex)
    {
        throw JsonException("JSON read error: " + toString(ex.what()));
    }
}

EXPORT Base::JsonObject* Base::JsonArray::getObject(uint index)
{
    return (JsonObject*)get(index, JsonType::OBJECT);
}

EXPORT Base::JsonArray* Base::JsonArray::getArray(uint index)
{
    return (JsonArray*)get(index, JsonType::ARRAY);
}

EXPORT string Base::JsonArray::getString(uint index)
{
    return ((JsonString*)get(index, JsonType::STRING))->value;
}

EXPORT float Base::JsonArray::getNumber(uint index)
{
    return ((JsonNumber*)get(index, JsonType::NUMBER))->value;
}

EXPORT bool Base::JsonArray::getBool(uint index)
{
    return ((JsonBool*)get(index, JsonType::BOOL))->value;
}

// TODO NULLABLE

EXPORT Base::JsonType Base::JsonObject::getType(string name)
{
    if (values.find(name) == values.end())
        throw JsonException("Value with name " + name + " was not found");
    
    return values[name]->getType();
}

Base::JsonAny* Base::JsonObject::get(string name, JsonType type)
{
    try
    {
        JsonType vType = getType(name);
        if (vType != type)
            throw JsonException("Unexpected type " + JsonTypeName[vType] + ", expected " + JsonTypeName[type]);

        return values[name];
    }
    catch (JsonException& ex)
    {
        throw JsonException("JSON read error: " + toString(ex.what()));
    }
}

EXPORT Base::JsonObject* Base::JsonObject::getObject(string name)
{
    return (JsonObject*)get(name, JsonType::OBJECT);
}

EXPORT Base::JsonArray* Base::JsonObject::getArray(string name)
{
    return (JsonArray*)get(name, JsonType::ARRAY);
}

EXPORT string Base::JsonObject::getString(string name)
{
    return ((JsonString*)get(name, JsonType::STRING))->value;
}

EXPORT float Base::JsonObject::getNumber(string name)
{
    return ((JsonNumber*)get(name, JsonType::NUMBER))->value;
}

EXPORT bool Base::JsonObject::getBool(string name)
{
    return ((JsonBool*)get(name, JsonType::BOOL))->value;
}