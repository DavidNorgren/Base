#include "common.hpp"
#include "file/json.hpp"
#include "file/filefunc.hpp"
#include "util/stringfunc.hpp"

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

// Read methods

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
        if (readCharacter() != CURLY_BEGIN)
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
    string str = "";

    while (true)
    {
        // Check EOF
        if (position >= size)
            throw JsonException("Unexpected end of file");
        
        // Advance
        lastChar = data[position++];
        column++;
        
        // Check end of string
        if (lastChar == QUOTE)
            break;

        // Check invalid linebreak
        if (lastChar == RETURN || lastChar == NEW_LINE)
            throw JsonException("Unexpected linebreak in string");

        // Read special character
        if (lastChar == BACKSLASH)
        {
            // Check EOF
            if (position >= size)
                throw JsonException("Unexpected end of file");

            // Advance
            lastChar = data[position++];
            column++;

            switch (lastChar)
            {
                case QUOTE:      str += "\""; break;
                case BACKSLASH:  str += "\\"; break;
                case N:          str += "\n"; break;
                case T:          str += "\t"; break;
                case U:
                {
                    // Parse Hex ASCII code
                    string hex = "";
                    repeat (4)
                    {
                        // Check EOF
                        if (position >= size)
                            throw JsonException("Unexpected end of file");

                        // Advance and add to Hex ASCII code
                        lastChar = data[position++];
                        column++;
                        hex += lastChar;
                    }
                    // str += hex_to_char(hex) // TODO
                    break;
                }
                default:
                    str += lastChar;
                    break;
            }
        }
        else
            str += lastChar;
    }

    uint stringEnd = position - 1;

    readCharacter();

    return str;
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
    while ((lastChar >= NUM_0 && lastChar <= NUM_9) ||
           lastChar == MINUS || lastChar == PLUS ||
           lastChar == E || lastChar == CAPITAL_E ||
           lastChar == POINT);

    // Skip whitespace
    if (lastChar == TAB ||
        lastChar == SPACE ||
        lastChar == NEW_LINE ||
        lastChar == RETURN)
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
        if (lastChar == TAB)
            column += 4;
        else if (lastChar == NEW_LINE)
        {
            line++;
            column = 0;
        }
        else
            column++;
    }
    // Skip blank characters
    while (lastChar == TAB ||
           lastChar == SPACE ||
           lastChar == NEW_LINE ||
           lastChar == RETURN ||
           lastChar > 127);

    return lastChar;
}

Base::JsonAny* Base::JsonFile::readJsonAny()
{
    if (lastChar == CURLY_BEGIN)
        return readJsonObject();
    else if (lastChar == SQUARE_BEGIN)
        return readJsonArray();
    else if (lastChar == QUOTE)
        return readJsonString();
    else if ((lastChar >= NUM_0 && lastChar <= NUM_9) || lastChar == MINUS)
        return readJsonNumber();
    else
        return readJsonWord();
}

Base::JsonObject* Base::JsonFile::readJsonObject(bool isRoot)
{
    JsonObject* obj = isRoot ? this : new JsonObject();

    while (readCharacter() != CURLY_END)
    {
        string name = readString();

        // Check Colon
        if (lastChar != COLON)
            throw JsonException("Expected :");
        
        readCharacter();

        // Read value
        obj->values[name] = readJsonAny();
        obj->keys.push_back(name);

        // End of values
        if (lastChar == CURLY_END)
            break;

        // Look for value separator
        if (lastChar != COMMA)
            throw JsonException("Expected ,");
    }

    if (!isRoot)
        readCharacter();

    return obj;
}

Base::JsonArray* Base::JsonFile::readJsonArray()
{
    JsonArray* arr = new JsonArray();

    while (readCharacter() != SQUARE_END)
    {
        arr->values.push_back(readJsonAny());
        
        // End of values
        if (lastChar == SQUARE_END)
            break;

        // Look for value separator
        if (lastChar != COMMA)
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
        case T:
        {
            repeat (4)
                readCharacter();
            
            return new JsonBool(true);
        }
        
        // "false"
        case F:
        {
            repeat (5)
                readCharacter();
            
            return new JsonBool(false);
        }

        // "null"
        case N:
        {
            repeat (4)
                readCharacter();

            return new JsonNull();
        }
    }

    throw JsonException("Unrecognized word");
}

// Writing methods

string saveStr;
uint tabs;
bool addComma;

EXPORT void Base::JsonFile::save(string filename)
{
    saveStr = "";
    tabs = 0;
    addComma = false;
    write();

    std::ofstream outStream;
    outStream.open(filename);
    outStream << saveStr;
    outStream.close();
}

void writeTabs()
{
    repeat (tabs)
        saveStr += '\t';
}

void writeChar(Character ch)
{
    saveStr += ch;
}

void writeString(string str)
{
    saveStr += str;
}

void writeEndLast()
{
    // End previous object
    if (addComma)
        writeChar(COMMA);

    if (saveStr.length() > 0)
        writeChar(NEW_LINE);

    writeTabs();
}

void writeStartSet(Character ch)
{
    writeChar(ch);
    tabs++;
    addComma = false;
}

void writeEndSet(Character ch)
{
    writeChar(NEW_LINE);
    tabs--;
    writeTabs();
    writeChar(ch);
    addComma = true;
}

