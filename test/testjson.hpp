#pragma once

#include "file/json.hpp"

namespace Base
{
    void testJSON(ResourceHandler* resHandler)
    {
        cout << "testJSON" << endl;
        
        // 1
        try
        {
            Base::JsonFile jf(resHandler->find("hello.json"));

            cout << jf.getString("foo") << endl;

            Base::JsonObject* obj =  jf.getArray("Objects")->getObject(0);
            cout << obj->getNumber("int") << endl;
            cout << obj->getNumber("float") << endl;
            cout << obj->getString("str") << endl;
        }
        catch (Base::JsonException& ex)
        {
            cout << ex.what() << endl;
        }
    }
}