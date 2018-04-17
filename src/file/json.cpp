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

    if (readCharacter() != Character::CURLY_BEGIN)
        return;
    
    root = readObject();
}

char Base::JsonFile::readCharacter()
{
    // Check EOF

    do
    {
        lastChar = data[position++];
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
    while (lastChar == Character::TAB ||
           lastChar == Character::SPACE ||
           lastChar == Character::NEW_LINE ||
           lastChar == Character::RETURN ||
           lastChar > 127);

    return lastChar;
}

string Base::JsonFile::readString()
{
    size_t stringStart = position;

    while (true)
    {
        // Check EOF
        lastChar = data[position++];
        column++;
        
        // End of string
        if (lastChar == Character::QUOTE)
            break;

        // Invalid linebreak
        // Special character
    }

    return string(&data[stringStart], (position - stringStart) - 1);
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
            break; // ERROR
        
        readCharacter();

        obj->values[name] = readAny();
    }

    return obj;
}

Base::JsonAny* Base::JsonFile::readAny()
{
    return 0;
}