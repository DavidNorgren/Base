#include "common.hpp"
#include "file/json.hpp"


EXPORT Base::JsonFile::JsonFile(string filename)
{

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
        
        root = readObject();
    }
    catch (JsonException& ex)
    {
        std::cout << "JSON Error: " << ex.what() << " at line " << line << ", column " << column << std::endl;
    }
}

char Base::JsonFile::readCharacter()
{
    // Check EOF
    if (position > size)
        throw JsonException("Unexpected end of file");

    do
    {
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

string Base::JsonFile::readString()
{
    uint stringStart = position;

    while (true)
    {
        // Check EOF
        if (position > size)
            throw JsonException("Unexpected end of file");
            
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

Base::JsonObject* Base::JsonFile::readObject()
{
    JsonObject* obj = new JsonObject();

    while (readCharacter() != Character::CURLY_END)
    {
        string name = readString();
        std::cout << name << std::endl;

        // Check Colon
        if (lastChar != Character::COLON)
            throw JsonException("Expected :");
        
        readCharacter();

        // Read value
        obj->values[name] = readAny();

        // End of values
        if (lastChar == Character::CURLY_END)
            break;

        // Look for value separator
        if (lastChar != Character::COMMA)
            throw JsonException("Expected ,");
    }

    return obj;
}

Base::JsonAny* Base::JsonFile::readAny()
{
   /* if (lastChar == Character::CURLY_BEGIN)

    else if (lastChar == Character::SQUARE_BEGIN)

    else if (lastChar == Character::QUOTE)
        return readString();
    else if ((lastChar >= Character::NUM_0 && lastChar <= Character::NUM_9) || lastChar == Character::MINUS)
    
    else
*/
return 0;
}