void Base::JsonObject::write()
{
    writeStartSet(CURLY_BEGIN);

    // Go through (named) keys in the same order as added
    for (uint n = 0; n < keys.size(); n++)
    {
        writeEndLast();
        writeChar(QUOTE);
        writeString(keys[n]);
        writeChar(QUOTE);
        writeChar(COLON);
        writeChar(SPACE);
        values[keys[n]]->write();
        addComma = true;
    }
    writeEndSet(CURLY_END);
}

void Base::JsonArray::write()
{
    // Whether to print on a single line (No objects/arrays in the set)
    bool singleLine = true;
    for (uint n = 0; n < values.size(); n++)
    {
        JsonType type = values[n]->getType();
        if (type == JsonType::OBJECT ||type == JsonType::ARRAY)
        {
            singleLine = false;
            break;
        }
    }

    // Go through (unnamed) elements
    if (singleLine)
    {
        // Single-line style, eg. [ 10, 20, 30 ]
        writeChar(SQUARE_BEGIN);
        writeChar(SPACE);
        for (uint n = 0; n < values.size(); n++)
        {
            if (n > 0)
            {
                writeChar(COMMA);
                writeChar(SPACE);
            }
            values[n]->write();
        }
        writeChar(SPACE);
        writeChar(SQUARE_END);
    }
    else
    {
        // Multi-line style
        writeStartSet(SQUARE_BEGIN);
        for (uint n = 0; n < values.size(); n++)
        {
            writeEndLast();
            values[n]->write();
            addComma = true;
        }
        writeEndSet(SQUARE_END);
    }
}

void Base::JsonString::write()
{
    writeChar(QUOTE);
    for (uint i = 0; i < value.length(); i++)
    {
        char ch = value[i];
        if (ch == NEW_LINE)
            writeString("\\n");
        else if (ch == TAB)
            writeString("\\t");
        else if (ch == QUOTE)
            writeString("\\\"");
        else if (ch == BACKSLASH)
            writeString("\\\\");
        // TODO unicode
        else
            writeChar((Character)ch);
    }
    writeChar(QUOTE);
}

void Base::JsonNumber::write()
{
    writeString(toString(value));
}

void Base::JsonBool::write()
{
    writeString(value ? "true": "false");
}

void Base::JsonNull::write()
{
    writeString("null");
}

// Get methods

EXPORT Base::JsonType Base::JsonArray::getType(uint index)
{
    if (index < 0 || index >= values.size())
        throw JsonException("Index " + toString(index) + " out of range [0, " + toString(values.size() - 1) + "]");

    return values[index]->getType();
}

EXPORT bool Base::JsonArray::isNull(uint index)
{
    return (getType(index) == JsonType::NULLVALUE);
}

Base::JsonAny* Base::JsonArray::get(uint index, JsonType type)
{
    try
    {
        JsonType vType = getType(index);
        if (vType != type)
            throw JsonException("Unexpected type " + JsonTypeName[(int)vType] + ", expected " + JsonTypeName[(int)type]);

        return values[index];
    }
    catch (JsonException& ex)
    {
        throw JsonException("JSON get error: " + toString(ex.what()));
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

EXPORT Base::JsonType Base::JsonObject::getType(string name)
{
    if (values.find(name) == values.end())
        throw JsonException("Value with name " + name + " was not found");
    
    return values[name]->getType();
}

EXPORT bool Base::JsonObject::isNull(string name)
{
    return (getType(name) == JsonType::NULLVALUE);
}

Base::JsonAny* Base::JsonObject::get(string name, JsonType type)
{
    try
    {
        JsonType vType = getType(name);
        if (vType != type)
            throw JsonException("Unexpected type " + JsonTypeName[(int)vType] + ", expected " + JsonTypeName[(int)type]);

        return values[name];
    }
    catch (JsonException& ex)
    {
        throw JsonException("JSON get error: " + toString(ex.what()));
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

// Adding methods

EXPORT void Base::JsonArray::addObject(JsonObject* obj)
{
    values.push_back(obj);
}

EXPORT void Base::JsonArray::addArray(JsonArray* arr)
{
    values.push_back(arr);
}

EXPORT void Base::JsonArray::addString(string value)
{
    values.push_back(new JsonString(value));
}

EXPORT void Base::JsonArray::addNumber(float value)
{
    values.push_back(new JsonNumber(value));
}

EXPORT void Base::JsonArray::addBool(bool value)
{
    values.push_back(new JsonBool(value));
}

EXPORT void Base::JsonArray::addNull()
{
    values.push_back(new JsonNull());
}

EXPORT void Base::JsonObject::addObject(string name, JsonObject* obj)
{
    values[name] = obj;
    keys.push_back(name);
}

EXPORT void Base::JsonObject::addArray(string name, JsonArray* arr)
{
    values[name] = arr;
    keys.push_back(name);
}

EXPORT void Base::JsonObject::addString(string name, string value)
{
    values[name] = new JsonString(value);
    keys.push_back(name);
}

EXPORT void Base::JsonObject::addNumber(string name, float value)
{
    values[name] = new JsonNumber(value);
    keys.push_back(name);
}

EXPORT void Base::JsonObject::addBool(string name, bool value)
{
    values[name] = new JsonBool(value);
    keys.push_back(name);
}

EXPORT void Base::JsonObject::addNull(string name)
{
    values[name] = new JsonNull();
    keys.push_back(name);
}